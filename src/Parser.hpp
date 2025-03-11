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

    ComponentCollection&& parse();

private:
    void parse_line(std::string_view line);

    void for_each_word(std::string_view line, auto&& fn);

    void parse_resistor(std::string_view line);

    void parse_capacitor(std::string_view line);

    void parse_inductor(std::string_view line);

    void parse_voltage_source(std::string_view line);

    void parse_current_source(std::string_view line);

    void parse_model(std::string_view line);

private:
    std::string _input;
    ComponentCollection _collection; // used to give methods access to common collection
};

} // namespace spicy
