#include "Configuration.h"
#include "solver.h"

#include <iostream>

int main(int const argc, char const* const argv[]) {
  using namespace ::spsolver;

  if (argc < 10) {
    std::cerr << "Usage:  " << argv[0] << "  Tile1  Tile2  ...  Tile9\n";
    return 1;
  }

  // returns true iff 'conf' is the 3x3 target configuration
  auto const isTarget = [](auto const conf) {
    if (conf.hole() != 8) {
      return false;
    }
    for (int offs = 0; offs < 8; ++offs) {
      if (conf(offs) != offs + 1) {
        return false;
      }
    }
    return true;
  };

  // runs the actual solver
  solve(Configuration<3, 3>{argv + 1}, isTarget);
}
