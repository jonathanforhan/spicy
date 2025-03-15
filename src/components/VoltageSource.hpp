#pragma once

#include <complex>
#include <cstring>
#include "Component.hpp"

namespace spicy {

class VoltageSource : public Component {
public:
    VoltageSource(const std::string& name, double dc, std::complex<double> ac) noexcept
        : Component{ComponentType::R, name},
          _dc{dc},
          _ac{ac} {}

    VoltageSource(std::string&& name, double dc, std::complex<double> ac) noexcept
        : Component{ComponentType::R, name},
          _dc{dc},
          _ac{ac} {}

    double dc() const noexcept { return _dc; }

    std::complex<double> ac() const noexcept { return _ac; }

private:
    double _dc;
    std::complex<double> _ac;
};

} // namespace spicy
