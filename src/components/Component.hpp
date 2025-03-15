#pragma once

#include <cstdint>
#include <cstring>
#include <memory>
#include <string>
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

class Component {
public:
    Component(ComponentType type, const std::string& name) noexcept
        : _type{type},
          _name{name} {}

    Component(ComponentType type, std::string&& name) noexcept
        : _type{type},
          _name{std::move(name)} {}

    Component(const Component&) = default;
    Component& operator=(const Component&) = default;

    Component(Component&&) noexcept = default;
    Component& operator=(Component&&) noexcept = default;

    virtual ~Component() = default;

    ComponentType type() const noexcept { return _type; }

    const std::string& name() const noexcept { return _name; }

private:
    ComponentType _type;
    std::string _name;
};

struct ComponentList {
    std::vector<std::unique_ptr<Component>> components;
    std::vector<std::vector<size_t>> nodes;
};

} // namespace spicy
