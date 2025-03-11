#pragma once

#include <complex>
#include <cstdint>
#include <cstring>
#include <memory>
#include <span>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

namespace spicy {

enum class ComponentType : int8_t {
    Unknown = -1,
    R, // resistor
    C, // capacitor
    L, // inductor
    V, // voltage source
    I, // current source
    // E, // vcvs
    // F, // cccs
    // G, // vccs
    // H, // ccvs
    // D, // diode
    Model,
};

class Component final {
public:
    using complex_t = std::complex<double>;

    using impedance_callback = complex_t (*)(const Component&, complex_t v, complex_t i, double t);
    using voltage_callback = complex_t (*)(const Component&, double t);
    using current_callback = complex_t (*)(const Component&, double t);

public:
    Component(ComponentType type, std::string_view name, std::span<uint64_t> nodes)
        : _type(type),
          _name(name),
          _nodes(nodes) {}

    std::variant<impedance_callback, voltage_callback, current_callback> callback = {};

    Component(const Component&) = delete;
    Component& operator=(const Component&) = delete;

    Component(Component&&) = default;
    Component& operator=(Component&&) = default;

    ~Component() {}

    // private:
    ComponentType _type;
    std::string_view _name;
    std::span<uint64_t> _nodes;
};

class ComponentCollection {
public:
    std::vector<Component> components;
    std::vector<uint64_t> node_pool;
    std::string name_pool;
};

} // namespace spicy
