#pragma once

#include "GameState.hpp"
#include "GameUtils.hpp"
#include "Move.hpp"
#include "assert.h"

namespace MoveGeneration {

// Pseduo-Legal Moves
auto get_moves(const GameState &game_state, Moves &moves, Color color) -> void;
auto get_pawn_moves(const GameState &game_state, Moves &moves, Color color) -> void;
auto get_knight_moves(const GameState &game_state, Moves &moves, Color color) -> void;
auto get_bishop_moves(const GameState &game_state, Moves &moves, Color color) -> void;
auto get_rook_moves(const GameState &game_state, Moves &moves, Color color) -> void;
auto get_queen_moves(const GameState &game_state, Moves &moves, Color color) -> void;
auto get_king_moves(const GameState &game_state, Moves &moves, Color color) -> void;
auto get_moves_in_direction(const GameState &game_state, BitBoard bit_board, int32_t vertical, int32_t horizontal, Moves &moves, Color color) -> void;

// Pseduo-Legal Moves Templates
template <const int V, const int H, const Color C>
constexpr auto get_moves_in_direction(const GameState &game_state, BitBoard bit_board, Moves &moves) -> void {
    for (int i = 0; i < 7; ++i) {
        if constexpr (V > 0) {
            if (GameUtils::is_piece_in_top_row(bit_board)) {
                break;
            }
        } else if constexpr (V < 0) {
            if (GameUtils::is_piece_in_bottom_row(bit_board)) {
                break;
            }
        }

        if constexpr (H > 0) {
            if (GameUtils::is_piece_in_right_col(bit_board)) {
                break;
            }
        } else if constexpr (H < 0) {
            if (GameUtils::is_piece_in_left_col(bit_board)) {
                break;
            }
        }

        bit_board = GameUtils::shift_bit_board<V, H>(bit_board);

        if (game_state.position.is_empty(bit_board)) {
            moves.push_back(0);  // TODO(EMU): PLACEHOLDER VALUES
        }

        if constexpr (C == Colors::WHITE) {
            if ((game_state.position.get_white_bit_board() & bit_board) == 0) {
                moves.push_back(0);  // TODO(EMU): PLACEHOLDER VALUES
            } else {
                break;
            }
        } else if constexpr (C == Colors::BLACK) {
            if ((game_state.position.get_black_bit_board() & bit_board) == 0) {
                moves.push_back(0);  // TODO(EMU): PLACEHOLDER VALUES
                break;
            } else {
                break;
            }
        }
    }
}

// Threaten Squares
auto get_capture_positions(const Position &position, Color color) -> BitBoard;
auto get_pawn_capture_positions(const Position &position, Color color) -> BitBoard;
auto get_knight_capture_positions(const Position &position, Color color) -> BitBoard;
auto get_bishop_capture_positions(const Position &position, Color color) -> BitBoard;
auto get_rook_capture_positions(const Position &position, Color color) -> BitBoard;
auto get_queen_capture_positions(const Position &position, Color color) -> BitBoard;
auto get_king_capture_positions(const Position &position, Color color) -> BitBoard;
auto get_captures_in_direction(const Position &position, BitBoard bit_board, int32_t vertical, int32_t horizontal) -> BitBoard;

// Threaten Squares Templates
template <const int V, const int H>
constexpr auto get_captures_in_direction(const Position &position, BitBoard bit_board) -> BitBoard {
    BitBoard capturable_bit_board = BitBoards::EMPTY;

    for (int i = 0; i < 8; ++i) {
        if constexpr (V > 0) {
            if (GameUtils::is_piece_in_top_row(bit_board)) {
                break;
            }
        } else if constexpr (V < 0) {
            if (GameUtils::is_piece_in_bottom_row(bit_board)) {
                break;
            }
        }

        if constexpr (H > 0) {
            if (GameUtils::is_piece_in_right_col(bit_board)) {
                break;
            }
        } else if constexpr (H < 0) {
            if (GameUtils::is_piece_in_left_col(bit_board)) {
                break;
            }
        }

        bit_board = GameUtils::shift_bit_board<V, H>(bit_board);
        capturable_bit_board |= bit_board;

        if (position.is_occupied(bit_board)) {
            break;
        }
    }

    return capturable_bit_board;
}
}  // namespace MoveGeneration