#pragma once

#include <string>
#include <vector>

#include "Board.hpp"
#include "GameState.hpp"
#include "Move.hpp"
#include "Position.hpp"

namespace GameUtils {

auto print_position(BitBoard position) -> void;
auto to_board(const Position &bit_board) -> Board;
auto move(BitBoard position, int32_t vertical, int32_t horizontal) -> BitBoard;
auto is_empty(const Position &bit_board, BitBoard position) -> bool;
auto is_valid(const GameState &game_state, BitBoard position) -> bool;

auto get_row_col(BitBoard position, int32_t &row, int32_t &col) -> void;
auto get_tile_name(BitBoard position) -> std::string;

auto get_source(Move move) -> Square;
auto get_dest(Move move) -> Square;

auto init_standard(GameState &game_state) -> void;

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

auto perform_user_move(GameState &game_state) -> int32_t;
auto apply_move(GameState &game_state, Move move) -> void;
}  // namespace GameUtils