#pragma once

#include <string>
#include <vector>

#include "Board.hpp"
#include "GameState.hpp"
#include "Move.hpp"
#include "Position.hpp"

namespace GameUtils {

auto print_position(BitBoard position) -> void;
auto move(BitBoard position, int32_t vertical, int32_t horizontal) -> BitBoard;
auto get_row_col(BitBoard position, int32_t &row, int32_t &col) -> void;
auto get_tile_name(BitBoard position) -> std::string;
auto init_standard(GameState &game_state) -> void;

// Conversions
auto square_to_bit_board(const Square square) -> BitBoard;
auto position_to_board(const Position &position) -> Board;

// Checks
auto is_empty(const Position &bit_board, BitBoard position) -> bool;
auto is_valid(const GameState &game_state, BitBoard position) -> bool;
auto is_piece_in_row(BitBoard position, int32_t row) -> bool;
auto is_piece_in_col(BitBoard position, int32_t col) -> bool;
auto is_piece_in_top_row(BitBoard position) -> bool;
auto is_piece_in_top_2_row(BitBoard position) -> bool;
auto is_piece_in_bottom_row(BitBoard position) -> bool;
auto is_piece_in_bottom_2_row(BitBoard position) -> bool;
auto is_piece_in_left_col(BitBoard position) -> bool;
auto is_piece_in_left_2_col(BitBoard position) -> bool;
auto is_piece_in_right_col(BitBoard position) -> bool;
auto is_piece_in_right_2_col(BitBoard position) -> bool;

// GameState Modifiers
auto perform_user_move(GameState &game_state) -> int32_t;
auto apply_move(GameState &game_state, Move move) -> void;
}  // namespace GameUtils