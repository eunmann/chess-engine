#include "MoveGeneration.hpp"

#include "GameUtils.hpp"

auto MoveGeneration::get_moves_in_direction(const GameState &game_state, BitBoard bit_board, int32_t vertical, int32_t horizontal, Moves &moves) -> void {
    BitBoard position = bit_board;
    bool is_white_piece = game_state.white_to_move;

    for (int i = 0; i < 7; ++i) {
        if ((GameUtils::is_piece_in_top_row(position) && vertical > 0) ||
            (GameUtils::is_piece_in_bottom_row(position) && vertical < 0) ||
            (GameUtils::is_piece_in_right_col(position) && horizontal > 0) ||
            (GameUtils::is_piece_in_left_col(position) && horizontal < 0)) {
            break;
        }

        position = GameUtils::move(position, vertical, horizontal);

        if (GameUtils::is_empty(game_state.position, position)) {
            Move &move = moves.back_ref();

        } else if ((is_white_piece && ((game_state.position.get_white_bit_board() & position) == 0)) ||
                   (!is_white_piece && ((game_state.position.get_black_bit_board() & position) == 0))) {
            Move &move = moves.back_ref();

            moves.back_ref() = move;
            break;
        } else {
            break;
        }
    }
}

auto MoveGeneration::get_moves(const GameState &game_state, Moves &moves) -> void {
    MoveGeneration::get_pawn_moves(game_state, moves);
    MoveGeneration::get_knight_moves(game_state, moves);
    MoveGeneration::get_bishop_moves(game_state, moves);
    MoveGeneration::get_rook_moves(game_state, moves);
    MoveGeneration::get_queen_moves(game_state, moves);
    MoveGeneration::get_king_moves(game_state, moves);
}

auto MoveGeneration::get_pawn_moves(const GameState &game_state, Moves &moves) -> void {
    const int64_t pawn_dir = game_state.white_to_move ? 1 : -1;
    const BitBoard pawn_position = game_state.position.get_piece_color_bit_board(PieceCodes::PAWN, Colors::bool_to_color(game_state.white_to_move));
    bool is_white_piece = game_state.white_to_move;

    if (pawn_position) {
        /* Up */
        BitBoard next_pawn_position = GameUtils::move(pawn_position, 1 * pawn_dir, 0);
        if (GameUtils::is_empty(game_state.position, next_pawn_position)) {
            if ((is_white_piece && GameUtils::is_piece_in_top_row(next_pawn_position)) ||
                (!is_white_piece && GameUtils::is_piece_in_bottom_row(next_pawn_position))) {
                for (int32_t i = PieceCodes::KNIGHT; i <= PieceCodes::QUEEN; ++i) {
                    Move &move = moves.back_ref();
                }
            } else {
                Move &move = moves.back_ref();
            }

            /* Up 2 first-move */
            if ((is_white_piece && GameUtils::is_piece_in_row(pawn_position, 1)) ||
                (!is_white_piece && GameUtils::is_piece_in_row(pawn_position, 6))) {
                next_pawn_position = GameUtils::move(pawn_position, 2 * pawn_dir, 0);
                if (GameUtils::is_empty(game_state.position, next_pawn_position)) {
                    Move &move = moves.back_ref();
                    //move.pawn_ep = piece_index;
                }
            }
        }

        /* Capture Left and Right */
        BitBoard pawn_position_left_capture = GameUtils::is_piece_in_left_col(pawn_position) ? 0 : GameUtils::move(pawn_position, 1 * pawn_dir, -1);
        BitBoard pawn_position_right_capture = GameUtils::is_piece_in_right_col(pawn_position) ? 0 : GameUtils::move(pawn_position, 1 * pawn_dir, 1);

        if (((game_state.position.get_black_bit_board() & pawn_position_left_capture) != 0) ||
            (!is_white_piece && ((game_state.position.get_white_bit_board() & pawn_position_left_capture) != 0))) {
            if ((is_white_piece && GameUtils::is_piece_in_top_row(pawn_position_left_capture)) ||
                (!is_white_piece && GameUtils::is_piece_in_bottom_row(pawn_position_left_capture))) {
                for (int32_t i = PieceCodes::KNIGHT; i <= PieceCodes::QUEEN; ++i) {
                    Move &move = moves.back_ref();
                }
            } else {
                Move &move = moves.back_ref();
            }
        }

        if (((game_state.position.get_black_bit_board() & pawn_position_right_capture) != 0) ||
            (!is_white_piece && ((game_state.position.get_white_bit_board() & pawn_position_right_capture) != 0))) {
            if ((is_white_piece && GameUtils::is_piece_in_top_row(pawn_position_right_capture)) ||
                (!is_white_piece && GameUtils::is_piece_in_bottom_row(pawn_position_right_capture))) {
                for (int32_t i = PieceCodes::KNIGHT; i <= PieceCodes::QUEEN; ++i) {
                    Move &move = moves.back_ref();
                }
            } else {
                Move &move = moves.back_ref();
            }
        }

        /* En Passant */
        int32_t row, col;
        GameUtils::get_row_col(pawn_position, row, col);
        const int32_t left_ep = is_white_piece ? col - 1 + PIECES_PER_PLAYER : col - 1 - PIECES_PER_PLAYER;
        const int32_t right_ep = is_white_piece ? col + 1 + PIECES_PER_PLAYER : col + 1 - PIECES_PER_PLAYER;
        const int32_t en_row = is_white_piece ? 4 : 3;

        if (game_state.pawn_ep == left_ep && GameUtils::is_piece_in_row(pawn_position, en_row)) {
            Move &move = moves.back_ref();
        }

        if (game_state.pawn_ep == right_ep && GameUtils::is_piece_in_row(pawn_position, en_row)) {
            Move &move = moves.back_ref();
        }
    }
}

auto MoveGeneration::get_knight_moves(const GameState &game_state, Moves &moves) -> void {
    BitBoard knight_position = game_state.position.get_piece_color_bit_board(PieceCodes::KNIGHT, Colors::bool_to_color(game_state.white_to_move));

    if (knight_position) {
        if (!(GameUtils::is_piece_in_top_2_row(knight_position) || GameUtils::is_piece_in_left_col(knight_position))) {
            BitBoard next_knight_position = GameUtils::move(knight_position, 2, -1);
            Move &move = moves.back_ref();
        }
        if (!(GameUtils::is_piece_in_top_row(knight_position) || GameUtils::is_piece_in_left_2_col(knight_position))) {
            BitBoard next_knight_position = GameUtils::move(knight_position, 1, -2);
            Move &move = moves.back_ref();
        }

        if (!(GameUtils::is_piece_in_bottom_row(knight_position) || GameUtils::is_piece_in_left_2_col(knight_position))) {
            BitBoard next_knight_position = GameUtils::move(knight_position, -1, -2);
            Move &move = moves.back_ref();
        }

        if (!(GameUtils::is_piece_in_bottom_2_row(knight_position) || GameUtils::is_piece_in_left_col(knight_position))) {
            BitBoard next_knight_position = GameUtils::move(knight_position, -2, -1);
            Move &move = moves.back_ref();
        }

        if (!(GameUtils::is_piece_in_bottom_2_row(knight_position) || GameUtils::is_piece_in_right_col(knight_position))) {
            BitBoard next_knight_position = GameUtils::move(knight_position, -2, 1);
            Move &move = moves.back_ref();
        }

        if (!(GameUtils::is_piece_in_bottom_row(knight_position) || GameUtils::is_piece_in_right_2_col(knight_position))) {
            BitBoard next_knight_position = GameUtils::move(knight_position, -1, 2);
            Move &move = moves.back_ref();
        }

        if (!(GameUtils::is_piece_in_top_row(knight_position) || GameUtils::is_piece_in_right_2_col(knight_position))) {
            BitBoard next_knight_position = GameUtils::move(knight_position, 1, 2);
            Move &move = moves.back_ref();
        }

        if (!(GameUtils::is_piece_in_top_2_row(knight_position) || GameUtils::is_piece_in_right_col(knight_position))) {
            BitBoard next_knight_position = GameUtils::move(knight_position, 2, 1);
            Move &move = moves.back_ref();
        }
    }
}

auto MoveGeneration::get_bishop_moves(const GameState &game_state, Moves &moves) -> void {
    BitBoard bishop_position = game_state.position.get_piece_color_bit_board(PieceCodes::BISHOP, Colors::bool_to_color(game_state.white_to_move));

    if (bishop_position) {
        MoveGeneration::get_moves_in_direction(game_state, bishop_position, 1, 1, moves);
        MoveGeneration::get_moves_in_direction(game_state, bishop_position, -1, 1, moves);
        MoveGeneration::get_moves_in_direction(game_state, bishop_position, 1, -1, moves);
        MoveGeneration::get_moves_in_direction(game_state, bishop_position, -1, -1, moves);
    }
}

auto MoveGeneration::get_rook_moves(const GameState &game_state, Moves &moves) -> void {
    BitBoard rook_position = game_state.position.get_piece_color_bit_board(PieceCodes::BISHOP, Colors::bool_to_color(game_state.white_to_move));

    if (rook_position) {
        MoveGeneration::get_moves_in_direction(game_state, rook_position, 1, 0, moves);
        MoveGeneration::get_moves_in_direction(game_state, rook_position, -1, 0, moves);
        MoveGeneration::get_moves_in_direction(game_state, rook_position, 0, -1, moves);
        MoveGeneration::get_moves_in_direction(game_state, rook_position, 0, 1, moves);
    }
}

auto MoveGeneration::get_queen_moves(const GameState &game_state, Moves &moves) -> void {
    BitBoard queen_position = game_state.position.get_piece_color_bit_board(PieceCodes::BISHOP, Colors::bool_to_color(game_state.white_to_move));

    if (queen_position) {
        // Diagonal
        MoveGeneration::get_moves_in_direction(game_state, queen_position, 1, 1, moves);
        MoveGeneration::get_moves_in_direction(game_state, queen_position, -1, 1, moves);
        MoveGeneration::get_moves_in_direction(game_state, queen_position, 1, -1, moves);
        MoveGeneration::get_moves_in_direction(game_state, queen_position, -1, -1, moves);

        // Horizontal and vertical
        MoveGeneration::get_moves_in_direction(game_state, queen_position, 1, 0, moves);
        MoveGeneration::get_moves_in_direction(game_state, queen_position, -1, 0, moves);
        MoveGeneration::get_moves_in_direction(game_state, queen_position, 0, -1, moves);
        MoveGeneration::get_moves_in_direction(game_state, queen_position, 0, 1, moves);
    }
}

auto MoveGeneration::get_king_moves(const GameState &game_state, Moves &moves) -> void {
    BitBoard king_position = game_state.position.get_piece_color_bit_board(PieceCodes::BISHOP, Colors::bool_to_color(game_state.white_to_move));
    if (king_position) {
        /* Up */
        if (!GameUtils::is_piece_in_top_row(king_position)) {
            BitBoard next_king_position = GameUtils::move(king_position, 1, 0);
            Move &move = moves.back_ref();
        }

        /* Down */
        if (!GameUtils::is_piece_in_bottom_row(king_position)) {
            BitBoard next_king_position = GameUtils::move(king_position, -1, 0);

            Move &move = moves.back_ref();
        }

        /* Left */
        if (!GameUtils::is_piece_in_left_col(king_position)) {
            BitBoard next_king_position = GameUtils::move(king_position, 0, -1);

            Move &move = moves.back_ref();
        }

        /* Right */
        if (!GameUtils::is_piece_in_right_col(king_position)) {
            BitBoard next_king_position = GameUtils::move(king_position, 0, 1);

            Move &move = moves.back_ref();
        }

        /* Up-Right */
        if (!(GameUtils::is_piece_in_top_row(king_position) || GameUtils::is_piece_in_right_col(king_position))) {
            BitBoard next_king_position = GameUtils::move(king_position, 1, 1);

            Move &move = moves.back_ref();
        }

        /* Up-Left */
        if (!(GameUtils::is_piece_in_top_row(king_position) || GameUtils::is_piece_in_left_col(king_position))) {
            BitBoard next_king_position = GameUtils::move(king_position, 1, -1);

            Move &move = moves.back_ref();
        }

        /* Down-Right */
        if (!(GameUtils::is_piece_in_bottom_row(king_position) || GameUtils::is_piece_in_right_col(king_position))) {
            BitBoard next_king_position = GameUtils::move(king_position, -1, 1);

            Move &move = moves.back_ref();
        }

        /* Down-Left */
        if (!(GameUtils::is_piece_in_bottom_row(king_position) || GameUtils::is_piece_in_left_col(king_position))) {
            BitBoard next_king_position = GameUtils::move(king_position, -1, -1);

            Move &move = moves.back_ref();
        }

        /* Castling */
        bool is_white_piece = game_state.white_to_move;
        if ((is_white_piece && !game_state.white_king_moved && !game_state.white_king_in_check) ||
            (!is_white_piece && !game_state.black_king_moved && !game_state.black_king_in_check)) {
            /* Queen Side */
            if ((is_white_piece && !game_state.white_rook_1_moved) || (!is_white_piece && !game_state.black_rook_1_moved)) {
                BitBoard next_king_position = GameUtils::move(king_position, 0, -2);
                BitBoard next_rook_position = GameUtils::move(next_king_position, 0, 1);
                BitBoard empty_position = GameUtils::move(next_king_position, 0, -1);

                BitBoard castle_threw_positions = next_rook_position | next_king_position;
                BitBoard attack_positions = MoveGeneration::get_capture_positions(game_state, !game_state.white_to_move);

                if (((castle_threw_positions & attack_positions) == 0) &&
                    GameUtils::is_empty(game_state.position, castle_threw_positions | empty_position)) {
                    Move &move = moves.back_ref();
                }
            }

            /* King Side */
            if ((is_white_piece && !game_state.white_rook_2_moved) || (!is_white_piece && !game_state.black_rook_2_moved)) {
                BitBoard next_king_position = GameUtils::move(king_position, 0, 2);
                BitBoard next_rook_position = GameUtils::move(next_king_position, 0, -1);

                BitBoard castle_threw_positions = next_rook_position | next_king_position;
                BitBoard attack_positions = MoveGeneration::get_capture_positions(game_state, !game_state.white_to_move);

                if (((castle_threw_positions & attack_positions) == 0) &&
                    GameUtils::is_empty(game_state.position, castle_threw_positions)) {
                    Move &move = moves.back_ref();
                }
            }
        }
    }
}

auto MoveGeneration::get_captures_in_direction(const GameState &game_state, BitBoard bit_board, int32_t vertical, int32_t horizontal) -> BitBoard {
    BitBoard position = bit_board;
    BitBoard capturable_positions = 0;

    for (int i = 0; i < 8; ++i) {
        if ((GameUtils::is_piece_in_top_row(position) && vertical > 0) ||
            (GameUtils::is_piece_in_bottom_row(position) && vertical < 0) ||
            (GameUtils::is_piece_in_right_col(position) && horizontal > 0) ||
            (GameUtils::is_piece_in_left_col(position) && horizontal < 0)) {
            break;
        }

        position = GameUtils::move(position, vertical, horizontal);
        capturable_positions |= position;

        if (!GameUtils::is_empty(game_state.position, position)) {
            break;
        }
    }

    return capturable_positions;
}

auto MoveGeneration::get_capture_positions(const GameState &game_state, Color color) -> BitBoard {
    BitBoard capturable_positions = 0;
    capturable_positions |= MoveGeneration::get_pawn_capture_positions(game_state, color);
    capturable_positions |= MoveGeneration::get_knight_capture_positions(game_state, color);
    capturable_positions |= MoveGeneration::get_bishop_capture_positions(game_state, color);
    capturable_positions |= MoveGeneration::get_rook_capture_positions(game_state, color);
    capturable_positions |= MoveGeneration::get_queen_capture_positions(game_state, color);
    capturable_positions |= MoveGeneration::get_king_capture_positions(game_state, color);
    return capturable_positions;
}

auto MoveGeneration::get_pawn_capture_positions(const GameState &game_state, Color color) -> BitBoard {
    const int64_t pawn_dir = color == Colors::WHITE ? 1 : -1;
    const BitBoard pawn_position = game_state.position.get_piece_color_bit_board(PieceCodes::PAWN, color);
    BitBoard capturable_positions = 0;

    if (pawn_position) {
        /* Capture Left and Right */
        BitBoard pawn_position_left_capture = GameUtils::is_piece_in_left_col(pawn_position) ? 0 : GameUtils::move(pawn_position, 1 * pawn_dir, -1);
        BitBoard pawn_position_right_capture = GameUtils::is_piece_in_right_col(pawn_position) ? 0 : GameUtils::move(pawn_position, 1 * pawn_dir, 1);
        capturable_positions |= pawn_position_left_capture | pawn_position_right_capture;
    }

    return capturable_positions;
}

auto MoveGeneration::get_knight_capture_positions(const GameState &game_state, Color color) -> BitBoard {
    BitBoard knight_position = game_state.position.get_piece_color_bit_board(PieceCodes::KNIGHT, color);
    BitBoard capturable_positions = 0;

    if (knight_position) {
        if (!(GameUtils::is_piece_in_top_2_row(knight_position) || GameUtils::is_piece_in_left_col(knight_position))) {
            BitBoard next_knight_position = GameUtils::move(knight_position, 2, -1);
            capturable_positions |= next_knight_position;
        }
        if (!(GameUtils::is_piece_in_top_row(knight_position) || GameUtils::is_piece_in_left_2_col(knight_position))) {
            BitBoard next_knight_position = GameUtils::move(knight_position, 1, -2);
            capturable_positions |= next_knight_position;
        }

        if (!(GameUtils::is_piece_in_bottom_row(knight_position) || GameUtils::is_piece_in_left_2_col(knight_position))) {
            BitBoard next_knight_position = GameUtils::move(knight_position, -1, -2);
            capturable_positions |= next_knight_position;
        }

        if (!(GameUtils::is_piece_in_bottom_2_row(knight_position) || GameUtils::is_piece_in_left_col(knight_position))) {
            BitBoard next_knight_position = GameUtils::move(knight_position, -2, -1);
            capturable_positions |= next_knight_position;
        }

        if (!(GameUtils::is_piece_in_bottom_2_row(knight_position) || GameUtils::is_piece_in_right_col(knight_position))) {
            BitBoard next_knight_position = GameUtils::move(knight_position, -2, 1);
            capturable_positions |= next_knight_position;
        }

        if (!(GameUtils::is_piece_in_bottom_row(knight_position) || GameUtils::is_piece_in_right_2_col(knight_position))) {
            BitBoard next_knight_position = GameUtils::move(knight_position, -1, 2);
            capturable_positions |= next_knight_position;
        }

        if (!(GameUtils::is_piece_in_top_row(knight_position) || GameUtils::is_piece_in_right_2_col(knight_position))) {
            BitBoard next_knight_position = GameUtils::move(knight_position, 1, 2);
            capturable_positions |= next_knight_position;
        }

        if (!(GameUtils::is_piece_in_top_2_row(knight_position) || GameUtils::is_piece_in_right_col(knight_position))) {
            BitBoard next_knight_position = GameUtils::move(knight_position, 2, 1);
            capturable_positions |= next_knight_position;
        }
    }

    return capturable_positions;
}

auto MoveGeneration::get_bishop_capture_positions(const GameState &game_state, Color color) -> BitBoard {
    BitBoard bishop_position = game_state.position.get_piece_color_bit_board(PieceCodes::KNIGHT, color);
    int64_t capturable_positions = 0;
    if (bishop_position) {
        capturable_positions |= MoveGeneration::get_captures_in_direction(game_state, bishop_position, 1, 1);
        capturable_positions |= MoveGeneration::get_captures_in_direction(game_state, bishop_position, -1, 1);
        capturable_positions |= MoveGeneration::get_captures_in_direction(game_state, bishop_position, 1, -1);
        capturable_positions |= MoveGeneration::get_captures_in_direction(game_state, bishop_position, -1, -1);
    }

    return capturable_positions;
}

auto MoveGeneration::get_rook_capture_positions(const GameState &game_state, Color color) -> BitBoard {
    int64_t capturable_positions = 0;
    if (game_state.position.get_piece_bit_board(PieceCodes::ROOK)) {
        capturable_positions |= MoveGeneration::get_captures_in_direction(game_state, PieceCodes::ROOK, 1, 0);
        capturable_positions |= MoveGeneration::get_captures_in_direction(game_state, PieceCodes::ROOK, -1, 0);
        capturable_positions |= MoveGeneration::get_captures_in_direction(game_state, PieceCodes::ROOK, 0, -1);
        capturable_positions |= MoveGeneration::get_captures_in_direction(game_state, PieceCodes::ROOK, 0, 1);
    }
    return capturable_positions;
}
auto MoveGeneration::get_queen_capture_positions(const GameState &game_state, Color color) -> BitBoard {
    int64_t capturable_positions = 0;

    if (game_state.position.get_piece_bit_board(PieceCodes::QUEEN)) {
        /* Diagonal */
        capturable_positions |= MoveGeneration::get_captures_in_direction(game_state, PieceCodes::QUEEN, 1, 1);
        capturable_positions |= MoveGeneration::get_captures_in_direction(game_state, PieceCodes::QUEEN, -1, 1);
        capturable_positions |= MoveGeneration::get_captures_in_direction(game_state, PieceCodes::QUEEN, 1, -1);
        capturable_positions |= MoveGeneration::get_captures_in_direction(game_state, PieceCodes::QUEEN, -1, -1);

        /* Vertical/Horizontal */
        capturable_positions |= MoveGeneration::get_captures_in_direction(game_state, PieceCodes::QUEEN, 1, 0);
        capturable_positions |= MoveGeneration::get_captures_in_direction(game_state, PieceCodes::QUEEN, -1, 0);
        capturable_positions |= MoveGeneration::get_captures_in_direction(game_state, PieceCodes::QUEEN, 0, -1);
        capturable_positions |= MoveGeneration::get_captures_in_direction(game_state, PieceCodes::QUEEN, 0, 1);
    }

    return capturable_positions;
}
auto MoveGeneration::get_king_capture_positions(const GameState &game_state, Color color) -> BitBoard {
    BitBoard capturable_positions = 0;
    BitBoard position = game_state.position.get_piece_bit_board(PieceCodes::KING);
    position |= game_state.position.get_color_bit_board(Colors::bool_to_color(game_state.white_to_move));
    if (position) {
        /* Up */
        if (!GameUtils::is_piece_in_top_row(position)) {
            BitBoard move = GameUtils::move(position, 1, 0);
            capturable_positions |= move;
        }

        /* Down */
        if (!GameUtils::is_piece_in_bottom_row(position)) {
            BitBoard move = GameUtils::move(position, -1, 0);
            capturable_positions |= move;
        }

        /* Left */
        if (!GameUtils::is_piece_in_left_col(position)) {
            BitBoard move = GameUtils::move(position, 0, -1);
            capturable_positions |= move;
        }

        /* Right */
        if (!GameUtils::is_piece_in_right_col(position)) {
            BitBoard move = GameUtils::move(position, 0, 1);
            capturable_positions |= move;
        }

        /* Up-Right */
        if (!(GameUtils::is_piece_in_top_row(position) || GameUtils::is_piece_in_right_col(position))) {
            BitBoard move = GameUtils::move(position, 1, 1);
            capturable_positions |= move;
        }

        /* Up-Left */
        if (!(GameUtils::is_piece_in_top_row(position) || GameUtils::is_piece_in_left_col(position))) {
            BitBoard move = GameUtils::move(position, 1, -1);
            capturable_positions |= move;
        }

        /* Down-Right */
        if (!(GameUtils::is_piece_in_bottom_row(position) || GameUtils::is_piece_in_right_col(position))) {
            BitBoard move = GameUtils::move(position, -1, 1);
            capturable_positions |= move;
        }

        /* Down-Left */
        if (!(GameUtils::is_piece_in_bottom_row(position) || GameUtils::is_piece_in_left_col(position))) {
            BitBoard move = GameUtils::move(position, -1, -1);
            capturable_positions |= move;
        }
    }

    return capturable_positions;
}