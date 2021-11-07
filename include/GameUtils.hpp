#pragma once

#include <functional>
#include <string>
#include <vector>

#include "Board.hpp"
#include "GameState.hpp"
#include "Move.hpp"
#include "Position.hpp"

namespace GameUtils {

auto print_position(BitBoard bit_board) -> void;

auto shift_bit_board(BitBoard bit_board, int32_t vertical, int32_t horizontal) -> BitBoard;

auto get_row_col(BitBoard bit_board, int32_t &row, int32_t &col) -> void;
auto get_tile_name(BitBoard bit_board) -> std::string;
auto init_standard(GameState &game_state) -> void;

// Conversions
auto square_to_bit_board(const Square square) -> BitBoard;
auto position_to_board(const Position &position) -> Board;
auto bit_board_to_square(const BitBoard bit_board) -> Square;

// Checks
auto is_piece_in_row(BitBoard bit_board, int32_t row) -> bool;
auto is_piece_in_col(BitBoard bit_board, int32_t col) -> bool;
auto is_piece_in_top_row(BitBoard bit_board) -> bool;
auto is_piece_in_top_2_row(BitBoard bit_board) -> bool;
auto is_piece_in_bottom_row(BitBoard bit_board) -> bool;
auto is_piece_in_bottom_2_row(BitBoard bit_board) -> bool;
auto is_piece_in_left_col(BitBoard bit_board) -> bool;
auto is_piece_in_left_2_col(BitBoard bit_board) -> bool;
auto is_piece_in_right_col(BitBoard bit_board) -> bool;
auto is_piece_in_right_2_col(BitBoard bit_board) -> bool;

// GameState Modifiers
auto perform_user_move(GameState &game_state) -> int32_t;
auto process_user_move(GameState &game_state, const Move move) -> int32_t;

// Input
auto get_user_input() -> std::string;
auto square_name_to_square(const std::string &square_name) -> Square;
auto move_str_to_move(const std::string &move_str) -> Move;

// Output
auto get_best_move(const GameState &game_state) -> Move;

// Bit Utils
auto for_each_set_bit(BitBoard bit_board, std::function<void(int32_t bit_index)> func) -> void;
auto for_each_bit_board(BitBoard bit_board, std::function<void(BitBoard bit_board)> func) -> void;

// Template functions
template <const int V, const int H>
constexpr auto shift_bit_board(BitBoard bit_board) -> BitBoard {
    constexpr int shift = V * 8 + H;
    if constexpr (shift >= 0) {
        return bit_board << shift;
    } else {
        return bit_board >> shift * -1;
    }
}

}  // namespace GameUtils