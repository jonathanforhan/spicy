#include "Parser.hpp"
#include <fmt/base.h>
#include <fmt/core.h>
#include <cassert>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include "components/Component.hpp"

#include <Eigen/Sparse>

using namespace spicy;

namespace fs = std::filesystem;

int main(int argc, char** argv) {
    Eigen::SparseMatrix<double> A(1000, 1000);

    auto netlist_path = fs::path{argv[0]}.parent_path() / "netlists" / "parser.test.cir";
    if (!fs::exists(netlist_path)) {
        fmt::println(stderr, "could not find path: {}", netlist_path.string());
        return -1;
    }

    std::ifstream ifs{netlist_path};
    if (!(ifs.is_open() and ifs.good())) {
        fmt::println(stderr, "problem opening file {}", netlist_path.string());
        return -1;
    }

    std::stringstream ss;
    ss << ifs.rdbuf();

    Parser parser(ss.str());

    ComponentList list;

    [[maybe_unused]] auto result = parser.parse(list);
    assert(result == Parser::Success);

    fmt::println("{}", list.components[0]->name());
    fmt::println("{}", list.components[1]->name());

    for (int i = 0; auto& node : list.nodes) {
        for (auto& component : node) {
            std::cout << "Node " << i << " component: " << component << '\n';
        }
        i++;
    }

    return 1;
}
