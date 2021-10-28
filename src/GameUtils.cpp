#include "GameUtils.hpp"

#include <stdio.h>

#include <iostream>
#include <string>

#include "Definitions.hpp"

void GameUtils::print_position(BitBoard position) {
    const BitBoard left_bit = 1ULL << 63;
    for (int32_t j = 0; j < 8; ++j) {
        for (int32_t i = 0; i < 8; ++i) {
            if (left_bit & position) {
                printf("1");
            } else {
                printf("0");
            }

            position <<= 1;
        }
        printf("\n");
    }
}

BitBoard GameUtils::move(BitBoard position, int32_t vertical, int32_t horizontal) {
    int32_t shift = vertical * 8 + horizontal;
    if (shift >= 0) {
        return position << shift;
    } else {
        return position >> (shift * -1);
    }
}

bool GameUtils::is_empty(const Position &position, BitBoard bit_board) {
    return (position.get_empty_bit_board() & bit_board) == 0;
}

bool GameUtils::is_valid(const GameState &game_state, BitBoard position) {
    /*
    bool is_white_piece = piece_index < PIECES_PER_PLAYER;

    if ((is_white_piece && ((game_state.position.get_color_bit_board(Colors::WHITE) & position) != 0)) ||
        (!is_white_piece && ((game_state.position.get_color_bit_board(Colors::BLACK) & position) != 0))) {
        return false;
    }

    // TODO(EMU): I don't like this copy here
    GameState temp = game_state;
    GameUtils::apply_move(temp, position, piece_index);

    if (is_white_piece) {
        return (temp.position.get_piece_bit_board(PieceCodes::KING) & temp.position.get_white_bit_board() & temp.position.get_black_threaten()) == 0;
    } else {
        return (temp.position.get_piece_bit_board(PieceCodes::KING) & temp.position.get_black_bit_board() & temp.position.get_white_threaten()) == 0;
    }
    */
    return true;
}

void GameUtils::get_row_col(BitBoard position, int32_t &row, int32_t &col) {
    BitBoard row_mask = 0xFFULL;
    BitBoard col_mask = 0x0101010101010101ULL;

    row = -1;
    col = -1;

    for (int i = 0; i < 8; ++i) {
        if ((position & row_mask) != 0) {
            row = i;
        }
        if ((position & col_mask) != 0) {
            col = i;
        }
        row_mask = row_mask << 8;
        col_mask = col_mask << 1;
    }
}

std::string GameUtils::get_tile_name(BitBoard position) {
    int32_t row, col;
    GameUtils::get_row_col(position, row, col);

    std::string name = "";
    name += (char)(col + 'a');
    name += (char)(row + '1');

    return name;
}

void GameUtils::init_standard(GameState &game_state) {
    game_state.position.init();

    game_state.white_to_move = true;

    game_state.white_to_move = true;
    game_state.white_king_moved = false;
    game_state.white_rook_1_moved = false;
    game_state.white_rook_2_moved = false;
    game_state.black_king_moved = false;
    game_state.black_rook_1_moved = false;
    game_state.black_rook_2_moved = false;
    game_state.black_king_in_check = false;

    game_state.pawn_ep = -128;
}

void GameUtils::get_moves_in_direction(const GameState &game_state, BitBoard bit_board, int32_t vertical, int32_t horizontal, Moves &moves) {
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

void GameUtils::get_moves(const GameState &game_state, Moves &moves) {
    GameUtils::get_pawn_moves(game_state, moves);
    GameUtils::get_knight_moves(game_state, moves);
    GameUtils::get_bishop_moves(game_state, moves);
    GameUtils::get_rook_moves(game_state, moves);
    GameUtils::get_queen_moves(game_state, moves);
    GameUtils::get_king_moves(game_state, moves);
}

void GameUtils::get_pawn_moves(const GameState &game_state, Moves &moves) {
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

void GameUtils::get_knight_moves(const GameState &game_state, Moves &moves) {
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

void GameUtils::get_bishop_moves(const GameState &game_state, Moves &moves) {
    BitBoard bishop_position = game_state.position.get_piece_color_bit_board(PieceCodes::BISHOP, Colors::bool_to_color(game_state.white_to_move));

    if (bishop_position) {
        GameUtils::get_moves_in_direction(game_state, bishop_position, 1, 1, moves);
        GameUtils::get_moves_in_direction(game_state, bishop_position, -1, 1, moves);
        GameUtils::get_moves_in_direction(game_state, bishop_position, 1, -1, moves);
        GameUtils::get_moves_in_direction(game_state, bishop_position, -1, -1, moves);
    }
}

void GameUtils::get_rook_moves(const GameState &game_state, Moves &moves) {
    BitBoard rook_position = game_state.position.get_piece_color_bit_board(PieceCodes::BISHOP, Colors::bool_to_color(game_state.white_to_move));

    if (rook_position) {
        GameUtils::get_moves_in_direction(game_state, rook_position, 1, 0, moves);
        GameUtils::get_moves_in_direction(game_state, rook_position, -1, 0, moves);
        GameUtils::get_moves_in_direction(game_state, rook_position, 0, -1, moves);
        GameUtils::get_moves_in_direction(game_state, rook_position, 0, 1, moves);
    }
}

void GameUtils::get_queen_moves(const GameState &game_state, Moves &moves) {
    BitBoard queen_position = game_state.position.get_piece_color_bit_board(PieceCodes::BISHOP, Colors::bool_to_color(game_state.white_to_move));

    if (queen_position) {
        // Diagonal
        GameUtils::get_moves_in_direction(game_state, queen_position, 1, 1, moves);
        GameUtils::get_moves_in_direction(game_state, queen_position, -1, 1, moves);
        GameUtils::get_moves_in_direction(game_state, queen_position, 1, -1, moves);
        GameUtils::get_moves_in_direction(game_state, queen_position, -1, -1, moves);

        // Horizontal and vertical
        GameUtils::get_moves_in_direction(game_state, queen_position, 1, 0, moves);
        GameUtils::get_moves_in_direction(game_state, queen_position, -1, 0, moves);
        GameUtils::get_moves_in_direction(game_state, queen_position, 0, -1, moves);
        GameUtils::get_moves_in_direction(game_state, queen_position, 0, 1, moves);
    }
}

void GameUtils::get_king_moves(const GameState &game_state, Moves &moves) {
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
                BitBoard attack_positions = GameUtils::get_capture_positions(game_state, !game_state.white_to_move);

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
                BitBoard attack_positions = GameUtils::get_capture_positions(game_state, !game_state.white_to_move);

                if (((castle_threw_positions & attack_positions) == 0) &&
                    GameUtils::is_empty(game_state.position, castle_threw_positions)) {
                    Move &move = moves.back_ref();
                }
            }
        }
    }
}

void GameUtils::apply_move(GameState &game_state, Move move) {
    /*
    bool is_white_piece = piece_index < PIECES_PER_PLAYER;
    if (is_white_piece) {
        game_state.position.get_white_bit_board() ^= (game_state.position.get_piece_bit_board(piece_index) | position);
    } else {
        game_state.position.get_black_bit_board() ^= (game_state.position.get_piece_bit_board(piece_index) | position);
    }
    game_state.position.get_piece_bit_board(piece_index) = position;
    game_state.white_to_move = !game_state.white_to_move;
    game_state.white_king_in_check = false;
    game_state.black_king_in_check = false;

    // Removed a captured piece from the board
    BitBoard *opp_positions;
    int32_t *opp_codes;
    if (is_white_piece) {
        opp_positions = game_state.position.piece_positions + PIECES_PER_PLAYER;
        opp_codes = game_state.position.piece_codes + PIECES_PER_PLAYER;
    } else {
        opp_positions = game_state.position.piece_positions;
        opp_codes = game_state.position.piece_codes;
    }

    for (int i = 0; i < PIECES_PER_PLAYER; ++i) {
        if (opp_positions[i] == position) {
            opp_positions[i] = 0;
            opp_codes[i] = PieceCodes::EMPTY;
            if (is_white_piece) {
                game_state.position.get_black_bit_board() ^= position;
            } else {
                game_state.position.get_white_bit_board() ^= position;
            }
            break;
        }
    }

    // Clear En Passant flag
    game_state.pawn_ep = -128;

    // Check for Rook or King moves
    if (piece_index == 15) {
        game_state.white_king_moved = true;
    } else if (piece_index == 15 + PIECES_PER_PLAYER) {
        game_state.black_king_moved = true;
    } else if (piece_index == 12) {
        game_state.white_rook_1_moved = true;
    } else if (piece_index == 12 + PIECES_PER_PLAYER) {
        game_state.black_rook_1_moved = true;
    } else if (piece_index == 13) {
        game_state.white_rook_2_moved = true;
    } else if (piece_index == 13 + PIECES_PER_PLAYER) {
        game_state.black_rook_2_moved = true;
    }

    // Check if King is in check
    game_state.position.get_white_threaten() = GameUtils::get_capture_positions(game_state, true);
    game_state.position.get_black_threaten() = GameUtils::get_capture_positions(game_state, false);

    game_state.white_king_in_check = (game_state.position.get_black_threaten() & game_state.position.piece_positions[15]) != 0;
    game_state.black_king_in_check = (game_state.position.get_white_threaten() & game_state.position.piece_positions[15 + PIECES_PER_PLAYER]) != 0;
    */
}

BitBoard GameUtils::get_captures_in_direction(const GameState &game_state, BitBoard bit_board, int32_t vertical, int32_t horizontal) {
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

BitBoard GameUtils::get_capture_positions(const GameState &game_state, Color color) {
    BitBoard capturable_positions = 0;
    capturable_positions |= GameUtils::get_pawn_capture_positions(game_state, color);
    capturable_positions |= GameUtils::get_knight_capture_positions(game_state, color);
    capturable_positions |= GameUtils::get_bishop_capture_positions(game_state, color);
    capturable_positions |= GameUtils::get_rook_capture_positions(game_state, color);
    capturable_positions |= GameUtils::get_queen_capture_positions(game_state, color);
    capturable_positions |= GameUtils::get_king_capture_positions(game_state, color);
    return capturable_positions;
}

BitBoard GameUtils::get_pawn_capture_positions(const GameState &game_state, Color color) {
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

BitBoard GameUtils::get_knight_capture_positions(const GameState &game_state, Color color) {
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

BitBoard GameUtils::get_bishop_capture_positions(const GameState &game_state, Color color) {
    BitBoard bishop_position = game_state.position.get_piece_color_bit_board(PieceCodes::KNIGHT, color);
    int64_t capturable_positions = 0;
    if (bishop_position) {
        capturable_positions |= GameUtils::get_captures_in_direction(game_state, bishop_position, 1, 1);
        capturable_positions |= GameUtils::get_captures_in_direction(game_state, bishop_position, -1, 1);
        capturable_positions |= GameUtils::get_captures_in_direction(game_state, bishop_position, 1, -1);
        capturable_positions |= GameUtils::get_captures_in_direction(game_state, bishop_position, -1, -1);
    }

    return capturable_positions;
}

BitBoard GameUtils::get_rook_capture_positions(const GameState &game_state, Color color) {
    int64_t capturable_positions = 0;
    if (game_state.position.get_piece_bit_board(PieceCodes::ROOK)) {
        capturable_positions |= GameUtils::get_captures_in_direction(game_state, PieceCodes::ROOK, 1, 0);
        capturable_positions |= GameUtils::get_captures_in_direction(game_state, PieceCodes::ROOK, -1, 0);
        capturable_positions |= GameUtils::get_captures_in_direction(game_state, PieceCodes::ROOK, 0, -1);
        capturable_positions |= GameUtils::get_captures_in_direction(game_state, PieceCodes::ROOK, 0, 1);
    }
    return capturable_positions;
}
BitBoard GameUtils::get_queen_capture_positions(const GameState &game_state, Color color) {
    int64_t capturable_positions = 0;

    if (game_state.position.get_piece_bit_board(PieceCodes::QUEEN)) {
        /* Diagonal */
        capturable_positions |= GameUtils::get_captures_in_direction(game_state, PieceCodes::QUEEN, 1, 1);
        capturable_positions |= GameUtils::get_captures_in_direction(game_state, PieceCodes::QUEEN, -1, 1);
        capturable_positions |= GameUtils::get_captures_in_direction(game_state, PieceCodes::QUEEN, 1, -1);
        capturable_positions |= GameUtils::get_captures_in_direction(game_state, PieceCodes::QUEEN, -1, -1);

        /* Vertical/Horizontal */
        capturable_positions |= GameUtils::get_captures_in_direction(game_state, PieceCodes::QUEEN, 1, 0);
        capturable_positions |= GameUtils::get_captures_in_direction(game_state, PieceCodes::QUEEN, -1, 0);
        capturable_positions |= GameUtils::get_captures_in_direction(game_state, PieceCodes::QUEEN, 0, -1);
        capturable_positions |= GameUtils::get_captures_in_direction(game_state, PieceCodes::QUEEN, 0, 1);
    }

    return capturable_positions;
}
BitBoard GameUtils::get_king_capture_positions(const GameState &game_state, Color color) {
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

bool GameUtils::is_piece_in_row(BitBoard position, int32_t row) {
    const BitBoard row_mask = 0xFFULL << (row * 8);
    return (position & row_mask) != 0;
}

bool GameUtils::is_piece_in_col(BitBoard position, int32_t col) {
    const BitBoard col_mask = 0x0101010101010101ULL << col;
    return (position & col_mask) != 0;
}

bool GameUtils::is_piece_in_top_row(BitBoard position) {
    return GameUtils::is_piece_in_row(position, 7);
}

bool GameUtils::is_piece_in_top_2_row(BitBoard position) {
    return GameUtils::is_piece_in_row(position, 7) || GameUtils::is_piece_in_row(position, 6);
}

bool GameUtils::is_piece_in_bottom_row(BitBoard position) {
    return GameUtils::is_piece_in_row(position, 0);
}

bool GameUtils::is_piece_in_bottom_2_row(BitBoard position) {
    return GameUtils::is_piece_in_row(position, 0) || GameUtils::is_piece_in_row(position, 1);
}

bool GameUtils::is_piece_in_left_col(BitBoard position) {
    return GameUtils::is_piece_in_col(position, 0);
}

bool GameUtils::is_piece_in_left_2_col(BitBoard position) {
    return GameUtils::is_piece_in_col(position, 0) || GameUtils::is_piece_in_col(position, 1);
}

bool GameUtils::is_piece_in_right_col(BitBoard position) {
    return GameUtils::is_piece_in_col(position, 7);
}

bool GameUtils::is_piece_in_right_2_col(BitBoard position) {
    return GameUtils::is_piece_in_col(position, 7) || GameUtils::is_piece_in_col(position, 6);
}

int32_t GameUtils::perform_user_move(GameState &game_state) {
    bool need_input = true;

    while (need_input) {
        printf("Select piece: ");
        std::string input = "";
        std::getline(std::cin, input);

        BitBoard selected_col;
        BitBoard selected_row;
        int32_t piece_index = -1;

        if (input.size() == 2) {
            char column_name = input[0];
            char row_name = input[1];

            selected_col = column_name - 'a';
            selected_row = row_name - '1';

            BitBoard curr_position = GameUtils::move(0x1ULL, selected_row, selected_col);

            for (uint64_t i = 0; i < PIECES_PER_PLAYER; ++i) {
                BitBoard position = game_state.position.get_piece_bit_board(i);
                if (curr_position == position) {
                    piece_index = i;
                    break;
                }
            }

            if (piece_index == -1) {
                printf("Invalid selection. No piece found.\n");
                continue;
            }
        } else if (input.compare("exit") == 0) {
            return -1;
        } else {
            printf("Invalid input.\n");
            continue;
        }

        printf("Select destination: ");
        std::getline(std::cin, input);

        BitBoard dest_col;
        BitBoard dest_row;

        if (input.size() == 2) {
            char column_name = input[0];
            char row_name = input[1];

            dest_col = column_name - 'a';
            dest_row = row_name - '1';
        } else {
            printf("Invalid input.\n");
            continue;
        }

        BitBoard next_position = GameUtils::move(0x1ULL, dest_row, dest_col);

        Moves moves;
        GameUtils::get_moves(game_state, moves);

        for (size_t i = 0; i < moves.size(); ++i) {
            Move &move = moves[i];
            /* TODO(EMU): Wrong, need to convert next_position to move */
            if (move == next_position) {
                GameUtils::apply_move(game_state, next_position);
                need_input = false;
                break;
            }
        }

        if (need_input) {
            printf("Invalid input, that piece cannot move there.\n");
        } else {
            break;
        }
    }

    return 1;
}

Move GameUtils::get_best_move(const GameState &game_state) {
    Moves moves;
    GameUtils::get_moves(game_state, moves);

    int32_t best_heuristic = game_state.white_to_move ? PieceValues::NEG_INFINITY : PieceValues::POS_INFINITY;
    Move best_move;

    for (size_t i = 0; i < moves.size(); ++i) {
        Move &move = moves[i];

        int32_t heuristic = GameUtils::alpha_beta_pruning_search(game_state, 6, PieceValues::NEG_INFINITY, PieceValues::POS_INFINITY, !game_state.white_to_move);
        if ((game_state.white_to_move && best_heuristic < heuristic) ||
            (!game_state.white_to_move && best_heuristic > heuristic)) {
            best_heuristic = heuristic;
            best_move = move;
        }
    }
    return best_move;
}

int32_t GameUtils::alpha_beta_pruning_search(const GameState &game_state, int32_t ply_depth, int32_t alpha, int32_t beta, bool max_white) {
    if (ply_depth == 0) {
        return GameUtils::get_position_heuristic(game_state);
    }

    Moves moves;
    GameUtils::get_moves(game_state, moves);

    if (max_white) {
        int32_t best_heuristic = PieceValues::NEG_INFINITY;
        for (size_t i = 0; i < moves.size(); ++i) {
            Move &move = moves[i];
            best_heuristic = std::max(best_heuristic, GameUtils::alpha_beta_pruning_search(game_state, ply_depth - 1, alpha, beta, false));
            if (best_heuristic >= beta) {
                break;
            }
            alpha = std::max(alpha, best_heuristic);
        }
        return best_heuristic;
    } else {
        int32_t best_heuristic = PieceValues::POS_INFINITY;
        for (size_t i = 0; i < moves.size(); ++i) {
            Move &move = moves[i];
            best_heuristic = std::min(best_heuristic, GameUtils::alpha_beta_pruning_search(game_state, ply_depth - 1, alpha, beta, true));
            if (best_heuristic <= alpha) {
                break;
            }
            beta = std::min(beta, best_heuristic);
        }
        return best_heuristic;
    }
}

int32_t GameUtils::get_position_heuristic(const GameState &game_state) {
    int32_t heuristic = 0;

    /* Sum piece values */
    for (uint64_t i = 0; i < PIECES_PER_PLAYER; ++i) {
        switch (game_state.position.get_piece_bit_board(i)) {
            case PieceCodes::PAWN: {
                heuristic += PieceValues::PAWN;
                break;
            }
            case PieceCodes::KNIGHT: {
                heuristic += PieceValues::KNIGHT;
                break;
            }
            case PieceCodes::BISHOP: {
                heuristic += PieceValues::BISHOP;
                break;
            }
            case PieceCodes::ROOK: {
                heuristic += PieceValues::ROOK;
                break;
            }
            case PieceCodes::QUEEN: {
                heuristic += PieceValues::QUEEN;
                break;
            }
            case PieceCodes::KING: {
                heuristic += PieceValues::KING;
                break;
            }
            case PieceCodes::NONE:
            default: {
            }
        }
    }

    for (uint64_t i = PIECES_PER_PLAYER; i < (PIECES_PER_PLAYER * 2); ++i) {
        switch (game_state.position.get_piece_bit_board(i)) {
            case PieceCodes::PAWN: {
                heuristic -= PieceValues::PAWN;
                break;
            }
            case PieceCodes::KNIGHT: {
                heuristic -= PieceValues::KNIGHT;
                break;
            }
            case PieceCodes::BISHOP: {
                heuristic -= PieceValues::BISHOP;
                break;
            }
            case PieceCodes::ROOK: {
                heuristic -= PieceValues::ROOK;
                break;
            }
            case PieceCodes::QUEEN: {
                heuristic -= PieceValues::QUEEN;
                break;
            }
            case PieceCodes::KING: {
                heuristic -= PieceValues::KING;
                break;
            }
            case PieceCodes::NONE:
            default: {
            }
        }
    }

    /* Put the King in check */
    if (game_state.black_king_in_check) {
        heuristic += PieceValues::PAWN / 2;
    }

    if (game_state.white_king_in_check) {
        heuristic -= PieceValues::PAWN / 2;
    }

    /* Bad check for Castling */
    if (game_state.black_king_moved && !(game_state.black_rook_1_moved || game_state.black_rook_2_moved)) {
        heuristic += PieceValues::PAWN / 2;
    }

    if (game_state.white_king_moved && !(game_state.white_rook_1_moved || game_state.white_rook_2_moved)) {
        heuristic -= PieceValues::PAWN / 2;
    }

    return heuristic;
}
