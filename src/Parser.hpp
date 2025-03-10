#pragma once

#include <string>
#include <string_view>
#include "Component.hpp"

namespace spicy {

class Parser {
public:
    Parser(const std::string& input)
        : _input(input) {}

    Parser(std::string&& input)
        : _input(std::move(input)) {}

    std::vector<Component> parse();

private:
    // expects the newline to be truncated before receiving line
    void parse_line(std::vector<Component>& components, std::string_view line);

private:
    std::string _input;
};

} // namespace spicy
