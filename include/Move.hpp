#pragma once

#include <array>

#include "Definitions.hpp"

// Bits [0-6] Source
// Bits [7-13] Dest
typedef int32_t Move;

class Moves {
   public:
    Moves();

    auto back_ref() -> Move&;
    auto size() -> std::size_t;
    auto operator[](int32_t index) -> Move&;
    auto add_moves(BitBoard bit_board) -> void;

   private:
    std::array<Move, MAX_NUM_MOVES> m_moves_array;
    std::size_t m_index;
};