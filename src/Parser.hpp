#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include "components/Component.hpp"

namespace spicy {

class Parser {
public:
    enum Result : uint8_t {
        Success = 0,
        InvalidArgument,
        UnknownComponent,
    };

public:
    Parser(const std::string& input)
        : _input{input},
          _list{nullptr} {}

    Parser(std::string&& input)
        : _input{std::move(input)},
          _list{nullptr} {}

    [[nodiscard]] Result parse(ComponentList& list);

private:
    [[nodiscard]] Result parse_arg(std::string_view s, auto& arg);

    [[nodiscard]] Result parse_line(std::string_view line);

    [[nodiscard]] Result for_each_word(std::string_view line, auto&& fn);

    [[nodiscard]] Result parse_resistor(std::string_view line);

    [[nodiscard]] Result parse_capacitor(std::string_view line);

    [[nodiscard]] Result parse_inductor(std::string_view line);

    [[nodiscard]] Result parse_voltage_source(std::string_view line);

    [[nodiscard]] Result parse_current_source(std::string_view line);

    [[nodiscard]] Result parse_model(std::string_view line);

private:
    std::string _input;
    ComponentList* _list;
};

} // namespace spicy
