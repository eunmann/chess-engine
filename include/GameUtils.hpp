#pragma once

#include <functional>
#include <string>
#include <vector>

#include "Board.hpp"
#include "GameState.hpp"
#include "Move.hpp"
#include "Position.hpp"

namespace GameUtils {

  auto print_position(const BitBoard bit_board) noexcept -> void;

  auto shift_bit_board(const BitBoard bit_board, const int32_t vertical, const int32_t horizontal) noexcept->BitBoard;

  auto get_row(const BitBoard bit_board) noexcept ->int32_t;
  auto get_col(const BitBoard bit_board) noexcept ->int32_t;
  auto get_row_col(const BitBoard bit_board, int32_t& row, int32_t& col) noexcept -> void;
  auto get_tile_name(const BitBoard bit_board) noexcept ->std::string;

  // Conversions
  auto square_to_bit_board(const Square square) noexcept ->BitBoard;
  auto bit_board_to_square(const BitBoard bit_board) noexcept ->Square;

  // Checks
  auto do_bit_boards_overlap(const BitBoard bit_board_1, const BitBoard bit_board_2) noexcept -> bool;
  auto is_piece_in_row(const BitBoard bit_board, const int32_t row) noexcept -> bool;
  auto is_piece_in_col(const BitBoard bit_board, const int32_t col) noexcept -> bool;
  auto is_piece_in_top_row(const BitBoard bit_board) noexcept -> bool;
  auto is_piece_in_top_2_row(const BitBoard bit_board) noexcept -> bool;
  auto is_piece_in_bottom_row(const BitBoard bit_board) noexcept -> bool;
  auto is_piece_in_bottom_2_row(const BitBoard bit_board) noexcept -> bool;
  auto is_piece_in_left_col(const BitBoard bit_board) noexcept -> bool;
  auto is_piece_in_left_2_col(const BitBoard bit_board) noexcept -> bool;
  auto is_piece_in_right_col(const BitBoard bit_board) noexcept -> bool;
  auto is_piece_in_right_2_col(const BitBoard bit_board) noexcept -> bool;

  // GameState Modifiers
  auto perform_user_move(GameState& game_state) noexcept -> bool;
  auto process_user_move(GameState& game_state, const std::string& move_str) noexcept -> bool;

  // Input
  auto get_user_input() noexcept ->std::string;
  auto square_name_to_square(const std::string& square_name) noexcept ->Square;
  auto move_str_to_move(const std::string& move_str) noexcept ->Move;

  // Bit Utils
  auto for_each_set_square(const BitBoard bit_board, const auto& func) noexcept -> void {
    BitBoard temp_bit_board = bit_board;
    while (true) {
      const Square square = GameUtils::bit_board_to_square(temp_bit_board);
      if (square == -1) {
        break;
      }
      func(square);
      temp_bit_board &= ~GameUtils::square_to_bit_board(square);
    };
  }

  auto for_each_bit_board(const BitBoard bit_board, const auto& func) noexcept -> void {
    BitBoard temp_bit_board = bit_board;
    while (true) {
      const Square square = GameUtils::bit_board_to_square(temp_bit_board);
      if (square == -1) {
        break;
      }
      const BitBoard single_bit_board = GameUtils::square_to_bit_board(square);
      func(single_bit_board);
      temp_bit_board &= ~single_bit_board;
    };
  }

  template <const int V, const int H>
  auto shift_bit_board(const BitBoard bit_board) noexcept -> BitBoard {
    constexpr int shift = V * 8 + H;
    if constexpr (shift >= 0) {
      return bit_board << shift;
    } else {
      return bit_board >> shift * -1;
    }
  }
}  // namespace GameUtils