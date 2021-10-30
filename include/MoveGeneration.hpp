#pragma once

#include "GameState.hpp"
#include "Move.hpp"

namespace MoveGeneration {
auto get_moves_in_direction(const GameState &game_state, BitBoard bit_board, int32_t vertical, int32_t horizontal, Moves &moves) -> void;

auto get_moves(const GameState &game_state, Moves &moves) -> void;
auto get_pawn_moves(const GameState &game_state, Moves &moves) -> void;
auto get_knight_moves(const GameState &game_state, Moves &moves) -> void;
auto get_bishop_moves(const GameState &game_state, Moves &moves) -> void;
auto get_rook_moves(const GameState &game_state, Moves &moves) -> void;
auto get_queen_moves(const GameState &game_state, Moves &moves) -> void;
auto get_king_moves(const GameState &game_state, Moves &moves) -> void;

auto get_captures_in_direction(const GameState &game_state, BitBoard bit_board, int32_t vertical, int32_t horizontal) -> BitBoard;

auto get_capture_positions(const GameState &game_state, Color color) -> BitBoard;
auto get_pawn_capture_positions(const GameState &game_state, Color color) -> BitBoard;
auto get_knight_capture_positions(const GameState &game_state, Color color) -> BitBoard;
auto get_bishop_capture_positions(const GameState &game_state, Color color) -> BitBoard;
auto get_rook_capture_positions(const GameState &game_state, Color color) -> BitBoard;
auto get_queen_capture_positions(const GameState &game_state, Color color) -> BitBoard;
auto get_king_capture_positions(const GameState &game_state, Color color) -> BitBoard;
}  // namespace MoveGeneration