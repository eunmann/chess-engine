#pragma once

#include <inttypes.h>

#include <array>

#include "Definitions.hpp"

typedef int32_t BoardValue;

namespace BoardValues {
  constexpr BoardValue EMPTY = 0;
  constexpr BoardValue PAWN = 1;
  constexpr BoardValue KNIGHT = 2;
  constexpr BoardValue BISHOP = 3;
  constexpr BoardValue ROOK = 4;
  constexpr BoardValue QUEEN = 5;
  constexpr BoardValue KING = 6;
  constexpr std::array<BoardValue, 6> ALL{PAWN, KNIGHT, BISHOP,
    ROOK, QUEEN, KING};
}; // namespace BoardValues

class Board {
  public:
  Board() noexcept;

  auto init_standard() noexcept -> void;
  auto clear() noexcept -> void;
  auto print() const noexcept -> void;

  std::array<BoardValue, BOARD_DIM* BOARD_DIM> positions;
};
