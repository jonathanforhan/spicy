#pragma once

#include <complex>
#include <cstdint>
#include <cstring>
#include <vector>

namespace spicy {

enum class ComponentType : int8_t {
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
    using complex_t = std::complex<double>;

    using impedance_callback = complex_t (*)(const Component&, complex_t v, complex_t i, double t);
    using voltage_callback = complex_t (*)(const Component&, double t);
    using current_callback = complex_t (*)(const Component&, double t);

public:
    Component(ComponentType type, std::vector<uint64_t>&& nodes)
        : _type(type),
          _nodes(std::move(nodes)) {}

    impedance_callback impedance = nullptr;

    voltage_callback voltage = nullptr;

    current_callback current = nullptr;

private:
    ComponentType _type;
    std::vector<uint64_t> _nodes;
};

} // namespace spicy
