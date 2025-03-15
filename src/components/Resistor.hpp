#pragma once

#include <cstring>
#include "Component.hpp"

namespace spicy {

class Resistor : public Component {
public:
    Resistor(const std::string& name, double resistance) noexcept
        : Component{ComponentType::R, name},
          _resistance{resistance} {}

    Resistor(std::string&& name, double resistance) noexcept
        : Component{ComponentType::R, name},
          _resistance{resistance} {}

    double resistance() const noexcept { return _resistance; }

private:
    double _resistance;
};

} // namespace spicy
