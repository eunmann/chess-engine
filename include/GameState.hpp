#pragma once

#include <inttypes.h>

#include "Position.hpp"

class GameState {
   public:
    GameState();

    Position position;
    bool white_to_move;

    bool white_king_moved;
    bool white_rook_1_moved;
    bool white_rook_2_moved;
    bool white_king_in_check;
    bool black_king_moved;
    bool black_rook_1_moved;
    bool black_rook_2_moved;
    bool black_king_in_check;

    int32_t pawn_ep;
};