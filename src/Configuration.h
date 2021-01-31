#pragma once

#include <functional>
#include <iostream>

#include <cstdint>
#include <cstdlib>

namespace spsolver {

/*
  A generic sliding puzzle configuration based on a compact representation
  that allows an efficient value semantics.
*/
template <uint8_t rows, uint8_t cols>
class Configuration {
 public:
  constexpr static auto columns = cols;

  constexpr Configuration() noexcept = default;
  constexpr Configuration(Configuration const&) noexcept = default;
  constexpr Configuration(Configuration&&) noexcept = default;

  constexpr Configuration& operator=(Configuration const&) noexcept = default;
  constexpr Configuration& operator=(Configuration&&) noexcept = default;

  explicit Configuration(char const* const* const argv) noexcept {
    static_assert(rows <= 4);
    static_assert(cols <= 4);
    for (int8_t offs = rows * cols - 1; offs >= 0; --offs) {
      tiles_ <<= 4;
      if (auto const v = std::atoi(argv[offs]) & 0b00001111) {
        tiles_ |= v - 1;
      } else {
        hole_ = offs;
      }
    }
  }

  constexpr uint8_t operator()(int const offs) const noexcept {
    return offs == hole_ ? 0 : ((tiles_ >> (offs * 4)) & 0b00001111) + 1;
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
    auto const v1 = (tiles_ & mask1) >> offs1;
    auto const v2 = (tiles_ & mask2) >> offs2;
    tiles_ &= ~(mask1 | mask2);
    tiles_ |= v1 << offs2;
    tiles_ |= v2 << offs1;
  }

  constexpr int8_t hole(int8_t const drow = 0,
                        int8_t const dcol = 0) const noexcept {
    if (hole_ == -1 || (!drow && !dcol)) {
      return hole_;
    }
    int8_t const r = (hole_ / cols) + drow;
    if (r < 0 || r >= rows) {
      return -1;
    }
    int8_t const c = (hole_ % cols) + dcol;
    if (c < 0 || c >= cols) {
      return -1;
    }
    return r * cols + c;
  }

  constexpr auto hashCode() const noexcept { return tiles_; }

  friend auto& operator<<(std::ostream& os, Configuration const c) {
    return os << c.tiles_ << '/' << (int)c.hole_;
  }

 private:
  uint64_t tiles_{0ULL};
  int8_t hole_{-1};
};

}  // namespace spsolver

//----------------------------------------------------------------------------

namespace std {

template <uint8_t rows, uint8_t cols>
class hash<::spsolver::Configuration<rows, cols>> {
 public:
  constexpr auto operator()(
      ::spsolver::Configuration<rows, cols> const c) const noexcept {
    return c.hashCode();
  }
};

}  // namespace std
