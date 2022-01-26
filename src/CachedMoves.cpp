#include "CachedMoves.hpp"

#include <ranges>
#include "BitBoardUtils.hpp"
#include "CapturesGeneration.hpp"
#include "Timer.hpp"

namespace CachedMoves {

  const std::array<BitBoard, Squares::NUM> KNIGHT = generate_knight_moves();
  const std::array<BitBoard, Squares::NUM> KING = generate_king_moves();
  const std::array<BitBoard, Squares::NUM* Colors::NUM> PAWN_CAPTURES = generate_pawn_captures();
  const std::array<BitBoard, Squares::NUM* Colors::NUM> PAWN = generate_pawn_moves();

  auto get_knight_moves(const Square square) noexcept -> BitBoard {
    return CachedMoves::KNIGHT[square];
  }

  auto get_king_moves(const Square square) noexcept -> BitBoard {
    return CachedMoves::KING[square];
  }
}