#include "Parser.hpp"

#include <fmt/base.h>
#include <fmt/core.h>
#include <algorithm>
#include <cassert>
#include <cctype>
#include <charconv>
#include <complex>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <string>
#include <string_view>
#include <system_error>
#include "components/Component.hpp"
#include "components/Resistor.hpp"
#include "components/VoltageSource.hpp"

#ifndef NDEBUG
#define DEBUG(...) fmt::println(__VA_ARGS__)
#else
#define DEBUG(...) (void)0
#endif
#define ERROR(...) fmt::println(stderr, "[ERROR] "__VA_ARGS__)

namespace spicy {

Parser::Result Parser::parse(ComponentList& list) {
    _list = &list;
    _list->components.reserve(100);

    Result res = Success;

    // use string view of _input to prevent copies
    std::string_view input{_input};

    // use curr and next to create sliding window
    auto curr = input.begin();
    auto next = curr; // for type deduction

    while ((next = std::find(curr, input.end(), '\n')) != input.end()) {
        if (next + 1 != input.end() && next[1] == '+') {
            // handle line continuation by removing '\n' in origin input, overwriting '+' and redoing find
            size_t i = static_cast<size_t>(next - input.begin());
            std::memcpy(&_input[i], "  ", 2);
        } else {
            std::string_view line{curr, next};
            res = parse_line(line);
            if (res != Success) {
                return res;
            }
            curr = next + 1;
        }
    }

    // handle case of input not ending with newline
    std::string_view line{curr, next};
    res = parse_line(line);

    return res;
}

Parser::Result Parser::parse_arg(std::string_view s, auto& arg) {
    Result res = Success;
    auto r = std::from_chars(s.begin(), s.end(), arg);
    if (r.ec != std::errc{} || r.ptr != s.end()) {
        res = InvalidArgument;
    }
    return res;
}

Parser::Result Parser::parse_line(std::string_view line) {
    auto match = []<size_t N>(const char* x, const char(&s)[N]) { return std::strncmp(x, s, N - 1) == 0; };

    Result res = Success;

    // get first non-space
    if (auto it = std::ranges::find_if_not(line, isspace); it != line.end()) {
        if (match(it, "r")) {
            res = parse_resistor(line);
        } else if (match(it, "c")) {
            res = parse_capacitor(line);
        } else if (match(it, "l")) {
            res = parse_inductor(line);
        } else if (match(it, "v")) {
            res = parse_voltage_source(line);
        } else if (match(it, "i")) {
            res = parse_current_source(line);
        } else if (match(it, ".model")) {
            res = parse_model(line);
        } else if (match(it, "*")) {
            // comment
        } else {
            DEBUG("[ERROR] parsing unknown component on line '{:.{}}", line.data(), line.length());
            res = UnknownComponent;
        }
    }

    return res;
}

Parser::Result Parser::for_each_word(std::string_view line, auto&& fn) {
    Result res = Success;

    // use curr and next to create sliding window
    auto curr = line.begin();
    auto next = curr; // for type deduction
    size_t i = 0;

    while ((next = std::find_if(curr, line.end(), isspace)) != line.end()) {
        if (std::string_view word{curr, next}; !word.empty()) {
            res = fn(word, i++);
            if (res != Success) {
                return res;
            }
        }
        curr = next + 1;
    }

    // handle case of line not ending with space
    if (std::string_view word{curr, next}; !word.empty()) {
        res = fn(word, i++);
    }

    return res;
}

Parser::Result Parser::parse_resistor(std::string_view line) {
    Result res = Success;

    std::string name;
    size_t node0 = 0, node1 = 0;
    double resistance = 0.0;

    res = for_each_word(line, [&](std::string_view word, size_t i) {
        switch (i) {
            case 0:
                assert(word[0] == 'r');
                name = std::string{word.begin() + 1, word.end()};
                break;
            case 1:
                res = parse_arg(word, node0);
                break;
            case 2:
                res = parse_arg(word, node1);
                break;
            case 3:
                res = parse_arg(word, resistance);
                break;
            case 4:
                // model, nop for now (TODO)
                break;
            default:
                res = InvalidArgument;
                break;
        }
        return res;
    });

    size_t index = _list->components.size();
    _list->components.emplace_back(std::make_unique<Resistor>(std::move(name), resistance));

    size_t node_max = std::max(node0, node1);
    while (_list->nodes.size() <= node_max) {
        _list->nodes.emplace_back();
    }

    _list->nodes[node0].push_back(index);
    _list->nodes[node1].push_back(index);

    return Success;
}

Parser::Result Parser::parse_capacitor(std::string_view line) {
    DEBUG("C: '{:.{}}'", line.data(), line.length());
    return Success;
}

Parser::Result Parser::parse_inductor(std::string_view line) {
    DEBUG("L: '{:.{}}'", line.data(), line.length());
    return Success;
}

Parser::Result Parser::parse_voltage_source(std::string_view line) {
    Result res = Success;

    std::string name;
    size_t node0 = 0, node1 = 0;
    double dc = 0.0;
    double ac_real = 0.0, ac_imag = 0.0;

    res = for_each_word(line, [&](std::string_view word, size_t i) {
        switch (i) {
            case 0:
                assert(word[0] == 'v');
                name = std::string{word.begin() + 1, word.end()};
                break;
            case 1:
                res = parse_arg(word, node0);
                break;
            case 2:
                res = parse_arg(word, node1);
                break;
            case 3:
                assert(word == "dc");
                break;
            case 4:
                res = parse_arg(word, dc);
                break;
            case 5:
                assert(word == "ac");
                break;
            case 6:
                res = parse_arg(word, ac_real);
                break;
            case 7:
                res = parse_arg(word, ac_imag);
                break;
            case 8:
            case 9:
            case 10:
            case 11:
            case 12:
            case 13:
                // distortion (TODO)
                break;
            default:
                res = InvalidArgument;
                break;
        }
        return res;
    });

    size_t index = _list->components.size();
    _list->components.emplace_back(
        std::make_unique<VoltageSource>(std::move(name), dc, std::complex{ac_real, ac_imag}));

    size_t node_max = std::max(node0, node1);
    while (_list->nodes.size() <= node_max) {
        _list->nodes.emplace_back();
    }

    _list->nodes[node0].push_back(index);
    _list->nodes[node1].push_back(index);

    return Success;
}

Parser::Result Parser::parse_current_source(std::string_view line) {
    DEBUG("I: '{:.{}}'", line.data(), line.length());
    return Success;
}

Parser::Result Parser::parse_model(std::string_view line) {
    DEBUG("M: '{:.{}}'", line.data(), line.length());
    return Success;
}

} // namespace spicy
