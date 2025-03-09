#pragma once

#include <cstring>
#include <vector>
#include "types.hpp"

namespace spicy {

enum class ComponentType : i8 {
    unknown = -1,
    R = 'r', // resistor
    C = 'c', // capacitor
    L = 'l', // inductor
    V = 'v', // voltage source
    I = 'i', // current source
    // E = 'e', // vcvs
    // F = 'f', // cccs
    // G = 'g', // vccs
    // H = 'h', // ccvs
    // D = 'd', // diode
};

class Component final {
public:
    using impedance_callback = complex_t (*)(const Component&, complex_t v, complex_t i, f64 t);
    using voltage_callback = complex_t (*)(const Component&, f64 t);
    using current_callback = complex_t (*)(const Component&, f64 t);

public:
    Component(ComponentType type, std::vector<u64>&& nodes)
        : _type(type),
          _nodes(std::move(nodes)) {}

    impedance_callback impedance = nullptr;

    voltage_callback voltage = nullptr;

    current_callback current = nullptr;

private:
    ComponentType _type;
    std::vector<u64> _nodes;
};

} // namespace spicy
