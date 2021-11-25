#pragma once

#include "Move.hpp"

class Moves {
public:
  Moves();

  auto push_back(const Move move) -> void;
  auto size()->std::size_t;
  auto begin()->std::array<Move, MAX_NUM_MOVES>::iterator;
  auto end()->std::array<Move, MAX_NUM_MOVES>::iterator;

private:
  std::array<Move, MAX_NUM_MOVES> m_moves_array;
  std::size_t m_index;
};