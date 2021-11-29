#pragma once

#include <functional>
#include <string>
#include <vector>

#include "Board.hpp"
#include "GameState.hpp"
#include "Move.hpp"
#include "Position.hpp"
#include "ThreadState.hpp"

namespace GameUtils {

<<<<<<< HEAD
void print_position(BitBoard position);
void copy(const Position &bit_board, Board &board);
BitBoard move(BitBoard position, int32_t vertical, int32_t horizontal);
bool is_empty(const Position &bit_board, BitBoard position);
bool is_valid(const GameState &game_state, BitBoard position, int32_t piece_index);

void get_row_col(BitBoard position, int32_t &row, int32_t &col);
std::string get_tile_name(BitBoard position);

void init_standard(GameState &game_state);

void get_moves_in_direction(const GameState &game_state, int32_t piece_index, int32_t vertical, int32_t horizontal, Moves &moves);

void get_moves(const GameState &game_state, Moves &moves);
void get_piece_moves(const GameState &game_state, int32_t piece_index, Moves &moves);
void get_pawn_moves(const GameState &game_state, int32_t piece_index, Moves &moves);
void get_knight_moves(const GameState &game_state, int32_t piece_index, Moves &moves);
void get_bishop_moves(const GameState &game_state, int32_t piece_index, Moves &moves);
void get_rook_moves(const GameState &game_state, int32_t piece_index, Moves &moves);
void get_queen_moves(const GameState &game_state, int32_t piece_index, Moves &moves);
void get_king_moves(const GameState &game_state, int32_t piece_index, Moves &moves);

BitBoard get_captures_in_direction(const GameState &game_state, int32_t piece_index, int32_t vertical, int32_t horizontal);

BitBoard get_capture_positions(const GameState &game_state, bool white_pieces);
BitBoard get_piece_capture_positions(const GameState &game_state, int32_t piece_index);
BitBoard get_pawn_capture_positions(const GameState &game_state, int32_t piece_index);
BitBoard get_knight_capture_positions(const GameState &game_state, int32_t piece_index);
BitBoard get_bishop_capture_positions(const GameState &game_state, int32_t piece_index);
BitBoard get_rook_capture_positions(const GameState &game_state, int32_t piece_index);
BitBoard get_queen_capture_positions(const GameState &game_state, int32_t piece_index);
BitBoard get_king_capture_positions(const GameState &game_state, int32_t piece_index);

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
void apply_move(GameState &game_state, BitBoard position, int32_t piece_index);

void get_best_move(ThreadState &thread_state);
int32_t alpha_beta_pruning_search(const ThreadState &thread_state, const GameState &game_state, int32_t ply_depth, int32_t alpha, int32_t beta, bool max_white);
int32_t get_position_heuristic(const GameState &game_state);

=======
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
  auto for_each_set_square(const BitBoard bit_board, const std::function<void(int32_t bit_index)>& func) noexcept -> void;
  auto for_each_bit_board(const BitBoard bit_board, const std::function<void(BitBoard bit_board)>& func) noexcept -> void;

  template <const int V, const int H>
  auto shift_bit_board(const BitBoard bit_board) noexcept -> BitBoard {
    constexpr int shift = V * 8 + H;
    if constexpr (shift >= 0) {
      return bit_board << shift;
    } else {
      return bit_board >> shift * -1;
    }
  }
>>>>>>> good
}  // namespace GameUtils