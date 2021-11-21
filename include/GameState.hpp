#pragma once

#include <inttypes.h>

#include "Move.hpp"
#include "Position.hpp"

class GameState {
   public:
    GameState();

    auto apply_move(const Move move) -> void;

    Position position;
    // TODO(EMU): Mask this a bit mask instead
    bool white_to_move;
    bool white_king_moved;
    bool white_rook_A_moved;
    bool white_rook_H_moved;
    bool white_king_in_check;
    bool black_king_moved;
    bool black_rook_A_moved;
    bool black_rook_H_moved;
    bool black_king_in_check;
    bool is_legal;

    int32_t pawn_ep;
};