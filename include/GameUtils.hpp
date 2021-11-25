#pragma once

#include <functional>
#include <string>
#include <vector>

#include "Board.hpp"
#include "GameState.hpp"
#include "Move.hpp"
#include "Position.hpp"

namespace GameUtils {

auto print_position(const BitBoard bit_board) -> void;

auto shift_bit_board(const BitBoard bit_board, const int32_t vertical,
                     const int32_t horizontal) -> BitBoard;

auto get_row(const BitBoard bit_board) -> int32_t;
auto get_col(const BitBoard bit_board) -> int32_t;
auto get_row_col(const BitBoard bit_board, int32_t& row, int32_t& col) -> void;
auto get_tile_name(const BitBoard bit_board) -> std::string;

// Conversions
auto square_to_bit_board(const Square square) -> BitBoard;
auto bit_board_to_square(const BitBoard bit_board) -> Square;

// Checks
auto do_bit_boards_overlap(const BitBoard bit_board_1,
                           const BitBoard bit_board_2) -> bool;
auto is_piece_in_row(const BitBoard bit_board, const int32_t row) -> bool;
auto is_piece_in_col(const BitBoard bit_board, const int32_t col) -> bool;
auto is_piece_in_top_row(const BitBoard bit_board) -> bool;
auto is_piece_in_top_2_row(const BitBoard bit_board) -> bool;
auto is_piece_in_bottom_row(const BitBoard bit_board) -> bool;
auto is_piece_in_bottom_2_row(const BitBoard bit_board) -> bool;
auto is_piece_in_left_col(const BitBoard bit_board) -> bool;
auto is_piece_in_left_2_col(const BitBoard bit_board) -> bool;
auto is_piece_in_right_col(const BitBoard bit_board) -> bool;
auto is_piece_in_right_2_col(const BitBoard bit_board) -> bool;

// GameState Modifiers
auto perform_user_move(GameState& game_state) -> int32_t;
auto process_user_move(GameState& game_state, const std::string& move_str)
    -> int32_t;

// Input
auto get_user_input() -> std::string;
auto square_name_to_square(const std::string& square_name) -> Square;
auto move_str_to_move(const std::string& move_str) -> Move;

// Bit Utils
auto for_each_set_square(const BitBoard bit_board,
                         const std::function<void(int32_t bit_index)>& func)
    -> void;
auto for_each_bit_board(const BitBoard bit_board,
                        const std::function<void(BitBoard bit_board)>& func)
    -> void;

// Template functions
template <const int V, const int H>
constexpr auto shift_bit_board(const BitBoard bit_board) -> BitBoard {
  constexpr int shift = V * 8 + H;
  if constexpr (shift >= 0) {
    return bit_board << shift;
  } else {
    return bit_board >> shift * -1;
  }
}

}  // namespace GameUtils