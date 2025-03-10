#include <cstdio>
#include <iostream>

#include <filesystem>
#include <fstream>
#include <sstream>
#include "Parser.hpp"

using namespace spicy;

namespace fs = std::filesystem;

int main(int argc, char** argv) {
    auto netlist_path = fs::path(argv[0]).parent_path() / "netlists" / "parser.test.cir";
    if (!fs::exists(netlist_path)) {
        (void)fprintf(stderr, "could not find path: %s\n", netlist_path.c_str());
        return -1;
    }

    std::ifstream ifs(netlist_path);
    if (!(ifs.is_open() and ifs.good())) {
        (void)fprintf(stderr, "problem opening file %s", netlist_path.c_str());
        return -1;
    }

    std::stringstream ss;
    ss << ifs.rdbuf();

    std::string s = ss.str();

    Parser parser(s);
    parser.parse();

    return 1;
}
