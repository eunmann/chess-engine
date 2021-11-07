#pragma once

#include "GameState.hpp"
#include "GameUtils.hpp"
#include "Move.hpp"
#include "assert.h"

namespace MoveGeneration {

// Pseduo-Legal Moves Templates
template <const int V, const int H, const Color C>
constexpr auto get_moves_in_direction(const GameState &game_state, BitBoard bit_board, Moves &moves) -> void {
    Square source_square = GameUtils::bit_board_to_square(bit_board);
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
            moves.push_back(Move(source_square, GameUtils::bit_board_to_square(bit_board)));
        }

        if constexpr (C == Colors::WHITE) {
            if ((game_state.position.get_white_bit_board() & bit_board) == 0) {
                moves.push_back(Move(source_square, GameUtils::bit_board_to_square(bit_board)));
            } else {
                break;
            }
        } else if constexpr (C == Colors::BLACK) {
            if ((game_state.position.get_black_bit_board() & bit_board) == 0) {
                moves.push_back(Move(source_square, GameUtils::bit_board_to_square(bit_board)));
                break;
            } else {
                break;
            }
        }
    }
}

template <const Color color>
constexpr auto get_pawn_moves(const GameState &game_state, Moves &moves) -> void {
    constexpr int64_t pawn_dir = color ? 1 : -1;
    const BitBoard pawns_bit_board = game_state.position.get_piece_color_bit_board(PieceCodes::PAWN, color);

    GameUtils::for_each_bit_board(pawns_bit_board, [&game_state, &moves, pawn_dir](const BitBoard pawn_bit_board) {
        Square source_square = GameUtils::bit_board_to_square(pawn_bit_board);
        auto add_promotion_moves = [source_square, &moves](const BitBoard destination_bit_board) {
            for (int32_t i = PieceCodes::KNIGHT; i <= PieceCodes::QUEEN; i++) {
                moves.push_back(0);  // TODO(EMU): PLACEHOLDER VALUES
            }
        };

        // Up
        BitBoard next_pawn_bit_board = GameUtils::shift_bit_board(pawn_bit_board, 1 * pawn_dir, 0);
        if (game_state.position.is_empty(next_pawn_bit_board)) {
            if constexpr (color == Colors::WHITE) {
                // Promotion
                if (GameUtils::is_piece_in_top_row(next_pawn_bit_board)) {
                    add_promotion_moves(next_pawn_bit_board);
                } else {
                    moves.push_back(Move(source_square, GameUtils::bit_board_to_square(next_pawn_bit_board)));
                }

                // First Move Up 2
                if (GameUtils::is_piece_in_row(pawn_bit_board, 1)) {
                    next_pawn_bit_board = GameUtils::shift_bit_board(pawn_bit_board, 2 * pawn_dir, 0);
                    if (game_state.position.is_empty(next_pawn_bit_board)) {
                        moves.push_back(Move(source_square, GameUtils::bit_board_to_square(next_pawn_bit_board)));
                        // TODO(EMU): Set enpassant-able flag
                    }
                }
            } else if constexpr (color == Colors::BLACK) {
                // Promotion
                if (GameUtils::is_piece_in_bottom_row(next_pawn_bit_board)) {
                    add_promotion_moves(next_pawn_bit_board);
                } else {
                    moves.push_back(Move(source_square, GameUtils::bit_board_to_square(next_pawn_bit_board)));
                }

                // First Move Up 2
                if (GameUtils::is_piece_in_row(pawn_bit_board, 6)) {
                    next_pawn_bit_board = GameUtils::shift_bit_board(pawn_bit_board, 2 * pawn_dir, 0);
                    if (game_state.position.is_empty(next_pawn_bit_board)) {
                        moves.push_back(Move(source_square, GameUtils::bit_board_to_square(next_pawn_bit_board)));
                        // TODO(EMU): Set enpassant-able flag
                    }
                }
            }
        }

        // Capture Left and Right
        BitBoard pawn_bit_board_left_capture = GameUtils::is_piece_in_left_col(pawn_bit_board) ? 0 : GameUtils::shift_bit_board(pawn_bit_board, 1 * pawn_dir, -1);
        BitBoard pawn_bit_board_right_capture = GameUtils::is_piece_in_right_col(pawn_bit_board) ? 0 : GameUtils::shift_bit_board(pawn_bit_board, 1 * pawn_dir, 1);

        if constexpr (color == Colors::WHITE) {
            if (game_state.position.is_black_occupied(pawn_bit_board_left_capture)) {
                // Promotions
                if (GameUtils::is_piece_in_top_row(pawn_bit_board_left_capture)) {
                    add_promotion_moves(pawn_bit_board_left_capture);
                } else {
                    moves.push_back(Move(source_square, GameUtils::bit_board_to_square(pawn_bit_board_left_capture)));
                }
            }

            if (game_state.position.is_black_occupied(pawn_bit_board_right_capture)) {
                // Promotions
                if (GameUtils::is_piece_in_top_row(pawn_bit_board_right_capture)) {
                    add_promotion_moves(pawn_bit_board_right_capture);
                } else {
                    moves.push_back(Move(source_square, GameUtils::bit_board_to_square(pawn_bit_board_right_capture)));
                }
            }
        } else if constexpr (color == Colors::BLACK) {
            if (game_state.position.is_white_occupied(pawn_bit_board_left_capture)) {
                // Promotions
                if (GameUtils::is_piece_in_top_row(pawn_bit_board_left_capture)) {
                    add_promotion_moves(pawn_bit_board_left_capture);
                } else {
                    moves.push_back(Move(source_square, GameUtils::bit_board_to_square(pawn_bit_board_left_capture)));
                }
            }

            if (game_state.position.is_white_occupied(pawn_bit_board_right_capture)) {
                // Promotions
                if (GameUtils::is_piece_in_top_row(pawn_bit_board_right_capture)) {
                    add_promotion_moves(pawn_bit_board_right_capture);
                } else {
                    moves.push_back(Move(source_square, GameUtils::bit_board_to_square(pawn_bit_board_right_capture)));
                }
            }
        }

        // TODO(EMU): En Passant
    });
}

template <const Color color>
constexpr auto get_knight_moves(const GameState &game_state, Moves &moves) -> void {
    BitBoard knights_bit_board = game_state.position.get_piece_color_bit_board(PieceCodes::KNIGHT, color);

    GameUtils::for_each_bit_board(knights_bit_board, [&game_state, &moves](const BitBoard knight_bit_board) {
        Square source_square = GameUtils::bit_board_to_square(knight_bit_board);

        if (!(GameUtils::is_piece_in_top_2_row(knight_bit_board) || GameUtils::is_piece_in_left_col(knight_bit_board))) {
            BitBoard next_knight_bit_board = GameUtils::shift_bit_board<2, -1>(knight_bit_board);
            moves.push_back(Move(source_square, GameUtils::bit_board_to_square(next_knight_bit_board)));
        }

        if (!(GameUtils::is_piece_in_top_row(knight_bit_board) || GameUtils::is_piece_in_left_2_col(knight_bit_board))) {
            BitBoard next_knight_bit_board = GameUtils::shift_bit_board<1, -2>(knight_bit_board);
            moves.push_back(Move(source_square, GameUtils::bit_board_to_square(next_knight_bit_board)));
        }

        if (!(GameUtils::is_piece_in_bottom_row(knight_bit_board) || GameUtils::is_piece_in_left_2_col(knight_bit_board))) {
            BitBoard next_knight_bit_board = GameUtils::shift_bit_board<-1, -2>(knight_bit_board);
            moves.push_back(Move(source_square, GameUtils::bit_board_to_square(next_knight_bit_board)));
        }

        if (!(GameUtils::is_piece_in_bottom_2_row(knight_bit_board) || GameUtils::is_piece_in_left_col(knight_bit_board))) {
            BitBoard next_knight_bit_board = GameUtils::shift_bit_board<-2, -1>(knight_bit_board);
            moves.push_back(Move(source_square, GameUtils::bit_board_to_square(next_knight_bit_board)));
        }

        if (!(GameUtils::is_piece_in_bottom_2_row(knight_bit_board) || GameUtils::is_piece_in_right_col(knight_bit_board))) {
            BitBoard next_knight_bit_board = GameUtils::shift_bit_board<-2, 1>(knight_bit_board);
            moves.push_back(Move(source_square, GameUtils::bit_board_to_square(next_knight_bit_board)));
        }

        if (!(GameUtils::is_piece_in_bottom_row(knight_bit_board) || GameUtils::is_piece_in_right_2_col(knight_bit_board))) {
            BitBoard next_knight_bit_board = GameUtils::shift_bit_board<-1, 2>(knight_bit_board);
            moves.push_back(Move(source_square, GameUtils::bit_board_to_square(next_knight_bit_board)));
        }

        if (!(GameUtils::is_piece_in_top_row(knight_bit_board) || GameUtils::is_piece_in_right_2_col(knight_bit_board))) {
            BitBoard next_knight_bit_board = GameUtils::shift_bit_board<1, 2>(knight_bit_board);
            moves.push_back(Move(source_square, GameUtils::bit_board_to_square(next_knight_bit_board)));
        }

        if (!(GameUtils::is_piece_in_top_2_row(knight_bit_board) || GameUtils::is_piece_in_right_col(knight_bit_board))) {
            BitBoard next_knight_bit_board = GameUtils::shift_bit_board<2, 1>(knight_bit_board);
            moves.push_back(Move(source_square, GameUtils::bit_board_to_square(next_knight_bit_board)));
        }
    });
}

template <const Color color>
constexpr auto get_bishop_moves(const GameState &game_state, Moves &moves) -> void {
    BitBoard bishops_bit_board = game_state.position.get_piece_color_bit_board(PieceCodes::BISHOP, color);

    GameUtils::for_each_bit_board(bishops_bit_board, [&game_state, &moves](const BitBoard bishop_bit_board) {
        MoveGeneration::get_moves_in_direction<1, 1, color>(game_state, bishop_bit_board, moves);
        MoveGeneration::get_moves_in_direction<-1, 1, color>(game_state, bishop_bit_board, moves);
        MoveGeneration::get_moves_in_direction<1, -1, color>(game_state, bishop_bit_board, moves);
        MoveGeneration::get_moves_in_direction<-1, -1, color>(game_state, bishop_bit_board, moves);
    });
}

template <const Color color>
constexpr auto get_rook_moves(const GameState &game_state, Moves &moves) -> void {
    BitBoard rooks_bit_board = game_state.position.get_piece_color_bit_board(PieceCodes::ROOK, color);

    GameUtils::for_each_bit_board(rooks_bit_board, [&game_state, &moves](const BitBoard rook_bit_board) {
        MoveGeneration::get_moves_in_direction<1, 0, color>(game_state, rook_bit_board, moves);
        MoveGeneration::get_moves_in_direction<-1, 0, color>(game_state, rook_bit_board, moves);
        MoveGeneration::get_moves_in_direction<0, -1, color>(game_state, rook_bit_board, moves);
        MoveGeneration::get_moves_in_direction<0, 1, color>(game_state, rook_bit_board, moves);
    });
}

template <const Color color>
constexpr auto get_queen_moves(const GameState &game_state, Moves &moves) -> void {
    BitBoard queens_bit_board = game_state.position.get_piece_color_bit_board(PieceCodes::QUEEN, color);
    GameUtils::for_each_bit_board(queens_bit_board, [&game_state, &moves](BitBoard queen_bit_board) {
        // Diagonal
        MoveGeneration::get_moves_in_direction<1, 1, color>(game_state, queen_bit_board, moves);
        MoveGeneration::get_moves_in_direction<-1, 1, color>(game_state, queen_bit_board, moves);
        MoveGeneration::get_moves_in_direction<1, -1, color>(game_state, queen_bit_board, moves);
        MoveGeneration::get_moves_in_direction<-1, -1, color>(game_state, queen_bit_board, moves);

        // Horizontal and vertical
        MoveGeneration::get_moves_in_direction<1, 0, color>(game_state, queen_bit_board, moves);
        MoveGeneration::get_moves_in_direction<-1, 0, color>(game_state, queen_bit_board, moves);
        MoveGeneration::get_moves_in_direction<0, -1, color>(game_state, queen_bit_board, moves);
        MoveGeneration::get_moves_in_direction<0, 1, color>(game_state, queen_bit_board, moves);
    });
}

template <const Color color>
constexpr auto get_king_moves(const GameState &game_state, Moves &moves) -> void {
    BitBoard kings_bit_board = game_state.position.get_piece_color_bit_board(PieceCodes::KING, color);

    GameUtils::for_each_bit_board(kings_bit_board, [&game_state, &moves](BitBoard king_bit_board) {
        Square source_square = GameUtils::bit_board_to_square(king_bit_board);

        // Up
        if (!GameUtils::is_piece_in_top_row(king_bit_board)) {
            BitBoard next_king_bit_board = GameUtils::shift_bit_board<1, 0>(king_bit_board);
            moves.push_back(Move(source_square, GameUtils::bit_board_to_square(next_king_bit_board)));
        }

        // Down
        if (!GameUtils::is_piece_in_bottom_row(king_bit_board)) {
            BitBoard next_king_bit_board = GameUtils::shift_bit_board<-1, 0>(king_bit_board);
            moves.push_back(Move(source_square, GameUtils::bit_board_to_square(next_king_bit_board)));
        }

        // Left
        if (!GameUtils::is_piece_in_left_col(king_bit_board)) {
            BitBoard next_king_bit_board = GameUtils::shift_bit_board<0, -1>(king_bit_board);
            moves.push_back(Move(source_square, GameUtils::bit_board_to_square(next_king_bit_board)));
        }

        // Right
        if (!GameUtils::is_piece_in_right_col(king_bit_board)) {
            BitBoard next_king_bit_board = GameUtils::shift_bit_board<0, 1>(king_bit_board);
            moves.push_back(Move(source_square, GameUtils::bit_board_to_square(next_king_bit_board)));
        }

        // Up-Right
        if (!(GameUtils::is_piece_in_top_row(king_bit_board) || GameUtils::is_piece_in_right_col(king_bit_board))) {
            BitBoard next_king_bit_board = GameUtils::shift_bit_board<1, 1>(king_bit_board);
            moves.push_back(Move(source_square, GameUtils::bit_board_to_square(next_king_bit_board)));
        }

        // Up-Left
        if (!(GameUtils::is_piece_in_top_row(king_bit_board) || GameUtils::is_piece_in_left_col(king_bit_board))) {
            BitBoard next_king_bit_board = GameUtils::shift_bit_board<1, -1>(king_bit_board);
            moves.push_back(Move(source_square, GameUtils::bit_board_to_square(next_king_bit_board)));
        }

        // Down-Right
        if (!(GameUtils::is_piece_in_bottom_row(king_bit_board) || GameUtils::is_piece_in_right_col(king_bit_board))) {
            BitBoard next_king_bit_board = GameUtils::shift_bit_board<-1, 1>(king_bit_board);
            moves.push_back(Move(source_square, GameUtils::bit_board_to_square(next_king_bit_board)));
        }

        // Down-Left
        if (!(GameUtils::is_piece_in_bottom_row(king_bit_board) || GameUtils::is_piece_in_left_col(king_bit_board))) {
            BitBoard next_king_bit_board = GameUtils::shift_bit_board<-1, -1>(king_bit_board);
            moves.push_back(Move(source_square, GameUtils::bit_board_to_square(next_king_bit_board)));
        }

        if constexpr (color == Colors::WHITE) {
            // Castling
            if (!game_state.white_king_moved && !game_state.white_king_in_check) {
                // Queen Side
                if (!game_state.white_rook_1_moved) {
                    BitBoard next_king_bit_board = GameUtils::shift_bit_board<0, -2>(king_bit_board);
                    BitBoard next_rook_position = GameUtils::shift_bit_board<0, 1>(next_king_bit_board);
                    BitBoard empty_position = GameUtils::shift_bit_board<0, -1>(next_king_bit_board);
                    //moves.push_back(0);  // TODO(EMU): PLACEHOLDER VALUES
                }

                // King Side
                if (!game_state.white_rook_2_moved) {
                    BitBoard next_king_bit_board = GameUtils::shift_bit_board<0, 2>(king_bit_board);
                    BitBoard next_rook_position = GameUtils::shift_bit_board<0, -1>(next_king_bit_board);
                    //moves.push_back(0);  // TODO(EMU): PLACEHOLDER VALUES
                }
            }
        } else if constexpr (color == Colors::BLACK) {
            // Castling
            if (!game_state.black_king_moved && !game_state.black_king_in_check) {
                // Queen Side
                if (!game_state.black_rook_1_moved) {
                    BitBoard next_king_bit_board = GameUtils::shift_bit_board<0, -2>(king_bit_board);
                    BitBoard next_rook_position = GameUtils::shift_bit_board<0, 1>(next_king_bit_board);
                    BitBoard empty_position = GameUtils::shift_bit_board<0, -1>(next_king_bit_board);
                    //moves.push_back(0);  // TODO(EMU): PLACEHOLDER VALUES
                }

                // King Side
                if (!game_state.black_rook_2_moved) {
                    BitBoard next_king_bit_board = GameUtils::shift_bit_board<0, 2>(king_bit_board);
                    BitBoard next_rook_position = GameUtils::shift_bit_board<0, -1>(next_king_bit_board);
                    //moves.push_back(0);  // TODO(EMU): PLACEHOLDER VALUES
                }
            }
        }
    });
}

template <const Color color>
constexpr auto get_moves(const GameState &game_state, Moves &moves) -> void {
    // Add moves in order of piece value
    MoveGeneration::get_king_moves<color>(game_state, moves);
    MoveGeneration::get_queen_moves<color>(game_state, moves);
    MoveGeneration::get_rook_moves<color>(game_state, moves);
    MoveGeneration::get_bishop_moves<color>(game_state, moves);
    MoveGeneration::get_knight_moves<color>(game_state, moves);
    MoveGeneration::get_pawn_moves<color>(game_state, moves);
}

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

template <const Color color>
constexpr auto get_pawn_capture_positions(const Position &position) -> BitBoard {
    constexpr int64_t pawn_dir = color == Colors::WHITE ? 1 : -1;
    BitBoard capturable_bit_board = BitBoards::EMPTY;
    BitBoard pawns_bit_board = position.get_piece_color_bit_board(PieceCodes::PAWN, color);
    GameUtils::for_each_bit_board(pawns_bit_board, [&capturable_bit_board, pawn_dir](BitBoard pawn_bit_board) {
        // Capture Left and Right
        BitBoard pawn_bit_board_left_capture = GameUtils::is_piece_in_left_col(pawn_bit_board) ? 0 : GameUtils::shift_bit_board<1 * pawn_dir, -1>(pawn_bit_board);
        BitBoard pawn_bit_board_right_capture = GameUtils::is_piece_in_right_col(pawn_bit_board) ? 0 : GameUtils::shift_bit_board<1 * pawn_dir, 1>(pawn_bit_board);
        capturable_bit_board |= pawn_bit_board_left_capture | pawn_bit_board_right_capture;
    });
    return capturable_bit_board;
}

template <const Color color>
constexpr auto get_knight_capture_positions(const Position &position) -> BitBoard {
    BitBoard capturable_bit_board = BitBoards::EMPTY;
    BitBoard knights_bit_board = position.get_piece_color_bit_board(PieceCodes::KNIGHT, color);
    GameUtils::for_each_bit_board(knights_bit_board, [&capturable_bit_board](BitBoard knight_bit_board) {
        // TODO(EMU): These if statements can be condensed. Also, better checks might be possible
        if (!(GameUtils::is_piece_in_top_2_row(knight_bit_board) || GameUtils::is_piece_in_left_col(knight_bit_board))) {
            BitBoard next_knight_bit_board = GameUtils::shift_bit_board<2, -1>(knight_bit_board);
            capturable_bit_board |= next_knight_bit_board;
        }

        if (!(GameUtils::is_piece_in_top_2_row(knight_bit_board) || GameUtils::is_piece_in_right_col(knight_bit_board))) {
            BitBoard next_knight_bit_board = GameUtils::shift_bit_board<2, 1>(knight_bit_board);
            capturable_bit_board |= next_knight_bit_board;
        }

        if (!(GameUtils::is_piece_in_top_row(knight_bit_board) || GameUtils::is_piece_in_left_2_col(knight_bit_board))) {
            BitBoard next_knight_bit_board = GameUtils::shift_bit_board<1, -2>(knight_bit_board);
            capturable_bit_board |= next_knight_bit_board;
        }

        if (!(GameUtils::is_piece_in_top_row(knight_bit_board) || GameUtils::is_piece_in_right_2_col(knight_bit_board))) {
            BitBoard next_knight_bit_board = GameUtils::shift_bit_board<1, 2>(knight_bit_board);
            capturable_bit_board |= next_knight_bit_board;
        }

        if (!(GameUtils::is_piece_in_bottom_row(knight_bit_board) || GameUtils::is_piece_in_left_2_col(knight_bit_board))) {
            BitBoard next_knight_bit_board = GameUtils::shift_bit_board<-1, -2>(knight_bit_board);
            capturable_bit_board |= next_knight_bit_board;
        }

        if (!(GameUtils::is_piece_in_bottom_row(knight_bit_board) || GameUtils::is_piece_in_right_2_col(knight_bit_board))) {
            BitBoard next_knight_bit_board = GameUtils::shift_bit_board<-1, 2>(knight_bit_board);
            capturable_bit_board |= next_knight_bit_board;
        }

        if (!(GameUtils::is_piece_in_bottom_2_row(knight_bit_board) || GameUtils::is_piece_in_left_col(knight_bit_board))) {
            BitBoard next_knight_bit_board = GameUtils::shift_bit_board<-2, -1>(knight_bit_board);
            capturable_bit_board |= next_knight_bit_board;
        }

        if (!(GameUtils::is_piece_in_bottom_2_row(knight_bit_board) || GameUtils::is_piece_in_right_col(knight_bit_board))) {
            BitBoard next_knight_bit_board = GameUtils::shift_bit_board<-2, 1>(knight_bit_board);
            capturable_bit_board |= next_knight_bit_board;
        }
    });

    return capturable_bit_board;
}

template <const Color color>
constexpr auto get_bishop_capture_positions(const Position &position) -> BitBoard {
    BitBoard capturable_bit_board = BitBoards::EMPTY;
    BitBoard bishops_bit_board = position.get_piece_color_bit_board(PieceCodes::BISHOP, color);
    GameUtils::for_each_bit_board(bishops_bit_board, [&capturable_bit_board, &position](BitBoard bishop_bit_board) {
        capturable_bit_board |= MoveGeneration::get_captures_in_direction<1, 1>(position, bishop_bit_board);
        capturable_bit_board |= MoveGeneration::get_captures_in_direction<-1, 1>(position, bishop_bit_board);
        capturable_bit_board |= MoveGeneration::get_captures_in_direction<1, -1>(position, bishop_bit_board);
        capturable_bit_board |= MoveGeneration::get_captures_in_direction<-1, -1>(position, bishop_bit_board);
    });
    return capturable_bit_board;
}

template <const Color color>
constexpr auto get_rook_capture_positions(const Position &position) -> BitBoard {
    BitBoard capturable_bit_board = BitBoards::EMPTY;
    BitBoard rooks_bit_board = position.get_piece_color_bit_board(PieceCodes::ROOK, color);
    GameUtils::for_each_bit_board(rooks_bit_board, [&capturable_bit_board, &position](BitBoard rook_bit_board) {
        capturable_bit_board |= MoveGeneration::get_captures_in_direction<1, 0>(position, rook_bit_board);
        capturable_bit_board |= MoveGeneration::get_captures_in_direction<-1, 0>(position, rook_bit_board);
        capturable_bit_board |= MoveGeneration::get_captures_in_direction<0, -1>(position, rook_bit_board);
        capturable_bit_board |= MoveGeneration::get_captures_in_direction<0, 1>(position, rook_bit_board);
    });
    return capturable_bit_board;
}

template <const Color color>
constexpr auto get_queen_capture_positions(const Position &position) -> BitBoard {
    BitBoard capturable_bit_board = BitBoards::EMPTY;
    BitBoard queens_bit_board = position.get_piece_color_bit_board(PieceCodes::QUEEN, color);
    GameUtils::for_each_bit_board(queens_bit_board, [&capturable_bit_board, &position](BitBoard queen_bit_board) {
        // Diagonal
        capturable_bit_board |= MoveGeneration::get_captures_in_direction<1, 1>(position, queen_bit_board);
        capturable_bit_board |= MoveGeneration::get_captures_in_direction<-1, 1>(position, queen_bit_board);
        capturable_bit_board |= MoveGeneration::get_captures_in_direction<1, -1>(position, queen_bit_board);
        capturable_bit_board |= MoveGeneration::get_captures_in_direction<-1, -1>(position, queen_bit_board);

        // Vertical/Horizontal
        capturable_bit_board |= MoveGeneration::get_captures_in_direction<1, 0>(position, queen_bit_board);
        capturable_bit_board |= MoveGeneration::get_captures_in_direction<-1, 0>(position, queen_bit_board);
        capturable_bit_board |= MoveGeneration::get_captures_in_direction<0, -1>(position, queen_bit_board);
        capturable_bit_board |= MoveGeneration::get_captures_in_direction<0, 1>(position, queen_bit_board);
    });
    return capturable_bit_board;
}

template <const Color color>
constexpr auto get_king_capture_positions(const Position &position) -> BitBoard {
    BitBoard capturable_bit_board = BitBoards::EMPTY;
    BitBoard kings_bit_board = position.get_piece_color_bit_board(PieceCodes::KING, color);

    GameUtils::for_each_bit_board(kings_bit_board, [&capturable_bit_board](const BitBoard king_bit_board) {
        // Up
        if (!GameUtils::is_piece_in_top_row(king_bit_board)) {
            BitBoard move = GameUtils::shift_bit_board<1, 0>(king_bit_board);
            capturable_bit_board |= move;
        }

        // Down
        if (!GameUtils::is_piece_in_bottom_row(king_bit_board)) {
            BitBoard move = GameUtils::shift_bit_board<-1, 0>(king_bit_board);
            capturable_bit_board |= move;
        }

        // Left
        if (!GameUtils::is_piece_in_left_col(king_bit_board)) {
            BitBoard move = GameUtils::shift_bit_board<0, -1>(king_bit_board);
            capturable_bit_board |= move;
        }

        // Right
        if (!GameUtils::is_piece_in_right_col(king_bit_board)) {
            BitBoard move = GameUtils::shift_bit_board<0, 1>(king_bit_board);
            capturable_bit_board |= move;
        }

        // Up-Right
        if (!(GameUtils::is_piece_in_top_row(king_bit_board) || GameUtils::is_piece_in_right_col(king_bit_board))) {
            BitBoard move = GameUtils::shift_bit_board<1, 1>(king_bit_board);
            capturable_bit_board |= move;
        }

        // Up-Left
        if (!(GameUtils::is_piece_in_top_row(king_bit_board) || GameUtils::is_piece_in_left_col(king_bit_board))) {
            BitBoard move = GameUtils::shift_bit_board<1, -1>(king_bit_board);
            capturable_bit_board |= move;
        }

        // Down-Right
        if (!(GameUtils::is_piece_in_bottom_row(king_bit_board) || GameUtils::is_piece_in_right_col(king_bit_board))) {
            BitBoard move = GameUtils::shift_bit_board<-1, 1>(king_bit_board);
            capturable_bit_board |= move;
        }

        // Down-Left
        if (!(GameUtils::is_piece_in_bottom_row(king_bit_board) || GameUtils::is_piece_in_left_col(king_bit_board))) {
            BitBoard move = GameUtils::shift_bit_board<-1, -1>(king_bit_board);
            capturable_bit_board |= move;
        }
    });

    return capturable_bit_board;
}

template <const Color color>
constexpr auto get_capture_positions(const Position &position) -> BitBoard {
    BitBoard capturable_bit_board = BitBoards::EMPTY;
    capturable_bit_board |= MoveGeneration::get_pawn_capture_positions<color>(position);
    capturable_bit_board |= MoveGeneration::get_knight_capture_positions<color>(position);
    capturable_bit_board |= MoveGeneration::get_bishop_capture_positions<color>(position);
    capturable_bit_board |= MoveGeneration::get_rook_capture_positions<color>(position);
    capturable_bit_board |= MoveGeneration::get_queen_capture_positions<color>(position);
    capturable_bit_board |= MoveGeneration::get_king_capture_positions<color>(position);
    return capturable_bit_board;
}
}  // namespace MoveGeneration