#pragma once

#include <array>

#include "Definitions.hpp"

// Bits [0-6] Source
// Bits [7-13] Dest
typedef int32_t Move;

Square get_source(Move move);

Square get_dest(Move move);

class Moves {
   public:
    Moves();

    Move& back_ref();
    std::size_t size();
    Move& operator[](int32_t index);
    void add_moves(BitBoard bit_board);

   private:
    std::array<Move, MAX_NUM_MOVES> m_moves_array;
    std::size_t m_index;
};