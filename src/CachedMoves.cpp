#include "CachedMoves.hpp"

#include <ranges>
#include "BitBoardUtils.hpp"
#include "CapturesGeneration.hpp"
#include "Timer.hpp"

auto CachedMoves::get_knight_moves(const Square square) noexcept -> BitBoard {
  return CachedMoves::KNIGHT[square];
}

auto CachedMoves::get_king_moves(const Square square) noexcept -> BitBoard {
  return CachedMoves::KING[square];
}