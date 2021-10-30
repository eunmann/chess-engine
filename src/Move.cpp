#include "Move.hpp"

#include <assert.h>

#include "GameUtils.hpp"

Move::Move() : m_move(0) {}

Move::Move(const int source, const int dest) : m_move((source | Move::MASK) | (dest | (Move::MASK << 6))) {}

Move::Move(const int& move) : m_move(move) {}

auto Move::get_source_square() const -> Square {
    return static_cast<Square>(this->m_move & Move::MASK);
}

auto Move::get_destination_square() const -> Square {
    return static_cast<Square>((this->m_move >> 6) & Move::MASK);
}

auto Move::get_source_bit_board() const -> BitBoard {
    return GameUtils::square_to_bit_board(this->get_source_square());
}

auto Move::get_destination_bit_board() const -> BitBoard {
    GameUtils::square_to_bit_board(this->get_destination_square());
    return 0x1UL << (this->get_destination_square() - 1);
}

Moves::Moves() : m_moves_array(),
                 m_index(0) {
}

auto Moves::size() -> std::size_t {
    return this->m_index;
}

auto Moves::push_back(const Move move) -> void {
    assert(this->m_index < this->m_moves_array.size());
    this->m_moves_array[this->m_index++] = move;
}

Move& Moves::operator[](int32_t index) {
    assert(index >= 0 && index < this->m_moves_array.size());
    return this->m_moves_array[index];
}

auto Moves::begin() -> Move* {
    return this->m_moves_array.begin();
}

auto Moves::end() -> Move* {
    return this->m_moves_array.begin() + this->m_index;
}