#pragma once

#include <inttypes.h>

#include <array>

#include "Definitions.hpp"

class Board {
   public:
    Board();

    void init_standard();
    void clear();
    void print();

    std::array<int32_t, BOARD_DIM * BOARD_DIM> positions;
};
