#pragma once

#include <inttypes.h>

#include "Definitions.hpp"

class Board {
   public:
    Board();

    void init_standard();
    void clear();
    void print();

    int32_t positions[BOARD_DIM * BOARD_DIM];
};
