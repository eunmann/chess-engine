#pragma once

#include <array>

#include "Definitions.hpp"

class Move {
   public:
    Move();
    Move(const int source, const int dest);
    Move(const int& move);

    auto get_source_square() const -> Square;
    auto get_destination_square() const -> Square;

    auto get_source_bit_board() const -> BitBoard;
    auto get_destination_bit_board() const -> BitBoard;

    bool operator==(const Move& move) {
        return this->m_move == move.m_move;
    }

   private:
    // Bits [0-6] Source
    // Bits [7-13] Dest
    int32_t m_move = 0;
    static constexpr int32_t MASK = 0b0011'1111;
};

class Moves {
   public:
    Moves();

    auto push_back(const Move move) -> void;
    auto size() -> std::size_t;
    auto operator[](int32_t index) -> Move&;
    auto begin() -> Move*;
    auto end() -> Move*;

   private:
    std::array<Move, MAX_NUM_MOVES> m_moves_array;
    std::size_t m_index;
};