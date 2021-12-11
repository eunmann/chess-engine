#include "CachedMoves.hpp"
#include <ranges>
#include "BitBoardUtils.hpp"
#include "MoveGeneration.hpp"

namespace CachedMoves {
  std::array<BitBoard, Squares::NUM> KNIGHT;
  std::array<BitBoard, Squares::NUM> KING;
  std::array<BitBoard, Squares::NUM> BISHOP;
  std::array<BitBoard, Squares::NUM> ROOK;

  auto init() noexcept -> void {
    for (auto square : std::views::iota(0, Squares::NUM)) {
      const BitBoard bit_board = BitBoardUtils::square_to_bit_board(square);
      CachedMoves::KNIGHT[square] = MoveGeneration::get_knight_capture_positions(bit_board);
      CachedMoves::KING[square] = MoveGeneration::get_king_capture_positions(bit_board);
    }
  }
}