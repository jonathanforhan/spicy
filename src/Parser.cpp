#include "Parser.hpp"

#include <algorithm>
#include <cassert>
#include <cctype>
#include <charconv>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <string>
#include <string_view>
#include "Component.hpp"
#include "log.hpp"

namespace spicy {

ComponentCollection&& Parser::parse() {
    _collection.components.reserve(100);

    // use string view of _input to prevent copies
    auto input = std::string_view{_input};

    // use curr and next to create sliding window
    auto curr = input.begin();
    auto next = curr; // for type deduction

    while ((next = std::find(curr, input.end(), '\n')) != input.end()) {
        if (next + 1 != input.end() && next[1] == '+') {
            // handle line continuation by removing '\n' in origin input, overwriting '+' and redoing find
            auto i = static_cast<size_t>(next - input.begin());
            std::memcpy(&_input[i], "  ", 2);
        } else {
            auto line_size = static_cast<size_t>(next - curr);
            if (line_size != 0) {
                auto line = std::string_view{curr, line_size};
                parse_line(line);
            }
            curr = next + 1;
        }
    }

    // handle case of input not ending with newline
    auto line_size = static_cast<size_t>(next - curr);
    if (line_size != 0) {
        auto line = std::string_view{curr, line_size};
        parse_line(line);
    }

    return std::move(_collection);
}

void Parser::parse_line(std::string_view line) {
    auto match = [](const char* x, std::string_view s) { return std::strncmp(x, s.data(), s.length()) == 0; };

    auto it = std::ranges::find_if(line, [](char c) { return !std::isspace(c); });
    if (it == line.end()) {
        return;
    }

    if (match(it, "r"))
        parse_resistor(line);
    else if (match(it, "c"))
        parse_capacitor(line);
    else if (match(it, "l"))
        parse_inductor(line);
    else if (match(it, "v"))
        parse_voltage_source(line);
    else if (match(it, "i"))
        parse_current_source(line);
    else if (match(it, ".model"))
        parse_model(line);
    else if (match(it, "*"))
        ; // comment
    else
        ERROR("parsing unknown component on line '%.*s'", (int)line.length(), line.data());
}

void Parser::for_each_word(std::string_view line, auto&& fn) {
    // use curr and next to create sliding window
    auto curr = line.begin();
    auto next = curr; // for type deduction
    size_t i = 0;

    while ((next = std::find_if(curr, line.end(), isspace)) != line.end()) {
        auto word_size = static_cast<size_t>(next - curr);
        if (word_size) {
            auto word = std::string_view{curr, word_size};
            fn(word, i++);
        }
        curr = next + 1;
    }

    // handle case of line not ending with space
    auto word_size = static_cast<size_t>(next - curr);
    if (word_size != 0) {
        auto word = std::string_view{curr, word_size};
        fn(word, i++);
    }
}

void Parser::parse_resistor(std::string_view line) {
    auto name_begin = _collection.name_pool.end();
    int name_size = 0;

    auto nodes_begin = _collection.node_pool.end();
    int nodes_size = 0;

    double resistance = 0.0;

    for_each_word(line, [&, this](std::string_view word, size_t i) {
        switch (i) {
            case 0:
                DEBUG("R name: %.*s", (int)word.length() - 1, word.data() + 1);
                _collection.name_pool.append(word.begin() + 1, word.end());
                name_size = (int)word.length() - 1;
                break;
            case 1:
            case 2:
                DEBUG("R node: %.*s", (int)word.length(), word.data());
                _collection.node_pool.emplace_back();
                std::from_chars(word.begin(), word.end(), _collection.node_pool.back());
                nodes_size++;
                break;
            case 3:
                DEBUG("R resistance: %.*s", (int)word.length(), word.data());
                std::from_chars(word.begin(), word.end(), resistance);
                break;
            case 4:
                DEBUG("R model: %.*s", (int)word.length(), word.data());
                break;
            default:
                break;
        }
    });

    auto name = std::string_view(name_begin, name_begin + name_size);
    auto nodes = std::span(nodes_begin, nodes_begin + nodes_size);

    _collection.components.emplace_back(ComponentType::R, name, nodes);
}

void Parser::parse_capacitor(std::string_view line) {
    DEBUG("C: '%.*s'", (int)line.length(), line.data());
}

void Parser::parse_inductor(std::string_view line) {
    DEBUG("L: '%.*s'", (int)line.length(), line.data());
}

void Parser::parse_voltage_source(std::string_view line) {
    DEBUG("V: '%.*s'", (int)line.length(), line.data());
}

void Parser::parse_current_source(std::string_view line) {
    DEBUG("I: '%.*s'", (int)line.length(), line.data());
}

void Parser::parse_model(std::string_view line) {
    DEBUG("M: '%.*s'", (int)line.length(), line.data());
}

} // namespace spicy
