#pragma once

#include <inttypes.h>

#include <array>

#include "Definitions.hpp"

class Board {
   public:
    Board();

    auto init_standard() -> void;
    auto clear() -> void;
    auto print() -> void;

    std::array<int32_t, BOARD_DIM * BOARD_DIM> positions;
};
