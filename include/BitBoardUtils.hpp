#pragma once

#include "Definitions.hpp"
#include <string>

namespace BitBoardUtils {
  auto print_position(const BitBoard bit_board) noexcept -> void;

  auto shift_bit_board(const BitBoard bit_board, const int32_t vertical, const int32_t horizontal) noexcept->BitBoard;

  auto get_row(const BitBoard bit_board) noexcept ->int32_t;
  auto get_col(const BitBoard bit_board) noexcept ->int32_t;
  auto get_row_col(const BitBoard bit_board, int32_t& row, int32_t& col) noexcept -> void;
  auto get_tile_name(const BitBoard bit_board) noexcept -> std::string;

  constexpr auto get_count(const BitBoard bit_board) noexcept -> int32_t {
    return std::popcount(bit_board);
  }

  // Conversions
  auto bit_board_to_square(const BitBoard bit_board) noexcept ->Square;

  constexpr auto square_to_bit_board(const Square square) noexcept ->BitBoard {
    return 0b1ULL << square;
  }

  // Checks
  constexpr auto do_bit_boards_overlap(const BitBoard bit_board_1, const BitBoard bit_board_2) noexcept -> bool {
    return (bit_board_1 & bit_board_2) != 0;
  }

  constexpr auto is_piece_in_row(const BitBoard bit_board, const int32_t row) noexcept -> bool {
    const BitBoard row_mask = BitBoards::ROW_1 << (row * 8);
    return do_bit_boards_overlap(row_mask, bit_board);
  }

  auto is_piece_in_col(const BitBoard bit_board, const int32_t col) noexcept -> bool;

  constexpr auto is_piece_in_top_row(const BitBoard bit_board) noexcept -> bool {
    return do_bit_boards_overlap(BitBoards::ROW_8, bit_board);
  }


  constexpr auto is_piece_in_bottom_row(const BitBoard bit_board) noexcept -> bool {
    return do_bit_boards_overlap(BitBoards::ROW_1, bit_board);
  }

  constexpr auto is_piece_in_left_col(const BitBoard bit_board) noexcept -> bool {
    return do_bit_boards_overlap(BitBoards::COL_A, bit_board);
  }

  constexpr auto is_piece_in_right_col(const BitBoard bit_board) noexcept -> bool {
    return do_bit_boards_overlap(BitBoards::COL_H, bit_board);
  }

  auto for_each_set_square(const BitBoard bit_board, const auto& func) noexcept -> void {
    BitBoard temp_bit_board = bit_board;
    while (true) {
      const Square square = bit_board_to_square(temp_bit_board);
      if (square == -1) {
        break;
      }
      func(square);
      temp_bit_board &= ~square_to_bit_board(square);
    };
  }

  constexpr auto for_each_bit_board(const BitBoard bit_board, const auto& func) noexcept -> void {
    BitBoard temp_bit_board = bit_board;
    while (true) {
      const Square square = BitBoardUtils::bit_board_to_square(temp_bit_board);
      if (square == -1) {
        break;
      }
      const BitBoard single_bit_board = BitBoardUtils::square_to_bit_board(square);
      func(single_bit_board);
      temp_bit_board &= ~single_bit_board;
    };
  }

  template <const int V, const int H>
  constexpr auto shift_bit_board(const BitBoard bit_board) noexcept -> BitBoard {
    constexpr int shift = V * 8 + H;
    if constexpr (shift >= 0) {
      return bit_board << shift;
    } else {
      return bit_board >> shift * -1;
    }
  }
}