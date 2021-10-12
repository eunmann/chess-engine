#pragma once

#include "Definitions.hpp"
#include "Move.hpp"
#include "Position.hpp"

namespace MoveGeneration {

template <Color color>
void pawns(const Position& position, Moves& moves) {
    BitBoard opp_position;
    BitBoard my_position;
    BitBoard start_rank;
    int32_t direction;

    if (color == Colors::WHITE) {
        opp_position = position.get_bit_board(Colors::BLACK);
        my_position = position.get_bit_board(Colors::WHITE);
        start_rank = ROW_2_BB;
        direction = 1;
    } else {
        opp_position = position.get_bit_board(Colors::WHITE);
        my_position = position.get_bit_board(Colors::BLACK);
        start_rank = ROW_7_BB;
        direction = -1;
    }

    BitBoard empty = position.get_empty_bit_board();

    // Moves Forward
    BitBoard pawns = position.get_bit_board(PieceCodes::PAWN, color);

    BitBoard pawns_forward_two = move<2 * direction, 0>(pawns & start_rank) & empty;
    Bitboard pawns_forward_one = move<1 * direction, 0>(pawns) & empty;

    // Captures
    BitBoard pawns_capture_left = move<1, -1>(pawns) & opp_position;
    BitBoard pawns_capture_right = move<1, 1>(pawns) & opp_position;

    // Promotions
}

template <Color color, PieceCode piece_code>
void piece(const Position& position, Moves& moves) {
}
}  // namespace MoveGeneration