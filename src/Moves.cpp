#include "Moves.hpp"

#include <assert.h>

Moves::Moves() noexcept : m_moves_array(), m_index(0) {
}

auto Moves::size() noexcept -> std::size_t {
  return this->m_index;
}

auto Moves::push_back(const Move move) noexcept -> void {
  assert(this->m_index < this->m_moves_array.size());
  this->m_moves_array[this->m_index++] = move;
}

auto Moves::begin()noexcept ->std::array<Move, MAX_NUM_MOVES>::iterator{
  return this->m_moves_array.begin();
}

auto Moves::end()noexcept ->std::array<Move, MAX_NUM_MOVES>::iterator{
  return this->m_moves_array.begin() + this->m_index;
}