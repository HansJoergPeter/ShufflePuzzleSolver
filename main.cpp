#include <iostream>
#include <queue>
#include <stack>
#include <string>
#include <vector>

#include <absl/container/flat_hash_map.h>

#include <cstdint>
#include <cstdlib>

using namespace std;

namespace {

class Configuration {
 public:
  constexpr Configuration() noexcept = default;
  constexpr Configuration(Configuration const&) noexcept = default;
  constexpr Configuration(Configuration&&) noexcept = default;

  constexpr Configuration& operator=(Configuration const&) noexcept = default;
  constexpr Configuration& operator=(Configuration&&) noexcept = default;

  explicit Configuration(char const* const* const argv) noexcept {
    for (int8_t offs = 15; offs >= 0; --offs) {
      tiles_ <<= 4;
      if (auto const v = atoi(argv[offs]) & 0b00001111) {
        tiles_ |= v - 1;
      } else {
        hole_ = offs;
      }
    }
  }

  constexpr uint8_t operator()(int const offs) const noexcept {
    return offs == hole_ ? 0 : ((tiles_ >> (offs * 4)) & 0b00001111) + 1;
  }
  constexpr uint8_t operator()(int const row, int const col) const noexcept {
    return (*this)(row * 4 + col);
  }

  constexpr bool operator==(Configuration const c) const noexcept {
    return tiles_ == c.tiles_ && hole_ == c.hole_;
  }
  constexpr bool operator!=(Configuration const c) const noexcept {
    return !(*this == c);
  }

  constexpr void swapTiles(uint64_t offs1, uint64_t offs2) noexcept {
    if (hole_ == static_cast<int8_t>(offs1)) {
      hole_ = offs2;
    } else if (hole_ == static_cast<int8_t>(offs2)) {
      hole_ = offs1;
    }

    offs1 *= 4;
    offs2 *= 4;
    uint64_t const mask1 = 15ULL << offs1;
    uint64_t const mask2 = 15ULL << offs2;
    auto v1 = (tiles_ & mask1) >> offs1;
    auto v2 = (tiles_ & mask2) >> offs2;
    tiles_ &= ~(mask1 | mask2);
    tiles_ |= v1 << offs2;
    tiles_ |= v2 << offs1;
  }

  constexpr int8_t hole(int8_t const drow = 0,
                        int8_t const dcol = 0) const noexcept {
    if (hole_ == -1 || (!drow && !dcol)) {
      return hole_;
    }
    int8_t const r = (hole_ / 4) + drow;
    if (r < 0 || r > 3) {
      return -1;
    }
    int8_t const c = (hole_ % 4) + dcol;
    if (c < 0 || c > 3) {
      return -1;
    }
    return r * 4 + c;
  }

  constexpr auto hashCode() const noexcept { return tiles_; }

  friend auto& operator<<(std::ostream& os, Configuration const c) {
    return os << c.tiles_ << '/' << (int)c.hole_;
  }

 private:
  uint64_t tiles_{0ULL};
  int8_t hole_{-1};
};

//----------------------------------------------------------------------------

template <typename Func>
void successors(Configuration conf, Func func) noexcept {
  auto const succ = [func{move(func)}, h1{conf.hole()}, conf](int const drow,
                                                              int const dcol) {
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
}

//----------------------------------------------------------------------------

void printMove(Configuration const from, Configuration const to) {
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
}

}  // namespace

//----------------------------------------------------------------------------

namespace std {

template <>
class hash<Configuration> {
 public:
  constexpr auto operator()(Configuration const c) const noexcept {
    return c.hashCode();
  }
};

}  // namespace std

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

int main(int const argc, char const* const argv[]) {
  if (argc < 17) {
    cerr << "Usage:  " << argv[0] << "  Tile1  Tile2  ...  Tile16\n";
    return 1;
  }

  auto const target = [](Configuration const conf) {
    if (conf.hole() != 15) {
      return false;
    }
    for (int offs = 0; offs < 15; ++offs) {
      if (conf(offs) != offs + 1) {
        return false;
      }
    }
    return true;
  };

  auto const reachedTarget = [](Configuration conf, auto const& parents) {
    cout << "Reverse winning strategy:\n";
    // cout << conf.str() << '\n';
    for (auto last = conf, conf = parents.at(last); last != conf;
         last = conf, conf = parents.at(last)) {
      printMove(conf, last);
      // cout << conf.str() << '\n';
    }
  };

  uint64_t steps = 0;
  Configuration const init{argv + 1};
  ::absl::flat_hash_map<Configuration, Configuration> parents{{init, init}};
  for (queue<Configuration> queue{{init}}; !queue.empty();) {
    auto const conf = queue.front();
    // cout << conf << '\n';
    queue.pop();

    if (target(conf)) {
      reachedTarget(conf, parents);
      break;
    }

    successors(conf, [&](auto const conf2) {
      if (auto const [it, n] = parents.emplace(conf2, conf); n) {
        queue.push(conf2);
      }
    });

    if ((++steps % 1000000) == 0) {
      cout << steps << " / " << parents.size() << " / " << queue.size() << '\n';
    }
  }
}
