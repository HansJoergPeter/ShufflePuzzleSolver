#pragma once

#include "Config.h"

#include <iostream>
#include <queue>

#include <cstdint>

namespace spsolver {

/*
  Runs a brute-force BFS to find the shortest winning strategy from
  'init' until a configuration that satisfies the unary predicate 'isTarget'
*/
template <typename Configuration, typename IsTarget>
void solve(Configuration const init, IsTarget const& isTarget) {
  using std::cout;

  // prints a shuffle move that connects two configurations
  auto const printMove = [](Configuration const from, Configuration const to) {
    auto const h1 = from.hole();
    auto const h2 = to.hole();
    if (h2 == h1 + 1) {
      cout << "Move " << (int)to(h1) << " left\n";
    } else if (h2 == h1 - 1) {
      cout << "Move " << (int)from(h2) << " right\n";
    } else if (h2 == h1 + 4) {
      cout << "Move " << (int)to(h1) << " up\n";
    } else if (h2 == h1 - 4) {
      cout << "Move " << (int)from(h2) << " down\n";
    } else {
      cout << from << " --> " << to << '\n';
    }
  };

  // called if the target configuration is reached
  auto const reachedTarget = [&printMove](Configuration conf,
                                          auto const& parents) {
    cout << "Reverse winning strategy:\n";
    for (auto last = conf, conf = parents.at(last); last != conf;
         last = conf, conf = parents.at(last)) {
      printMove(conf, last);
    }
  };

  // enumerates all configurations reachable by a single shuffle move
  auto const successors = [](Configuration const conf, auto func) {
    auto const succ = [func{std::move(func)}, h1{conf.hole()}, conf](
                          int const drow, int const dcol) {
      if (auto const h2 = conf.hole(drow, dcol); h2 != -1) {
        auto conf2 = conf;
        conf2.swapTiles(h2, h1);
        func(conf2);
      }
    };
    succ(1, 0);
    succ(-1, 0);
    succ(0, 1);
    succ(0, -1);
  };

  // run BFS to find the shortest winning strategy
  uint64_t steps = 0;
  HashMap<Configuration, Configuration> parents{{init, init}};
  for (std::queue<Configuration> queue{{init}}; !queue.empty(); queue.pop()) {
    auto const conf = queue.front();
    if (isTarget(conf)) {
      reachedTarget(conf, parents);
      return;
    }

    // schedule successors
    successors(conf, [&](auto const conf2) {
      if (auto const [it, n] = parents.emplace(conf2, conf); n) {
        queue.push(conf2);
      }
    });

    // progression logging
    if ((++steps % 1000000) == 0) {
      cout << "steps: " << steps << "\t visited: " << parents.size()
           << "\t queue: " << queue.size() << '\n';
    }
  }

  cout << "No winning strategy could be found!\n";
}

}  // namespace spsolver
