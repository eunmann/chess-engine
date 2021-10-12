#pragma once

#include "Definitions.hpp"

typedef uint64_t BitBoard;

namespace BitBoards {

// Entire Boards
const BitBoard EMPTY = 0ULL;
const BitBoard ALL_SQUARES = ~EMPTY;

// Rows
const BitBoard ROW_1 = 0xFFULL;
const BitBoard ROW_2 = ROW_1 << 8;
const BitBoard ROW_3 = ROW_2 << 8;
const BitBoard ROW_4 = ROW_3 << 8;
const BitBoard ROW_5 = ROW_4 << 8;
const BitBoard ROW_6 = ROW_5 << 8;
const BitBoard ROW_7 = ROW_6 << 8;
const BitBoard ROW_8 = ROW_7 << 8;

// Cols
const BitBoard COL_A = 0x0101010101010101ULL;
const BitBoard COL_B = COL_A << 1;
const BitBoard COL_C = COL_B << 1;
const BitBoard COL_D = COL_C << 1;
const BitBoard COL_E = COL_D << 1;
const BitBoard COL_F = COL_E << 1;
const BitBoard COL_G = COL_F << 1;
const BitBoard COL_H = COL_G << 1;

BitBoard PSEDUO_MOVES[PieceCodes::NUM][Squares::NUM];
}  // namespace BitBoards

// This is for BitBoards with a single piece in it
template <int32_t vertical, int32_t horizontal>
BitBoard move(BitBoard position) {
    int32_t shift = vertical * 8 + horizontal;
    if (horizontal > 0 && (position & (BitBoards::COL_H >> (horizontal - 1)))) {
        return 0;
    } else if (horizontal < 0 && (position & (BitBoards::COL_A << (-1 * horizontal - 1)))) {
        return 0;
    } else if (shift > 0) {
        return position << shift;
    } else {
        return position >> -1 * shift;
    }
}