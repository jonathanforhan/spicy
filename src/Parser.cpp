#include "Parser.hpp"

#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstddef>
#include <cstring>
#include <string_view>
#include "log.hpp"

namespace spicy {

std::vector<Component> Parser::parse() {
    std::vector<Component> components;
    components.reserve(100);

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
            auto line = std::string_view{curr, line_size};
            parse_line(components, line);
            curr = next + 1;
        }
    }

    // handle case of input not ending with newline
    if (curr != input.end()) {
        auto line_size = static_cast<size_t>(next - curr);
        auto line = std::string_view{curr, line_size};
        parse_line(components, line);
    }

    return components;
}

void Parser::parse_line(std::vector<Component>& components, std::string_view line) {
    if (line.empty()) {
        return;
    }

    // use curr and next to create sliding window
    auto curr = line.begin();
    auto next = curr; // for type deduction

    while ((next = std::find_if(curr, line.end(), isspace)) != line.end()) {
        if (next - curr != 0) {
            DEBUG("'%.*s'", (int)(next - curr), curr);
        }
        curr = next + 1;
    }

    // handle case of line not ending with space
    if (curr != line.end()) {
        if (next - curr != 0) {
            DEBUG("'%.*s'", (int)(next - curr), curr);
        }
    }

    DEBUG();
}

} // namespace spicy
