#pragma once

#include "Definitions.hpp"
#include "Position.hpp"
#include "MagicBitBoards.hpp"
#include "CachedMoves.hpp"

namespace CachedCapturesGeneration {

  template<const Color color>
  auto get_cached_pawn_capture_positions(const BitBoard bit_board) noexcept -> BitBoard {
    BitBoard capturable_bit_board = BitBoards::EMPTY;
    BitBoardUtils::for_each_set_square(bit_board, [&capturable_bit_board](const Square square) {
      capturable_bit_board |= CachedMoves::get_pawn_capture_moves<color>(square);
      });
    return capturable_bit_board;
  }

  auto get_cached_knight_capture_positions(const BitBoard knights_bit_board) noexcept -> BitBoard;

  template<const Color color>
  auto get_cached_bishop_capture_positions(const Position& position) noexcept -> BitBoard {
    BitBoard capturable_bit_board = BitBoards::EMPTY;
    BitBoard bishops_bit_board = position.get_piece_color_bit_board(PieceCodes::BISHOP, color);
    BitBoardUtils::for_each_set_square(bishops_bit_board, [&capturable_bit_board, &position](const Square square) {
      capturable_bit_board |= MagicBitBoards::get_bishop_moves(square, position.get_occupied_bit_board());
      });
    return capturable_bit_board;
  }

  template<const Color color>
  auto get_cached_rook_capture_positions(const Position& position) noexcept -> BitBoard {
    BitBoard capturable_bit_board = BitBoards::EMPTY;
    BitBoard rook_bit_board = position.get_piece_color_bit_board(PieceCodes::ROOK, color);
    BitBoardUtils::for_each_set_square(rook_bit_board, [&capturable_bit_board, &position](const Square square) {
      capturable_bit_board |= MagicBitBoards::get_rook_moves(square, position.get_occupied_bit_board());
      });
    return capturable_bit_board;
  }

  template<const Color color>
  auto get_cached_queen_capture_positions(const Position& position) noexcept -> BitBoard {
    BitBoard capturable_bit_board = BitBoards::EMPTY;
    BitBoard queen_bit_board = position.get_piece_color_bit_board(PieceCodes::ROOK, color);
    BitBoardUtils::for_each_set_square(queen_bit_board, [&capturable_bit_board, &position](const Square square) {
      capturable_bit_board |= MagicBitBoards::get_bishop_moves(square, position.get_occupied_bit_board());
      capturable_bit_board |= MagicBitBoards::get_rook_moves(square, position.get_occupied_bit_board());
      });
    return capturable_bit_board;
  }

  auto get_cached_king_capture_positions(const BitBoard kings_bit_board) noexcept -> BitBoard;

  template <const Color color>
  auto get_capture_positions(const Position& position) noexcept -> BitBoard {
    BitBoard capturable_bit_board = BitBoards::EMPTY;
    capturable_bit_board |= CachedCapturesGeneration::get_cached_pawn_capture_positions<color>(position.get_piece_color_bit_board<PieceCodes::PAWN, color>());
    capturable_bit_board |= CachedCapturesGeneration::get_cached_knight_capture_positions(position.get_piece_color_bit_board<PieceCodes::KNIGHT, color>());
    capturable_bit_board |= CachedCapturesGeneration::get_cached_bishop_capture_positions<color>(position);
    capturable_bit_board |= CachedCapturesGeneration::get_cached_rook_capture_positions<color>(position);
    capturable_bit_board |= CachedCapturesGeneration::get_cached_queen_capture_positions<color>(position);
    capturable_bit_board |= CachedCapturesGeneration::get_cached_king_capture_positions(position.get_piece_color_bit_board<PieceCodes::KING, color>());
    return capturable_bit_board;
  }
}