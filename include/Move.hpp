#pragma once

#include <array>

#include "Definitions.hpp"

// Bits [0-6] Source
// Bits [7-13] Dest
typedef int32_t Move;

inline Square get_source(Move move) {
    return static_cast<Square>(move & 0x3F);
}

inline Square get_dest(Move move) {
    return static_cast<Square>((move >> 6) & 0x3F);
}

class Moves {
   public:
    Moves();

    Move& back_ref();
    int32_t size();
    Move& operator[](int32_t index);
    void add_moves(BitBoard bit_board);

   private:
    std::array<Move, MAX_NUM_MOVES> m_moves_array;
    std::size_t m_index;
};