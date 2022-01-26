#include "CachedCapturesGeneration.hpp"

#include "BitBoardUtils.hpp"

namespace CachedCapturesGeneration {

  auto get_cached_knight_capture_positions(const BitBoard knights_bit_board) noexcept ->BitBoard {
    BitBoard capturable_bit_board = BitBoards::EMPTY;
    BitBoardUtils::for_each_set_square(knights_bit_board, [&capturable_bit_board](auto knight_square) {
      capturable_bit_board |= CachedMoves::get_knight_moves(knight_square);
      });
    return capturable_bit_board;
  }

  auto get_cached_king_capture_positions(const BitBoard kings_bit_board) noexcept -> BitBoard {
    BitBoard capturable_bit_board = BitBoards::EMPTY;
    BitBoardUtils::for_each_set_square(kings_bit_board, [&capturable_bit_board](auto king_square) {
      capturable_bit_board |= CachedMoves::get_king_moves(king_square);
      });
    return capturable_bit_board;

  }
}