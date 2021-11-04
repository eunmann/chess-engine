#include "MoveGeneration.hpp"

#include "GameUtils.hpp"

auto MoveGeneration::get_moves_in_direction(const GameState &game_state, BitBoard bit_board, int32_t vertical, int32_t horizontal, Moves &moves) -> void {
    bool is_white_piece = game_state.white_to_move;

    for (int i = 0; i < 7; ++i) {
        if ((GameUtils::is_piece_in_top_row(bit_board) && vertical > 0) ||
            (GameUtils::is_piece_in_bottom_row(bit_board) && vertical < 0) ||
            (GameUtils::is_piece_in_right_col(bit_board) && horizontal > 0) ||
            (GameUtils::is_piece_in_left_col(bit_board) && horizontal < 0)) {
            break;
        }

        bit_board = GameUtils::shift_bit_board(bit_board, vertical, horizontal);

        if (GameUtils::is_empty(game_state.position, bit_board)) {
            moves.push_back(0);  // TODO(EMU): PLACEHOLDER VALUES

        } else if ((is_white_piece && ((game_state.position.get_white_bit_board() & bit_board) == 0)) ||
                   (!is_white_piece && ((game_state.position.get_black_bit_board() & bit_board) == 0))) {
            moves.push_back(0);  // TODO(EMU): PLACEHOLDER VALUES
            break;
        } else {
            break;
        }
    }
}

auto MoveGeneration::get_moves(const GameState &game_state, Moves &moves, Color color) -> void {
    // Add moves in order of piece value
    MoveGeneration::get_king_moves(game_state, moves, color);
    MoveGeneration::get_queen_moves(game_state, moves, color);
    MoveGeneration::get_rook_moves(game_state, moves, color);
    MoveGeneration::get_bishop_moves(game_state, moves, color);
    MoveGeneration::get_knight_moves(game_state, moves, color);
    MoveGeneration::get_pawn_moves(game_state, moves, color);
}

auto MoveGeneration::get_pawn_moves(const GameState &game_state, Moves &moves, Color color) -> void {
    const int64_t pawn_dir = game_state.white_to_move ? 1 : -1;
    const BitBoard pawn_position = game_state.position.get_piece_color_bit_board(PieceCodes::PAWN, Colors::bool_to_color(game_state.white_to_move));
    bool is_white_piece = game_state.white_to_move;

    if (pawn_position) {
        /* Up */
        BitBoard next_pawn_position = GameUtils::shift_bit_board(pawn_position, 1 * pawn_dir, 0);
        if (GameUtils::is_empty(game_state.position, next_pawn_position)) {
            if ((is_white_piece && GameUtils::is_piece_in_top_row(next_pawn_position)) ||
                (!is_white_piece && GameUtils::is_piece_in_bottom_row(next_pawn_position))) {
                for (int32_t i = PieceCodes::KNIGHT; i <= PieceCodes::QUEEN; ++i) {
                    moves.push_back(0);  // TODO(EMU): PLACEHOLDER VALUES
                }
            } else {
                moves.push_back(0);  // TODO(EMU): PLACEHOLDER VALUES
            }

            /* Up 2 first-move */
            if ((is_white_piece && GameUtils::is_piece_in_row(pawn_position, 1)) ||
                (!is_white_piece && GameUtils::is_piece_in_row(pawn_position, 6))) {
                next_pawn_position = GameUtils::shift_bit_board(pawn_position, 2 * pawn_dir, 0);
                if (GameUtils::is_empty(game_state.position, next_pawn_position)) {
                    moves.push_back(0);  // TODO(EMU): PLACEHOLDER VALUES
                    //move.pawn_ep = piece_index;
                }
            }
        }

        /* Capture Left and Right */
        BitBoard pawn_position_left_capture = GameUtils::is_piece_in_left_col(pawn_position) ? 0 : GameUtils::shift_bit_board(pawn_position, 1 * pawn_dir, -1);
        BitBoard pawn_position_right_capture = GameUtils::is_piece_in_right_col(pawn_position) ? 0 : GameUtils::shift_bit_board(pawn_position, 1 * pawn_dir, 1);

        if (((game_state.position.get_black_bit_board() & pawn_position_left_capture) != 0) ||
            (!is_white_piece && ((game_state.position.get_white_bit_board() & pawn_position_left_capture) != 0))) {
            if ((is_white_piece && GameUtils::is_piece_in_top_row(pawn_position_left_capture)) ||
                (!is_white_piece && GameUtils::is_piece_in_bottom_row(pawn_position_left_capture))) {
                for (int32_t i = PieceCodes::KNIGHT; i <= PieceCodes::QUEEN; ++i) {
                    moves.push_back(0);  // TODO(EMU): PLACEHOLDER VALUES
                }
            } else {
                moves.push_back(0);  // TODO(EMU): PLACEHOLDER VALUES
            }
        }

        if (((game_state.position.get_black_bit_board() & pawn_position_right_capture) != 0) ||
            (!is_white_piece && ((game_state.position.get_white_bit_board() & pawn_position_right_capture) != 0))) {
            if ((is_white_piece && GameUtils::is_piece_in_top_row(pawn_position_right_capture)) ||
                (!is_white_piece && GameUtils::is_piece_in_bottom_row(pawn_position_right_capture))) {
                for (int32_t i = PieceCodes::KNIGHT; i <= PieceCodes::QUEEN; ++i) {
                    moves.push_back(0);  // TODO(EMU): PLACEHOLDER VALUES
                }
            } else {
                moves.push_back(0);  // TODO(EMU): PLACEHOLDER VALUES
            }
        }

        /* En Passant */
        int32_t row, col;
        GameUtils::get_row_col(pawn_position, row, col);
        const int32_t left_ep = is_white_piece ? col - 1 + PIECES_PER_PLAYER : col - 1 - PIECES_PER_PLAYER;
        const int32_t right_ep = is_white_piece ? col + 1 + PIECES_PER_PLAYER : col + 1 - PIECES_PER_PLAYER;
        const int32_t en_row = is_white_piece ? 4 : 3;

        if (game_state.pawn_ep == left_ep && GameUtils::is_piece_in_row(pawn_position, en_row)) {
            moves.push_back(0);  // TODO(EMU): PLACEHOLDER VALUES
        }

        if (game_state.pawn_ep == right_ep && GameUtils::is_piece_in_row(pawn_position, en_row)) {
            moves.push_back(0);  // TODO(EMU): PLACEHOLDER VALUES
        }
    }
}

auto MoveGeneration::get_knight_moves(const GameState &game_state, Moves &moves, Color color) -> void {
    BitBoard knight_bit_board = game_state.position.get_piece_color_bit_board(PieceCodes::KNIGHT, Colors::bool_to_color(game_state.white_to_move));

    if (knight_bit_board) {
        if (!(GameUtils::is_piece_in_top_2_row(knight_bit_board) || GameUtils::is_piece_in_left_col(knight_bit_board))) {
            BitBoard next_knight_bit_board = GameUtils::shift_bit_board<2, -1>(knight_bit_board);
            moves.push_back(0);  // TODO(EMU): PLACEHOLDER VALUES
        }
        if (!(GameUtils::is_piece_in_top_row(knight_bit_board) || GameUtils::is_piece_in_left_2_col(knight_bit_board))) {
            BitBoard next_knight_bit_board = GameUtils::shift_bit_board<1, -2>(knight_bit_board);
            moves.push_back(0);  // TODO(EMU): PLACEHOLDER VALUES
        }

        if (!(GameUtils::is_piece_in_bottom_row(knight_bit_board) || GameUtils::is_piece_in_left_2_col(knight_bit_board))) {
            BitBoard next_knight_bit_board = GameUtils::shift_bit_board<-1, -2>(knight_bit_board);
            moves.push_back(0);  // TODO(EMU): PLACEHOLDER VALUES
        }

        if (!(GameUtils::is_piece_in_bottom_2_row(knight_bit_board) || GameUtils::is_piece_in_left_col(knight_bit_board))) {
            BitBoard next_knight_bit_board = GameUtils::shift_bit_board<-2, -1>(knight_bit_board);
            moves.push_back(0);  // TODO(EMU): PLACEHOLDER VALUES
        }

        if (!(GameUtils::is_piece_in_bottom_2_row(knight_bit_board) || GameUtils::is_piece_in_right_col(knight_bit_board))) {
            BitBoard next_knight_bit_board = GameUtils::shift_bit_board<-2, 1>(knight_bit_board);
            moves.push_back(0);  // TODO(EMU): PLACEHOLDER VALUES
        }

        if (!(GameUtils::is_piece_in_bottom_row(knight_bit_board) || GameUtils::is_piece_in_right_2_col(knight_bit_board))) {
            BitBoard next_knight_bit_board = GameUtils::shift_bit_board<-1, 2>(knight_bit_board);
            moves.push_back(0);  // TODO(EMU): PLACEHOLDER VALUES
        }

        if (!(GameUtils::is_piece_in_top_row(knight_bit_board) || GameUtils::is_piece_in_right_2_col(knight_bit_board))) {
            BitBoard next_knight_bit_board = GameUtils::shift_bit_board<1, 2>(knight_bit_board);
            moves.push_back(0);  // TODO(EMU): PLACEHOLDER VALUES
        }

        if (!(GameUtils::is_piece_in_top_2_row(knight_bit_board) || GameUtils::is_piece_in_right_col(knight_bit_board))) {
            BitBoard next_knight_bit_board = GameUtils::shift_bit_board<2, 1>(knight_bit_board);
            moves.push_back(0);  // TODO(EMU): PLACEHOLDER VALUES
        }
    }
}

auto MoveGeneration::get_bishop_moves(const GameState &game_state, Moves &moves, Color color) -> void {
    BitBoard bishop_position = game_state.position.get_piece_color_bit_board(PieceCodes::BISHOP, Colors::bool_to_color(game_state.white_to_move));

    if (bishop_position) {
        MoveGeneration::get_moves_in_direction(game_state, bishop_position, 1, 1, moves);
        MoveGeneration::get_moves_in_direction(game_state, bishop_position, -1, 1, moves);
        MoveGeneration::get_moves_in_direction(game_state, bishop_position, 1, -1, moves);
        MoveGeneration::get_moves_in_direction(game_state, bishop_position, -1, -1, moves);
    }
}

auto MoveGeneration::get_rook_moves(const GameState &game_state, Moves &moves, Color color) -> void {
    BitBoard rook_position = game_state.position.get_piece_color_bit_board(PieceCodes::BISHOP, Colors::bool_to_color(game_state.white_to_move));

    if (rook_position) {
        MoveGeneration::get_moves_in_direction(game_state, rook_position, 1, 0, moves);
        MoveGeneration::get_moves_in_direction(game_state, rook_position, -1, 0, moves);
        MoveGeneration::get_moves_in_direction(game_state, rook_position, 0, -1, moves);
        MoveGeneration::get_moves_in_direction(game_state, rook_position, 0, 1, moves);
    }
}

auto MoveGeneration::get_queen_moves(const GameState &game_state, Moves &moves, Color color) -> void {
    BitBoard queens_bit_board = game_state.position.get_piece_color_bit_board(PieceCodes::KNIGHT, color);
    GameUtils::for_each_bit_board(queens_bit_board, [&game_state, &moves](BitBoard queen_bit_board) {
        // Diagonal
        MoveGeneration::get_moves_in_direction(game_state, queen_bit_board, 1, 1, moves);
        MoveGeneration::get_moves_in_direction(game_state, queen_bit_board, -1, 1, moves);
        MoveGeneration::get_moves_in_direction(game_state, queen_bit_board, 1, -1, moves);
        MoveGeneration::get_moves_in_direction(game_state, queen_bit_board, -1, -1, moves);

        // Horizontal and vertical
        MoveGeneration::get_moves_in_direction(game_state, queen_bit_board, 1, 0, moves);
        MoveGeneration::get_moves_in_direction(game_state, queen_bit_board, -1, 0, moves);
        MoveGeneration::get_moves_in_direction(game_state, queen_bit_board, 0, -1, moves);
        MoveGeneration::get_moves_in_direction(game_state, queen_bit_board, 0, 1, moves);
    });
}

auto MoveGeneration::get_king_moves(const GameState &game_state, Moves &moves, Color color) -> void {
    BitBoard king_position = game_state.position.get_piece_color_bit_board(PieceCodes::KING, Colors::bool_to_color(game_state.white_to_move));

    GameUtils::for_each_bit_board(king_position, [game_state, &moves](BitBoard bit_board) {
        /* Up */
        if (!GameUtils::is_piece_in_top_row(bit_board)) {
            BitBoard next_king_position = GameUtils::shift_bit_board<1, 0>(bit_board);
            moves.push_back(0);  // TODO(EMU): PLACEHOLDER VALUES
        }

        /* Down */
        if (!GameUtils::is_piece_in_bottom_row(bit_board)) {
            BitBoard next_king_position = GameUtils::shift_bit_board<-1, 0>(bit_board);
            moves.push_back(0);  // TODO(EMU): PLACEHOLDER VALUES
        }

        /* Left */
        if (!GameUtils::is_piece_in_left_col(bit_board)) {
            BitBoard next_king_position = GameUtils::shift_bit_board<0, -1>(bit_board);
            moves.push_back(0);  // TODO(EMU): PLACEHOLDER VALUES
        }

        /* Right */
        if (!GameUtils::is_piece_in_right_col(bit_board)) {
            BitBoard next_king_position = GameUtils::shift_bit_board<0, 1>(bit_board);
            moves.push_back(0);  // TODO(EMU): PLACEHOLDER VALUES
        }

        /* Up-Right */
        if (!(GameUtils::is_piece_in_top_row(bit_board) || GameUtils::is_piece_in_right_col(bit_board))) {
            BitBoard next_king_position = GameUtils::shift_bit_board<1, 1>(bit_board);
            moves.push_back(0);  // TODO(EMU): PLACEHOLDER VALUES
        }

        /* Up-Left */
        if (!(GameUtils::is_piece_in_top_row(bit_board) || GameUtils::is_piece_in_left_col(bit_board))) {
            BitBoard next_king_position = GameUtils::shift_bit_board<1, -1>(bit_board);
            moves.push_back(0);  // TODO(EMU): PLACEHOLDER VALUES
        }

        /* Down-Right */
        if (!(GameUtils::is_piece_in_bottom_row(bit_board) || GameUtils::is_piece_in_right_col(bit_board))) {
            BitBoard next_king_position = GameUtils::shift_bit_board<-1, 1>(bit_board);
            moves.push_back(0);  // TODO(EMU): PLACEHOLDER VALUES
        }

        /* Down-Left */
        if (!(GameUtils::is_piece_in_bottom_row(bit_board) || GameUtils::is_piece_in_left_col(bit_board))) {
            BitBoard next_king_position = GameUtils::shift_bit_board<-1, -1>(bit_board);
            moves.push_back(0);  // TODO(EMU): PLACEHOLDER VALUES
        }

        /* Castling */
        bool is_white_piece = game_state.white_to_move;
        if ((is_white_piece && !game_state.white_king_moved && !game_state.white_king_in_check) ||
            (!is_white_piece && !game_state.black_king_moved && !game_state.black_king_in_check)) {
            /* Queen Side */
            if ((is_white_piece && !game_state.white_rook_1_moved) || (!is_white_piece && !game_state.black_rook_1_moved)) {
                BitBoard next_king_position = GameUtils::shift_bit_board<0, -2>(bit_board);
                BitBoard next_rook_position = GameUtils::shift_bit_board<0, 1>(next_king_position);
                BitBoard empty_position = GameUtils::shift_bit_board<0, -1>(next_king_position);

                BitBoard castle_threw_positions = next_rook_position | next_king_position;
                BitBoard attack_positions = MoveGeneration::get_capture_positions(game_state, !game_state.white_to_move);

                if (((castle_threw_positions & attack_positions) == 0) &&
                    GameUtils::is_empty(game_state.position, castle_threw_positions | empty_position)) {
                    moves.push_back(0);  // TODO(EMU): PLACEHOLDER VALUES
                }
            }

            /* King Side */
            if ((is_white_piece && !game_state.white_rook_2_moved) || (!is_white_piece && !game_state.black_rook_2_moved)) {
                BitBoard next_king_position = GameUtils::shift_bit_board<0, 2>(bit_board);
                BitBoard next_rook_position = GameUtils::shift_bit_board<0, -1>(next_king_position);

                BitBoard castle_threw_positions = next_rook_position | next_king_position;
                BitBoard attack_positions = MoveGeneration::get_capture_positions(game_state, !game_state.white_to_move);

                if (((castle_threw_positions & attack_positions) == 0) &&
                    GameUtils::is_empty(game_state.position, castle_threw_positions)) {
                    moves.push_back(0);  // TODO(EMU): PLACEHOLDER VALUES
                }
            }
        }
    });
}

auto MoveGeneration::get_captures_in_direction(const GameState &game_state, BitBoard bit_board, int32_t vertical, int32_t horizontal) -> BitBoard {
    BitBoard capturable_bit_board = 0;

    for (int i = 0; i < 8; ++i) {
        if ((GameUtils::is_piece_in_top_row(bit_board) && vertical > 0) ||
            (GameUtils::is_piece_in_bottom_row(bit_board) && vertical < 0) ||
            (GameUtils::is_piece_in_right_col(bit_board) && horizontal > 0) ||
            (GameUtils::is_piece_in_left_col(bit_board) && horizontal < 0)) {
            break;
        }

        bit_board = GameUtils::shift_bit_board(bit_board, vertical, horizontal);
        capturable_bit_board |= bit_board;

        if (!GameUtils::is_empty(game_state.position, bit_board)) {
            break;
        }
    }

    return capturable_bit_board;
}

auto MoveGeneration::get_capture_positions(const GameState &game_state, Color color) -> BitBoard {
    BitBoard capturable_bit_board = 0;
    capturable_bit_board |= MoveGeneration::get_pawn_capture_positions(game_state, color);
    capturable_bit_board |= MoveGeneration::get_knight_capture_positions(game_state, color);
    capturable_bit_board |= MoveGeneration::get_bishop_capture_positions(game_state, color);
    capturable_bit_board |= MoveGeneration::get_rook_capture_positions(game_state, color);
    capturable_bit_board |= MoveGeneration::get_queen_capture_positions(game_state, color);
    capturable_bit_board |= MoveGeneration::get_king_capture_positions(game_state, color);
    return capturable_bit_board;
}

auto MoveGeneration::get_pawn_capture_positions(const GameState &game_state, Color color) -> BitBoard {
    const int64_t pawn_dir = color == Colors::WHITE ? 1 : -1;
    BitBoard capturable_bit_board = 0;
    BitBoard pawns_bit_board = game_state.position.get_piece_color_bit_board(PieceCodes::KNIGHT, color);
    GameUtils::for_each_bit_board(pawns_bit_board, [&capturable_bit_board, &game_state, pawn_dir](BitBoard pawn_bit_board) {
        /* Capture Left and Right */
        BitBoard pawn_bit_board_left_capture = GameUtils::is_piece_in_left_col(pawn_bit_board) ? 0 : GameUtils::shift_bit_board(pawn_bit_board, 1 * pawn_dir, -1);
        BitBoard pawn_bit_board_right_capture = GameUtils::is_piece_in_right_col(pawn_bit_board) ? 0 : GameUtils::shift_bit_board(pawn_bit_board, 1 * pawn_dir, 1);
        capturable_bit_board |= pawn_bit_board_left_capture | pawn_bit_board_right_capture;
    });
    return capturable_bit_board;
}

auto MoveGeneration::get_knight_capture_positions(const GameState &game_state, Color color) -> BitBoard {
    BitBoard capturable_bit_board = 0;
    BitBoard knights_bit_board = game_state.position.get_piece_color_bit_board(PieceCodes::KNIGHT, color);
    GameUtils::for_each_bit_board(knights_bit_board, [&capturable_bit_board, &game_state](BitBoard knight_bit_board) {
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

auto MoveGeneration::get_bishop_capture_positions(const GameState &game_state, Color color) -> BitBoard {
    BitBoard capturable_bit_board = 0;
    BitBoard bishops_bit_board = game_state.position.get_piece_color_bit_board(PieceCodes::BISHOP, color);
    GameUtils::for_each_bit_board(bishops_bit_board, [&capturable_bit_board, &game_state](BitBoard bishop_bit_board) {
        capturable_bit_board |= MoveGeneration::get_captures_in_direction(game_state, bishop_bit_board, 1, 1);
        capturable_bit_board |= MoveGeneration::get_captures_in_direction(game_state, bishop_bit_board, -1, 1);
        capturable_bit_board |= MoveGeneration::get_captures_in_direction(game_state, bishop_bit_board, 1, -1);
        capturable_bit_board |= MoveGeneration::get_captures_in_direction(game_state, bishop_bit_board, -1, -1);
    });
    return capturable_bit_board;
}

auto MoveGeneration::get_rook_capture_positions(const GameState &game_state, Color color) -> BitBoard {
    BitBoard capturable_bit_board = 0;
    BitBoard rooks_bit_board = game_state.position.get_piece_color_bit_board(PieceCodes::ROOK, color);
    GameUtils::for_each_bit_board(rooks_bit_board, [&capturable_bit_board, &game_state](BitBoard rook_bit_board) {
        capturable_bit_board |= MoveGeneration::get_captures_in_direction(game_state, rook_bit_board, 1, 0);
        capturable_bit_board |= MoveGeneration::get_captures_in_direction(game_state, rook_bit_board, -1, 0);
        capturable_bit_board |= MoveGeneration::get_captures_in_direction(game_state, rook_bit_board, 0, -1);
        capturable_bit_board |= MoveGeneration::get_captures_in_direction(game_state, rook_bit_board, 0, 1);
    });
    return capturable_bit_board;
}
auto MoveGeneration::get_queen_capture_positions(const GameState &game_state, Color color) -> BitBoard {
    BitBoard capturable_bit_board = 0;
    BitBoard queens_bit_board = game_state.position.get_piece_color_bit_board(PieceCodes::QUEEN, color);
    GameUtils::for_each_bit_board(queens_bit_board, [&capturable_bit_board, &game_state](BitBoard queen_bit_board) {
        /* Diagonal */
        capturable_bit_board |= MoveGeneration::get_captures_in_direction(game_state, queen_bit_board, 1, 1);
        capturable_bit_board |= MoveGeneration::get_captures_in_direction(game_state, queen_bit_board, -1, 1);
        capturable_bit_board |= MoveGeneration::get_captures_in_direction(game_state, queen_bit_board, 1, -1);
        capturable_bit_board |= MoveGeneration::get_captures_in_direction(game_state, queen_bit_board, -1, -1);

        /* Vertical/Horizontal */
        capturable_bit_board |= MoveGeneration::get_captures_in_direction(game_state, queen_bit_board, 1, 0);
        capturable_bit_board |= MoveGeneration::get_captures_in_direction(game_state, queen_bit_board, -1, 0);
        capturable_bit_board |= MoveGeneration::get_captures_in_direction(game_state, queen_bit_board, 0, -1);
        capturable_bit_board |= MoveGeneration::get_captures_in_direction(game_state, queen_bit_board, 0, 1);
    });
    return capturable_bit_board;
}
auto MoveGeneration::get_king_capture_positions(const GameState &game_state, Color color) -> BitBoard {
    BitBoard capturable_bit_board = 0;
    BitBoard king_bit_board = game_state.position.get_piece_color_bit_board(PieceCodes::KING, color);
    if (king_bit_board) {
        /* Up */
        if (!GameUtils::is_piece_in_top_row(king_bit_board)) {
            BitBoard move = GameUtils::shift_bit_board<1, 0>(king_bit_board);
            capturable_bit_board |= move;
        }

        /* Down */
        if (!GameUtils::is_piece_in_bottom_row(king_bit_board)) {
            BitBoard move = GameUtils::shift_bit_board<-1, 0>(king_bit_board);
            capturable_bit_board |= move;
        }

        /* Left */
        if (!GameUtils::is_piece_in_left_col(king_bit_board)) {
            BitBoard move = GameUtils::shift_bit_board<0, -1>(king_bit_board);
            capturable_bit_board |= move;
        }

        /* Right */
        if (!GameUtils::is_piece_in_right_col(king_bit_board)) {
            BitBoard move = GameUtils::shift_bit_board<0, 1>(king_bit_board);
            capturable_bit_board |= move;
        }

        /* Up-Right */
        if (!(GameUtils::is_piece_in_top_row(king_bit_board) || GameUtils::is_piece_in_right_col(king_bit_board))) {
            BitBoard move = GameUtils::shift_bit_board<1, 1>(king_bit_board);
            capturable_bit_board |= move;
        }

        /* Up-Left */
        if (!(GameUtils::is_piece_in_top_row(king_bit_board) || GameUtils::is_piece_in_left_col(king_bit_board))) {
            BitBoard move = GameUtils::shift_bit_board<1, -1>(king_bit_board);
            capturable_bit_board |= move;
        }

        /* Down-Right */
        if (!(GameUtils::is_piece_in_bottom_row(king_bit_board) || GameUtils::is_piece_in_right_col(king_bit_board))) {
            BitBoard move = GameUtils::shift_bit_board<-1, 1>(king_bit_board);
            capturable_bit_board |= move;
        }

        /* Down-Left */
        if (!(GameUtils::is_piece_in_bottom_row(king_bit_board) || GameUtils::is_piece_in_left_col(king_bit_board))) {
            BitBoard move = GameUtils::shift_bit_board<-1, -1>(king_bit_board);
            capturable_bit_board |= move;
        }
    }

    return capturable_bit_board;
}