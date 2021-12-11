#include "Moves.hpp"
#include "Assert.hpp"

Moves::Moves() noexcept : m_moves_array(), m_index(0) {
}

auto Moves::size() const noexcept -> std::size_t {
  return this->m_index;
}

auto Moves::push_back(const Move move) noexcept -> void {
  ASSERT(this->m_index < this->m_moves_array.size());
  this->m_moves_array[this->m_index++] = move;
}

auto Moves::begin() const noexcept -> std::array<Move, MAX_NUM_MOVES>::const_iterator{
  return this->m_moves_array.begin();
}

auto Moves::end() const noexcept -> std::array<Move, MAX_NUM_MOVES>::const_iterator{
  return this->m_moves_array.begin() + this->m_index;
}