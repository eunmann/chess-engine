#include "Moves.hpp"

#include <assert.h>

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

Move& Moves::operator[](std::size_t index) {
    assert(index < this->m_moves_array.size());
    return this->m_moves_array[index];
}

auto Moves::begin() -> Move* {
    return this->m_moves_array.begin();
}

auto Moves::end() -> Move* {
    return this->m_moves_array.begin() + this->m_index;
}