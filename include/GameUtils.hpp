#pragma once

#include <string>
#include <vector>

#include "Board.hpp"
#include "GameState.hpp"
#include "Move.hpp"
#include "Position.hpp"

namespace GameUtils {

void print_position(BitBoard position);
void copy(const Position &bit_board, Board &board);
BitBoard move(BitBoard position, int32_t vertical, int32_t horizontal);
bool is_empty(const Position &bit_board, BitBoard position);
bool is_valid(const GameState &game_state, BitBoard position);

void get_row_col(BitBoard position, int32_t &row, int32_t &col);
std::string get_tile_name(BitBoard position);

void init_standard(GameState &game_state);

void get_moves_in_direction(const GameState &game_state, int32_t vertical, int32_t horizontal, Moves &moves);

void get_moves(const GameState &game_state, Moves &moves);
void get_pawn_moves(const GameState &game_state, Moves &moves);
void get_knight_moves(const GameState &game_state, Moves &moves);
void get_bishop_moves(const GameState &game_state, Moves &moves);
void get_rook_moves(const GameState &game_state, Moves &moves);
void get_queen_moves(const GameState &game_state, Moves &moves);
void get_king_moves(const GameState &game_state, Moves &moves);

BitBoard get_captures_in_direction(const GameState &game_state, int32_t vertical, int32_t horizontal);

BitBoard get_capture_positions(const GameState &game_state, bool white_pieces);
BitBoard get_pawn_capture_positions(const GameState &game_state);
BitBoard get_knight_capture_positions(const GameState &game_state);
BitBoard get_bishop_capture_positions(const GameState &game_state);
BitBoard get_rook_capture_positions(const GameState &game_state);
BitBoard get_queen_capture_positions(const GameState &game_state);
BitBoard get_king_capture_positions(const GameState &game_state);

bool is_piece_in_row(BitBoard position, int32_t row);
bool is_piece_in_col(BitBoard position, int32_t col);
bool is_piece_in_top_row(BitBoard position);
bool is_piece_in_top_2_row(BitBoard position);
bool is_piece_in_bottom_row(BitBoard position);
bool is_piece_in_bottom_2_row(BitBoard position);
bool is_piece_in_left_col(BitBoard position);
bool is_piece_in_left_2_col(BitBoard position);
bool is_piece_in_right_col(BitBoard position);
bool is_piece_in_right_2_col(BitBoard position);

int32_t perform_user_move(GameState &game_state);
void apply_move(GameState &game_state, BitBoard position);

Move get_best_move(const GameState &game_state);
int32_t alpha_beta_pruning_search(const GameState &game_state, int32_t ply_depth, int32_t alpha, int32_t beta, bool max_white);
int32_t get_position_heuristic(const GameState &game_state);

}  // namespace GameUtils