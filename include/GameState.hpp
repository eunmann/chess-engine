#pragma once

#include <inttypes.h>

#include "Move.hpp"
#include "Position.hpp"

class GameState {
   public:
    GameState();

    auto apply_move(const Move move) -> void;
    auto is_move_legal(const Move move) const -> bool;

    Position position;
    // TODO(EMU): Mask this a bit mask instead
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