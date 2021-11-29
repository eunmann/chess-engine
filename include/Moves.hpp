#pragma once

#include "Move.hpp"

class Moves {
  public:
  Moves() noexcept;

  auto push_back(const Move move) noexcept -> void;
  auto size() noexcept -> std::size_t;
  auto begin() noexcept -> std::array<Move, MAX_NUM_MOVES>::iterator;
  auto end() noexcept -> std::array<Move, MAX_NUM_MOVES>::iterator;

  private:
  std::array<Move, MAX_NUM_MOVES> m_moves_array;
  std::size_t m_index;
};