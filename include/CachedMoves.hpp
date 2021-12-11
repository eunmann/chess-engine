#pragma once

#include <array>
#include "Definitions.hpp"

namespace CachedMoves {
  extern std::array<BitBoard, Squares::NUM> KNIGHT;
  extern std::array<BitBoard, Squares::NUM> KING;

  auto init() noexcept -> void;
}