#pragma once

#include <inttypes.h>

#include <array>

#include "Definitions.hpp"

typedef int32_t BoardValue;

namespace BoardValues {
constexpr BoardValue PAWN = 1;
constexpr BoardValue KNIGHT = 2;
constexpr BoardValue BISHOP = 3;
constexpr BoardValue ROOK = 4;
constexpr BoardValue QUEEN = 5;
constexpr BoardValue KING = 6;
constexpr std::array<BoardValue,6> ALL{PAWN,KNIGHT,BISHOP,ROOK,QUEEN,KING};
};  // namespace BoardValues

class Board {
   public:
    Board();

    auto init_standard() -> void;
    auto clear() -> void;
    auto print() const -> void;

    std::array<int32_t, BOARD_DIM * BOARD_DIM> positions;
};
