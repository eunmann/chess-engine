#pragma once

#include "Definitions.hpp"
#include "BitBoardUtils.hpp"

namespace CapturesGeneration {

  // Threaten Squares Templates
  template <const int V, const int H>
  constexpr auto get_captures_in_direction(BitBoard bit_board, const BitBoard occupied_bit_board) noexcept -> BitBoard {
    BitBoard capturable_bit_board = BitBoards::EMPTY;

    for (int i = 0; i < 8; ++i) {
      if constexpr (V > 0) {
        if (BitBoardUtils::is_piece_in_top_row(bit_board)) {
          break;
        }
      } else if constexpr (V < 0) {
        if (BitBoardUtils::is_piece_in_bottom_row(bit_board)) {
          break;
        }
      }

      if constexpr (H > 0) {
        if (BitBoardUtils::is_piece_in_right_col(bit_board)) {
          break;
        }
      } else if constexpr (H < 0) {
        if (BitBoardUtils::is_piece_in_left_col(bit_board)) {
          break;
        }
      }

      bit_board = BitBoardUtils::shift_bit_board<V, H>(bit_board);
      capturable_bit_board |= bit_board;

      if (BitBoardUtils::do_bit_boards_overlap(occupied_bit_board, bit_board)) {
        break;
      }
    }

    return capturable_bit_board;
  }

  template <const Color color>
  constexpr auto get_pawn_capture_positions(const BitBoard pawn_bit_board) noexcept -> BitBoard {
    constexpr int64_t pawn_dir = color == Colors::WHITE ? 1 : -1;
    BitBoard capturable_bit_board = BitBoards::EMPTY;
    const BitBoard pawn_bit_board_left_capture = !BitBoardUtils::is_piece_in_left_col(pawn_bit_board) * BitBoardUtils::shift_bit_board<1 * pawn_dir, -1>(pawn_bit_board);
    const BitBoard pawn_bit_board_right_capture = !BitBoardUtils::is_piece_in_right_col(pawn_bit_board) * BitBoardUtils::shift_bit_board<1 * pawn_dir, 1>(pawn_bit_board);
    capturable_bit_board |= pawn_bit_board_left_capture | pawn_bit_board_right_capture;
    return capturable_bit_board;
  }

  template <const Color color>
  constexpr auto get_pawn_move_positions(const BitBoard pawn_bit_board) noexcept -> BitBoard {
    constexpr int64_t pawn_dir = color == Colors::WHITE ? 1 : -1;
    constexpr int32_t starting_row = color == Colors::WHITE ? 1 : 6;
    constexpr Color opponent_color = color == Colors::WHITE ? Colors::BLACK : Colors::WHITE;
    BitBoard moves_bit_board = BitBoards::EMPTY;

    moves_bit_board |= BitBoardUtils::shift_bit_board<pawn_dir, 0>(pawn_bit_board);
    moves_bit_board |= BitBoardUtils::is_piece_in_row(pawn_bit_board, starting_row) * BitBoardUtils::shift_bit_board<2 * pawn_dir, 0>(pawn_bit_board);

    return moves_bit_board;
  }

  constexpr auto get_knight_capture_positions(const BitBoard knight_bit_board) noexcept -> BitBoard {
    BitBoard capturable_bit_board = BitBoards::EMPTY;
    {
      constexpr BitBoard negate_top_2_left_1 = ~(BitBoards::ROW_8 | BitBoards::ROW_7 | BitBoards::COL_A);
      const BitBoard next_knight_bit_board = BitBoardUtils::do_bit_boards_overlap(knight_bit_board, negate_top_2_left_1) * BitBoardUtils::shift_bit_board<2, -1>(knight_bit_board);
      capturable_bit_board |= next_knight_bit_board;
    }

    {
      constexpr BitBoard negate_top_2_right_1 = ~(BitBoards::ROW_8 | BitBoards::ROW_7 | BitBoards::COL_H);
      const BitBoard next_knight_bit_board = BitBoardUtils::do_bit_boards_overlap(knight_bit_board, negate_top_2_right_1) * BitBoardUtils::shift_bit_board<2, 1>(knight_bit_board);
      capturable_bit_board |= next_knight_bit_board;
    }

    {
      constexpr BitBoard negate_top_1_left_2 = ~(BitBoards::ROW_8 | BitBoards::COL_A | BitBoards::COL_B);
      const BitBoard next_knight_bit_board = BitBoardUtils::do_bit_boards_overlap(knight_bit_board, negate_top_1_left_2) * BitBoardUtils::shift_bit_board<1, -2>(knight_bit_board);
      capturable_bit_board |= next_knight_bit_board;
    }

    {
      constexpr BitBoard negate_top_1_right_2 = ~(BitBoards::ROW_8 | BitBoards::COL_G | BitBoards::COL_H);
      const BitBoard next_knight_bit_board = BitBoardUtils::do_bit_boards_overlap(knight_bit_board, negate_top_1_right_2) * BitBoardUtils::shift_bit_board<1, 2>(knight_bit_board);
      capturable_bit_board |= next_knight_bit_board;
    }

    {
      constexpr BitBoard negate_bottom_1_left_2 = ~(BitBoards::ROW_1 | BitBoards::COL_A | BitBoards::COL_B);
      const BitBoard next_knight_bit_board = BitBoardUtils::do_bit_boards_overlap(knight_bit_board, negate_bottom_1_left_2) * BitBoardUtils::shift_bit_board<-1, -2>(knight_bit_board);
      capturable_bit_board |= next_knight_bit_board;
    }

    {
      constexpr BitBoard negate_bottom_1_right_2 = ~(BitBoards::ROW_1 | BitBoards::COL_G | BitBoards::COL_H);
      const BitBoard next_knight_bit_board = BitBoardUtils::do_bit_boards_overlap(knight_bit_board, negate_bottom_1_right_2) * BitBoardUtils::shift_bit_board<-1, 2>(knight_bit_board);
      capturable_bit_board |= next_knight_bit_board;
    }

    {
      constexpr BitBoard negate_bottom_2_left_1 = ~(BitBoards::ROW_1 | BitBoards::ROW_2 | BitBoards::COL_A);
      BitBoard next_knight_bit_board = BitBoardUtils::do_bit_boards_overlap(knight_bit_board, negate_bottom_2_left_1) * BitBoardUtils::shift_bit_board<-2, -1>(knight_bit_board);
      capturable_bit_board |= next_knight_bit_board;
    }

    {
      constexpr BitBoard negate_bottom_2_right_1 = ~(BitBoards::ROW_1 | BitBoards::ROW_2 | BitBoards::COL_H);
      const BitBoard next_knight_bit_board = BitBoardUtils::do_bit_boards_overlap(knight_bit_board, negate_bottom_2_right_1) * BitBoardUtils::shift_bit_board<-2, 1>(knight_bit_board);
      capturable_bit_board |= next_knight_bit_board;
    }

    return capturable_bit_board;
  }

  constexpr auto get_bishop_capture_positions(const BitBoard bishop_bit_board, const BitBoard occupied_bit_board) noexcept -> BitBoard {
    BitBoard capturable_bit_board = BitBoards::EMPTY;
    capturable_bit_board |= CapturesGeneration::get_captures_in_direction<1, 1>(bishop_bit_board, occupied_bit_board);
    capturable_bit_board |= CapturesGeneration::get_captures_in_direction<-1, 1>(bishop_bit_board, occupied_bit_board);
    capturable_bit_board |= CapturesGeneration::get_captures_in_direction<1, -1>(bishop_bit_board, occupied_bit_board);
    capturable_bit_board |= CapturesGeneration::get_captures_in_direction<-1, -1>(bishop_bit_board, occupied_bit_board);
    return capturable_bit_board;
  }

  constexpr auto get_rook_capture_positions(const BitBoard rook_bit_board, const BitBoard occupied_bit_board) noexcept  -> BitBoard {
    BitBoard capturable_bit_board = BitBoards::EMPTY;
    capturable_bit_board |= CapturesGeneration::get_captures_in_direction<1, 0>(rook_bit_board, occupied_bit_board);
    capturable_bit_board |= CapturesGeneration::get_captures_in_direction<-1, 0>(rook_bit_board, occupied_bit_board);
    capturable_bit_board |= CapturesGeneration::get_captures_in_direction<0, -1>(rook_bit_board, occupied_bit_board);
    capturable_bit_board |= CapturesGeneration::get_captures_in_direction<0, 1>(rook_bit_board, occupied_bit_board);
    return capturable_bit_board;
  }

  constexpr auto get_queen_capture_positions(const BitBoard queen_bit_board, const BitBoard occupied_bit_board) noexcept  -> BitBoard {
    BitBoard capturable_bit_board = BitBoards::EMPTY;
    // Diagonal
    capturable_bit_board |= CapturesGeneration::get_captures_in_direction<1, 1>(queen_bit_board, occupied_bit_board);
    capturable_bit_board |= CapturesGeneration::get_captures_in_direction<-1, 1>(queen_bit_board, occupied_bit_board);
    capturable_bit_board |= CapturesGeneration::get_captures_in_direction<1, -1>(queen_bit_board, occupied_bit_board);
    capturable_bit_board |= CapturesGeneration::get_captures_in_direction<-1, -1>(queen_bit_board, occupied_bit_board);

    // Vertical/Horizontal
    capturable_bit_board |= CapturesGeneration::get_captures_in_direction<1, 0>(queen_bit_board, occupied_bit_board);
    capturable_bit_board |= CapturesGeneration::get_captures_in_direction<-1, 0>(queen_bit_board, occupied_bit_board);
    capturable_bit_board |= CapturesGeneration::get_captures_in_direction<0, -1>(queen_bit_board, occupied_bit_board);
    capturable_bit_board |= CapturesGeneration::get_captures_in_direction<0, 1>(queen_bit_board, occupied_bit_board);

    return capturable_bit_board;
  }

  constexpr auto get_king_capture_positions(const BitBoard king_bit_board) noexcept -> BitBoard {
    BitBoard capturable_bit_board = BitBoards::EMPTY;

    // Up
    {
      const BitBoard next_king_position = BitBoardUtils::do_bit_boards_overlap(king_bit_board, ~BitBoards::ROW_8) * BitBoardUtils::shift_bit_board<1, 0>(king_bit_board);
      capturable_bit_board |= next_king_position;
    }

    // Down
    {
      const BitBoard next_king_position = BitBoardUtils::do_bit_boards_overlap(king_bit_board, ~BitBoards::ROW_1) * BitBoardUtils::shift_bit_board<-1, 0>(king_bit_board);
      capturable_bit_board |= next_king_position;
    }

    // Left
    {
      const BitBoard next_king_position = BitBoardUtils::do_bit_boards_overlap(king_bit_board, ~BitBoards::COL_A) * BitBoardUtils::shift_bit_board<0, -1>(king_bit_board);
      capturable_bit_board |= next_king_position;
    }

    // Right
    {
      const BitBoard next_king_position = BitBoardUtils::do_bit_boards_overlap(king_bit_board, ~BitBoards::COL_H) * BitBoardUtils::shift_bit_board<0, 1>(king_bit_board);
      capturable_bit_board |= next_king_position;
    }

    // Up-Right
    {
      constexpr BitBoard top_1_right_1 = BitBoards::ROW_8 | BitBoards::COL_H;
      const BitBoard next_king_position = BitBoardUtils::do_bit_boards_overlap(king_bit_board, ~top_1_right_1) * BitBoardUtils::shift_bit_board<1, 1>(king_bit_board);
      capturable_bit_board |= next_king_position;
    }

    // Up-Left
    {
      constexpr BitBoard top_1_left_1 = BitBoards::ROW_8 | BitBoards::COL_A;
      const BitBoard next_king_position = BitBoardUtils::do_bit_boards_overlap(king_bit_board, ~top_1_left_1) * BitBoardUtils::shift_bit_board<1, -1>(king_bit_board);
      capturable_bit_board |= next_king_position;
    }

    // Down-Right
    {
      constexpr BitBoard bottom_1_right_1 = BitBoards::ROW_1 | BitBoards::COL_H;
      const BitBoard next_king_position = BitBoardUtils::do_bit_boards_overlap(king_bit_board, ~bottom_1_right_1) * BitBoardUtils::shift_bit_board<-1, 1>(king_bit_board);
      capturable_bit_board |= next_king_position;
    }

    // Down-Left
    {
      constexpr BitBoard bottom_1_left_1 = BitBoards::ROW_1 | BitBoards::COL_A;
      BitBoard next_king_position = BitBoardUtils::do_bit_boards_overlap(king_bit_board, ~bottom_1_left_1) * BitBoardUtils::shift_bit_board<-1, -1>(king_bit_board);
      capturable_bit_board |= next_king_position;
    }

    return capturable_bit_board;
  }
}