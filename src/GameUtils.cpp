#include "GameUtils.hpp"

#include <stdio.h>

#include <iostream>
#include <string>

#include "Definitions.hpp"
#include "omp.h"

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

bool GameUtils::is_valid(const GameState &game_state, BitBoard position, int32_t piece_index) {
    bool is_white_piece = piece_index < PIECES_PER_PLAYER;

    if ((is_white_piece && ((game_state.position.get_color_bit_board(Colors::WHITE) & position) != 0)) ||
        (!is_white_piece && ((game_state.position.get_color_bit_board(Colors::BLACK) & position) != 0))) {
        return false;
    }

    /* TODO(EMU): I don't like this copy here */
    GameState temp = game_state;
    GameUtils::apply_move(temp, position, piece_index);

    if (is_white_piece) {
        return (temp.position.get_piece_bit_board(PieceCodes::KING) & temp.position.get_white_bit_board() & temp.position.get_black_threaten()) == 0;
    } else {
        return (temp.position.get_piece_bit_board(PieceCodes::KING) & temp.position.get_black_bit_board() & temp.position.get_white_threaten()) == 0;
    }
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

void GameUtils::get_moves_in_direction(const GameState &game_state, int32_t piece_index, int32_t vertical, int32_t horizontal, Moves &moves) {
    BitBoard position = game_state.position.piece_positions[piece_index];
    bool is_white_piece = piece_index < PIECES_PER_PLAYER;

    for (int i = 0; i < 7; ++i) {
        if ((GameUtils::is_piece_in_top_row(position) && vertical > 0) ||
            (GameUtils::is_piece_in_bottom_row(position) && vertical < 0) ||
            (GameUtils::is_piece_in_right_col(position) && horizontal > 0) ||
            (GameUtils::is_piece_in_left_col(position) && horizontal < 0)) {
            break;
        }

        position = GameUtils::move(position, vertical, horizontal);

        if (!GameUtils::is_valid(game_state, position, piece_index)) {
            break;
        }

        if (GameUtils::is_empty(game_state.position, position)) {
            GameState &move = moves.back_ref();
            move = game_state;
            GameUtils::apply_move(move, position, piece_index);
        } else if ((is_white_piece && ((game_state.position.white_pieces & position) == 0)) ||
                   (!is_white_piece && ((game_state.position.black_pieces & position) == 0))) {
            GameState &move = moves.back_ref();
            move = game_state;
            GameUtils::apply_move(move, position, piece_index);
            break;
        } else {
            break;
        }
    }
}

void GameUtils::get_moves(const GameState &game_state, Moves &moves) {
    int start = game_state.white_to_move ? 0 : PIECES_PER_PLAYER;
    int end = game_state.white_to_move ? PIECES_PER_PLAYER : PIECES_PER_PLAYER * 2;

    for (int i = start; i < end; ++i) {
        GameUtils::get_piece_moves(game_state, i, moves);
    }
}

void GameUtils::get_piece_moves(const GameState &game_state, int32_t piece_index, Moves &moves) {
    int32_t code = game_state.position.piece_codes[piece_index];
    switch (code) {
        case PieceCodes::PAWN: {
            GameUtils::get_pawn_moves(game_state, piece_index, moves);
            break;
        }
        case PieceCodes::KNIGHT: {
            GameUtils::get_knight_moves(game_state, piece_index, moves);
            break;
        }
        case PieceCodes::BISHOP: {
            GameUtils::get_bishop_moves(game_state, piece_index, moves);
            break;
        }
        case PieceCodes::ROOK: {
            GameUtils::get_rook_moves(game_state, piece_index, moves);
            break;
        }
        case PieceCodes::QUEEN: {
            GameUtils::get_queen_moves(game_state, piece_index, moves);
            break;
        }
        case PieceCodes::KING: {
            GameUtils::get_king_moves(game_state, piece_index, moves);
            break;
        }
        case 0:
        default: {
        }
    }
}

void GameUtils::get_pawn_moves(const GameState &game_state, int32_t piece_index, Moves &moves) {
    const bool is_white_piece = piece_index < PIECES_PER_PLAYER;
    const int64_t pawn_dir = is_white_piece ? 1 : -1;
    const BitBoard pawn_position = game_state.position.piece_positions[piece_index];

    if (pawn_position) {
        /* Up */
        BitBoard next_pawn_position = GameUtils::move(pawn_position, 1 * pawn_dir, 0);
        if (GameUtils::is_empty(game_state.position, next_pawn_position) &&
            GameUtils::is_valid(game_state, next_pawn_position, piece_index)) {
            if ((is_white_piece && GameUtils::is_piece_in_top_row(next_pawn_position)) ||
                (!is_white_piece && GameUtils::is_piece_in_bottom_row(next_pawn_position))) {
                for (int32_t i = PieceCodes::KNIGHT; i <= PieceCodes::QUEEN; ++i) {
                    GameState &move = moves.back_ref();
                    move = game_state;
                    move.position.piece_codes[piece_index] = i;
                    GameUtils::apply_move(move, next_pawn_position, piece_index);
                }
            } else {
                GameState &move = moves.back_ref();
                move = game_state;
                GameUtils::apply_move(move, next_pawn_position, piece_index);
            }

            /* Up 2 first-move */
            if ((is_white_piece && GameUtils::is_piece_in_row(pawn_position, 1)) ||
                (!is_white_piece && GameUtils::is_piece_in_row(pawn_position, 6))) {
                next_pawn_position = GameUtils::move(pawn_position, 2 * pawn_dir, 0);
                if (GameUtils::is_empty(game_state.position, next_pawn_position) &&
                    GameUtils::is_valid(game_state, next_pawn_position, piece_index)) {
                    GameState &move = moves.back_ref();
                    move = game_state;
                    GameUtils::apply_move(move, next_pawn_position, piece_index);
                    move.pawn_ep = piece_index;
                }
            }
        }

        /* Capture Left and Right */
        BitBoard pawn_position_left_capture = GameUtils::is_piece_in_left_col(pawn_position) ? 0 : GameUtils::move(pawn_position, 1 * pawn_dir, -1);
        bool left_capture_valid = GameUtils::is_valid(game_state, pawn_position_left_capture, piece_index);
        BitBoard pawn_position_right_capture = GameUtils::is_piece_in_right_col(pawn_position) ? 0 : GameUtils::move(pawn_position, 1 * pawn_dir, 1);
        bool right_capture_valid = GameUtils::is_valid(game_state, pawn_position_right_capture, piece_index);

        if (left_capture_valid &&
            ((is_white_piece && ((game_state.position.black_pieces & pawn_position_left_capture) != 0)) ||
             (!is_white_piece && ((game_state.position.white_pieces & pawn_position_left_capture) != 0)))) {
            if ((is_white_piece && GameUtils::is_piece_in_top_row(pawn_position_left_capture)) ||
                (!is_white_piece && GameUtils::is_piece_in_bottom_row(pawn_position_left_capture))) {
                for (int32_t i = PieceCodes::KNIGHT; i <= PieceCodes::QUEEN; ++i) {
                    GameState &move = moves.back_ref();
                    move = game_state;
                    move.position.piece_codes[piece_index] = i;
                    GameUtils::apply_move(move, pawn_position_left_capture, piece_index);
                }
            } else {
                GameState &move = moves.back_ref();
                move = game_state;
                GameUtils::apply_move(move, pawn_position_left_capture, piece_index);
            }
        }

        if (right_capture_valid &&
            ((is_white_piece && ((game_state.position.black_pieces & pawn_position_right_capture) != 0)) ||
             (!is_white_piece && ((game_state.position.white_pieces & pawn_position_right_capture) != 0)))) {
            if ((is_white_piece && GameUtils::is_piece_in_top_row(pawn_position_right_capture)) ||
                (!is_white_piece && GameUtils::is_piece_in_bottom_row(pawn_position_right_capture))) {
                for (int32_t i = PieceCodes::KNIGHT; i <= PieceCodes::QUEEN; ++i) {
                    GameState &move = moves.back_ref();
                    move = game_state;
                    move.position.piece_codes[piece_index] = i;
                    GameUtils::apply_move(move, pawn_position_right_capture, piece_index);
                }
            } else {
                GameState &move = moves.back_ref();
                move = game_state;
                GameUtils::apply_move(move, pawn_position_right_capture, piece_index);
            }
        }

        /* En Passant */
        int32_t row, col;
        GameUtils::get_row_col(pawn_position, row, col);
        const int32_t left_ep = is_white_piece ? col - 1 + PIECES_PER_PLAYER : col - 1 - PIECES_PER_PLAYER;
        const int32_t right_ep = is_white_piece ? col + 1 + PIECES_PER_PLAYER : col + 1 - PIECES_PER_PLAYER;
        const int32_t en_row = is_white_piece ? 4 : 3;

        if (game_state.pawn_ep == left_ep && GameUtils::is_piece_in_row(pawn_position, en_row)) {
            GameState &move = moves.back_ref();
            move = game_state;
            GameUtils::apply_move(move, GameUtils::move(pawn_position_left_capture, -1 * pawn_dir, 0), piece_index);
            move.position.piece_positions[piece_index] = pawn_position_left_capture;
        }

        if (game_state.pawn_ep == right_ep && GameUtils::is_piece_in_row(pawn_position, en_row)) {
            GameState &move = moves.back_ref();
            move = game_state;
            GameUtils::apply_move(move, GameUtils::move(pawn_position_right_capture, -1 * pawn_dir, 0), piece_index);
            move.position.piece_positions[piece_index] = pawn_position_right_capture;
        }
    }
}

void GameUtils::get_knight_moves(const GameState &game_state, int32_t piece_index, Moves &moves) {
    BitBoard knight_position = game_state.position.piece_positions[piece_index];

    if (knight_position) {
        if (!(GameUtils::is_piece_in_top_2_row(knight_position) || GameUtils::is_piece_in_left_col(knight_position))) {
            BitBoard next_knight_position = GameUtils::move(knight_position, 2, -1);
            if (GameUtils::is_valid(game_state, next_knight_position, piece_index)) {
                GameState &move = moves.back_ref();
                move = game_state;
                GameUtils::apply_move(move, next_knight_position, piece_index);
            }
        }
        if (!(GameUtils::is_piece_in_top_row(knight_position) || GameUtils::is_piece_in_left_2_col(knight_position))) {
            BitBoard next_knight_position = GameUtils::move(knight_position, 1, -2);
            if (GameUtils::is_valid(game_state, next_knight_position, piece_index)) {
                GameState &move = moves.back_ref();
                move = game_state;
                GameUtils::apply_move(move, next_knight_position, piece_index);
            }
        }

        if (!(GameUtils::is_piece_in_bottom_row(knight_position) || GameUtils::is_piece_in_left_2_col(knight_position))) {
            BitBoard next_knight_position = GameUtils::move(knight_position, -1, -2);
            if (GameUtils::is_valid(game_state, next_knight_position, piece_index)) {
                GameState &move = moves.back_ref();
                move = game_state;
                GameUtils::apply_move(move, next_knight_position, piece_index);
            }
        }

        if (!(GameUtils::is_piece_in_bottom_2_row(knight_position) || GameUtils::is_piece_in_left_col(knight_position))) {
            BitBoard next_knight_position = GameUtils::move(knight_position, -2, -1);
            if (GameUtils::is_valid(game_state, next_knight_position, piece_index)) {
                GameState &move = moves.back_ref();
                move = game_state;
                GameUtils::apply_move(move, next_knight_position, piece_index);
            }
        }

        if (!(GameUtils::is_piece_in_bottom_2_row(knight_position) || GameUtils::is_piece_in_right_col(knight_position))) {
            BitBoard next_knight_position = GameUtils::move(knight_position, -2, 1);
            if (GameUtils::is_valid(game_state, next_knight_position, piece_index)) {
                GameState &move = moves.back_ref();
                move = game_state;
                GameUtils::apply_move(move, next_knight_position, piece_index);
            }
        }

        if (!(GameUtils::is_piece_in_bottom_row(knight_position) || GameUtils::is_piece_in_right_2_col(knight_position))) {
            BitBoard next_knight_position = GameUtils::move(knight_position, -1, 2);
            if (GameUtils::is_valid(game_state, next_knight_position, piece_index)) {
                GameState &move = moves.back_ref();
                move = game_state;
                GameUtils::apply_move(move, next_knight_position, piece_index);
            }
        }

        if (!(GameUtils::is_piece_in_top_row(knight_position) || GameUtils::is_piece_in_right_2_col(knight_position))) {
            BitBoard next_knight_position = GameUtils::move(knight_position, 1, 2);
            if (GameUtils::is_valid(game_state, next_knight_position, piece_index)) {
                GameState &move = moves.back_ref();
                move = game_state;
                GameUtils::apply_move(move, next_knight_position, piece_index);
            }
        }

        if (!(GameUtils::is_piece_in_top_2_row(knight_position) || GameUtils::is_piece_in_right_col(knight_position))) {
            BitBoard next_knight_position = GameUtils::move(knight_position, 2, 1);
            if (GameUtils::is_valid(game_state, next_knight_position, piece_index)) {
                GameState &move = moves.back_ref();
                move = game_state;
                GameUtils::apply_move(move, next_knight_position, piece_index);
            }
        }
    }
}

void GameUtils::get_bishop_moves(const GameState &game_state, int32_t piece_index, Moves &moves) {
    if (game_state.position.piece_positions[piece_index]) {
        GameUtils::get_moves_in_direction(game_state, piece_index, 1, 1, moves);
        GameUtils::get_moves_in_direction(game_state, piece_index, -1, 1, moves);
        GameUtils::get_moves_in_direction(game_state, piece_index, 1, -1, moves);
        GameUtils::get_moves_in_direction(game_state, piece_index, -1, -1, moves);
    }
}

void GameUtils::get_rook_moves(const GameState &game_state, int32_t piece_index, Moves &moves) {
    if (game_state.position.piece_positions[piece_index]) {
        GameUtils::get_moves_in_direction(game_state, piece_index, 1, 0, moves);
        GameUtils::get_moves_in_direction(game_state, piece_index, -1, 0, moves);
        GameUtils::get_moves_in_direction(game_state, piece_index, 0, -1, moves);
        GameUtils::get_moves_in_direction(game_state, piece_index, 0, 1, moves);
    }
}

void GameUtils::get_queen_moves(const GameState &game_state, int32_t piece_index, Moves &moves) {
    GameUtils::get_bishop_moves(game_state, piece_index, moves);
    GameUtils::get_rook_moves(game_state, piece_index, moves);
}

void GameUtils::get_king_moves(const GameState &game_state, int32_t piece_index, Moves &moves) {
    BitBoard king_position = game_state.position.piece_positions[piece_index];
    if (king_position) {
        /* Up */
        if (!GameUtils::is_piece_in_top_row(king_position)) {
            BitBoard next_king_position = GameUtils::move(king_position, 1, 0);
            if (GameUtils::is_valid(game_state, next_king_position, piece_index)) {
                GameState &move = moves.back_ref();
                move = game_state;
                GameUtils::apply_move(move, next_king_position, piece_index);
            }
        }

        /* Down */
        if (!GameUtils::is_piece_in_bottom_row(king_position)) {
            BitBoard next_king_position = GameUtils::move(king_position, -1, 0);
            if (GameUtils::is_valid(game_state, next_king_position, piece_index)) {
                GameState &move = moves.back_ref();
                move = game_state;
                GameUtils::apply_move(move, next_king_position, piece_index);
            }
        }

        /* Left */
        if (!GameUtils::is_piece_in_left_col(king_position)) {
            BitBoard next_king_position = GameUtils::move(king_position, 0, -1);
            if (GameUtils::is_valid(game_state, next_king_position, piece_index)) {
                GameState &move = moves.back_ref();
                move = game_state;
                GameUtils::apply_move(move, next_king_position, piece_index);
            }
        }

        /* Right */
        if (!GameUtils::is_piece_in_right_col(king_position)) {
            BitBoard next_king_position = GameUtils::move(king_position, 0, 1);
            if (GameUtils::is_valid(game_state, next_king_position, piece_index)) {
                GameState &move = moves.back_ref();
                move = game_state;
                GameUtils::apply_move(move, next_king_position, piece_index);
            }
        }

        /* Up-Right */
        if (!(GameUtils::is_piece_in_top_row(king_position) || GameUtils::is_piece_in_right_col(king_position))) {
            BitBoard next_king_position = GameUtils::move(king_position, 1, 1);
            if (GameUtils::is_valid(game_state, next_king_position, piece_index)) {
                GameState &move = moves.back_ref();
                move = game_state;
                GameUtils::apply_move(move, next_king_position, piece_index);
            }
        }

        /* Up-Left */
        if (!(GameUtils::is_piece_in_top_row(king_position) || GameUtils::is_piece_in_left_col(king_position))) {
            BitBoard next_king_position = GameUtils::move(king_position, 1, -1);
            if (GameUtils::is_valid(game_state, next_king_position, piece_index)) {
                GameState &move = moves.back_ref();
                move = game_state;
                GameUtils::apply_move(move, next_king_position, piece_index);
            }
        }

        /* Down-Right */
        if (!(GameUtils::is_piece_in_bottom_row(king_position) || GameUtils::is_piece_in_right_col(king_position))) {
            BitBoard next_king_position = GameUtils::move(king_position, -1, 1);
            if (GameUtils::is_valid(game_state, next_king_position, piece_index)) {
                GameState &move = moves.back_ref();
                move = game_state;
                GameUtils::apply_move(move, next_king_position, piece_index);
            }
        }

        /* Down-Left */
        if (!(GameUtils::is_piece_in_bottom_row(king_position) || GameUtils::is_piece_in_left_col(king_position))) {
            BitBoard next_king_position = GameUtils::move(king_position, -1, -1);
            if (GameUtils::is_valid(game_state, next_king_position, piece_index)) {
                GameState &move = moves.back_ref();
                move = game_state;
                GameUtils::apply_move(move, next_king_position, piece_index);
            }
        }

        /* Castling */
        bool is_white_piece = piece_index < PIECES_PER_PLAYER;
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
                    GameState &move = moves.back_ref();
                    move = game_state;
                    int32_t rook_piece_index = is_white_piece ? 12 : 12 + PIECES_PER_PLAYER;

                    if (is_white_piece) {
                        move.position.white_pieces ^= (move.position.piece_positions[rook_piece_index] | next_rook_position);
                    } else {
                        move.position.black_pieces ^= (move.position.piece_positions[rook_piece_index] | next_rook_position);
                    }
                    move.position.piece_positions[rook_piece_index] = next_rook_position;

                    if (is_white_piece) {
                        move.white_rook_1_moved = true;
                    } else {
                        move.black_rook_1_moved = true;
                    }

                    GameUtils::apply_move(move, next_king_position, piece_index);
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
                    GameState &move = moves.back_ref();
                    move = game_state;
                    int32_t rook_piece_index = is_white_piece ? 13 : 13 + PIECES_PER_PLAYER;

                    if (is_white_piece) {
                        move.position.white_pieces ^= (move.position.piece_positions[rook_piece_index] | next_rook_position);
                    } else {
                        move.position.black_pieces ^= (move.position.piece_positions[rook_piece_index] | next_rook_position);
                    }
                    move.position.piece_positions[rook_piece_index] = next_rook_position;
                    if (is_white_piece) {
                        move.white_rook_2_moved = true;
                    } else {
                        move.black_rook_2_moved = true;
                    }

                    GameUtils::apply_move(move, next_king_position, piece_index);
                }
            }
        }
    }
}

void GameUtils::apply_move(GameState &game_state, BitBoard position, int32_t piece_index) {
    bool is_white_piece = piece_index < PIECES_PER_PLAYER;
    if (is_white_piece) {
        game_state.position.white_pieces ^= (game_state.position.piece_positions[piece_index] | position);
    } else {
        game_state.position.black_pieces ^= (game_state.position.piece_positions[piece_index] | position);
    }
    game_state.position.piece_positions[piece_index] = position;
    game_state.white_to_move = !game_state.white_to_move;
    game_state.white_king_in_check = false;
    game_state.black_king_in_check = false;

    /* Removed a captured piece from the board */
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
                game_state.position.black_pieces ^= position;
            } else {
                game_state.position.white_pieces ^= position;
            }
            break;
        }
    }

    /* Clear En Passant flag */
    game_state.pawn_ep = -128;

    /* Check for Rook or King moves */
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

    /* Check if King is in check */
    game_state.position.white_threaten = GameUtils::get_capture_positions(game_state, true);
    game_state.position.black_threaten = GameUtils::get_capture_positions(game_state, false);

    game_state.white_king_in_check = (game_state.position.black_threaten & game_state.position.piece_positions[15]) != 0;
    game_state.black_king_in_check = (game_state.position.white_threaten & game_state.position.piece_positions[15 + PIECES_PER_PLAYER]) != 0;
}

BitBoard GameUtils::get_captures_in_direction(const GameState &game_state, int32_t piece_index, int32_t vertical, int32_t horizontal) {
    BitBoard position = game_state.position.piece_positions[piece_index];
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

BitBoard GameUtils::get_capture_positions(const GameState &game_state, bool white_pieces) {
    int s = white_pieces ? 0 : PIECES_PER_PLAYER;
    int e = white_pieces ? PIECES_PER_PLAYER : PIECES_PER_PLAYER * 2;
    BitBoard capturable_positions = 0;
    for (int i = s; i < e; ++i) {
        capturable_positions |= GameUtils::get_piece_capture_positions(game_state, i);
    }
    return capturable_positions;
}

BitBoard GameUtils::get_piece_capture_positions(const GameState &game_state, int32_t piece_index) {
    int32_t code = game_state.position.piece_codes[piece_index];
    switch (code) {
        case PieceCodes::PAWN: {
            return GameUtils::get_pawn_capture_positions(game_state, piece_index);
        }
        case PieceCodes::KNIGHT: {
            return GameUtils::get_knight_capture_positions(game_state, piece_index);
        }
        case PieceCodes::BISHOP: {
            return GameUtils::get_bishop_capture_positions(game_state, piece_index);
        }
        case PieceCodes::ROOK: {
            return GameUtils::get_rook_capture_positions(game_state, piece_index);
        }
        case PieceCodes::QUEEN: {
            return GameUtils::get_queen_capture_positions(game_state, piece_index);
        }
        case PieceCodes::KING: {
            return GameUtils::get_king_capture_positions(game_state, piece_index);
        }
        case 0:
        default: {
            return 0;
        }
    }
}

BitBoard GameUtils::get_pawn_capture_positions(const GameState &game_state, int32_t piece_index) {
    const bool is_white_piece = piece_index < PIECES_PER_PLAYER;
    const int64_t pawn_dir = is_white_piece ? 1 : -1;
    const BitBoard pawn_position = game_state.position.piece_positions[piece_index];
    BitBoard capturable_positions = 0;

    if (pawn_position) {
        /* Capture Left and Right */
        BitBoard pawn_position_left_capture = GameUtils::is_piece_in_left_col(pawn_position) ? 0 : GameUtils::move(pawn_position, 1 * pawn_dir, -1);
        BitBoard pawn_position_right_capture = GameUtils::is_piece_in_right_col(pawn_position) ? 0 : GameUtils::move(pawn_position, 1 * pawn_dir, 1);
        capturable_positions |= pawn_position_left_capture | pawn_position_right_capture;
    }

    return capturable_positions;
}

BitBoard GameUtils::get_knight_capture_positions(const GameState &game_state, int32_t piece_index) {
    BitBoard knight_position = game_state.position.piece_positions[piece_index];
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

BitBoard GameUtils::get_bishop_capture_positions(const GameState &game_state, int32_t piece_index) {
    int64_t capturable_positions = 0;
    if (game_state.position.piece_positions[piece_index]) {
        capturable_positions |= GameUtils::get_captures_in_direction(game_state, piece_index, 1, 1);
        capturable_positions |= GameUtils::get_captures_in_direction(game_state, piece_index, -1, 1);
        capturable_positions |= GameUtils::get_captures_in_direction(game_state, piece_index, 1, -1);
        capturable_positions |= GameUtils::get_captures_in_direction(game_state, piece_index, -1, -1);
    }

    return capturable_positions;
}

BitBoard GameUtils::get_rook_capture_positions(const GameState &game_state, int32_t piece_index) {
    int64_t capturable_positions = 0;
    if (game_state.position.piece_positions[piece_index]) {
        capturable_positions |= GameUtils::get_captures_in_direction(game_state, piece_index, 1, 0);
        capturable_positions |= GameUtils::get_captures_in_direction(game_state, piece_index, -1, 0);
        capturable_positions |= GameUtils::get_captures_in_direction(game_state, piece_index, 0, -1);
        capturable_positions |= GameUtils::get_captures_in_direction(game_state, piece_index, 0, 1);
    }
    return capturable_positions;
}
BitBoard GameUtils::get_queen_capture_positions(const GameState &game_state, int32_t piece_index) {
    int64_t capturable_positions = 0;

    capturable_positions |= GameUtils::get_bishop_capture_positions(game_state, piece_index);
    capturable_positions |= GameUtils::get_rook_capture_positions(game_state, piece_index);

    return capturable_positions;
}
BitBoard GameUtils::get_king_capture_positions(const GameState &game_state, int32_t piece_index) {
    BitBoard capturable_positions = 0;
    BitBoard position = game_state.position.piece_positions[piece_index];
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

            for (int32_t i = 0; i < PIECES_PER_PLAYER; ++i) {
                BitBoard position = game_state.position.piece_positions[i];
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
        GameUtils::get_piece_moves(game_state, piece_index, moves);

        for (size_t i = 0; i < moves.size(); ++i) {
            GameState &move = moves[i];
            if (move.position.piece_positions[piece_index] == next_position) {
                GameUtils::apply_move(game_state, next_position, piece_index);
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

void GameUtils::get_best_move(ThreadState &thread_state) {
    Moves moves;
    GameUtils::get_moves(thread_state.game_state, moves);

    int32_t best_heuristics[omp_get_max_threads()];
    ThreadState thread_states[omp_get_max_threads()];

    int32_t heuristic_init = thread_state.game_state.white_to_move ? PieceValues::NEG_INFINITY : PieceValues::POS_INFINITY;

    for (int i = 0; i < omp_get_max_threads(); ++i) {
        best_heuristics[i] = heuristic_init;
        thread_states[i] = thread_state;
    }

#pragma omp parallel for schedule(dynamic, 1)
    for (size_t i = 0; i < moves.size(); ++i) {
        GameState &move = moves[i];
        ThreadState &ts = thread_states[omp_get_thread_num()];

        int32_t heuristic = GameUtils::alpha_beta_pruning_search(ts, move, 6, PieceValues::NEG_INFINITY, PieceValues::POS_INFINITY, !ts.game_state.white_to_move);
        if ((ts.game_state.white_to_move && best_heuristics[omp_get_thread_num()] < heuristic) ||
            (!ts.game_state.white_to_move && best_heuristics[omp_get_thread_num()] > heuristic)) {
            best_heuristics[omp_get_thread_num()] = heuristic;
            ts.best_move = move;
        }
    }

    int32_t best_heuristic = heuristic_init;

    for (int i = 0; i < omp_get_max_threads(); ++i) {
        if ((thread_state.game_state.white_to_move && best_heuristic <= best_heuristics[i]) ||
            (!thread_state.game_state.white_to_move && best_heuristic >= best_heuristics[i])) {
            best_heuristic = best_heuristics[i];
            thread_state.best_move = thread_states[i].best_move;
        }
    }
}

int32_t GameUtils::alpha_beta_pruning_search(const ThreadState &thread_state, const GameState &game_state, int32_t ply_depth, int32_t alpha, int32_t beta, bool max_white) {
    if (!thread_state.should_search) {
        /* TODO(EMU): What should this value be? */
        return 0;
    }

    if (ply_depth == 0) {
        return GameUtils::get_position_heuristic(game_state);
    }

    Moves moves;
    GameUtils::get_moves(game_state, moves);

    if (max_white) {
        int32_t best_heuristic = PieceValues::NEG_INFINITY;
        for (size_t i = 0; i < moves.size(); ++i) {
            GameState &move = moves[i];
            best_heuristic = std::max(best_heuristic, GameUtils::alpha_beta_pruning_search(thread_state, move, ply_depth - 1, alpha, beta, false));
            if (best_heuristic >= beta) {
                break;
            }
            alpha = std::max(alpha, best_heuristic);
        }
        return best_heuristic;
    } else {
        int32_t best_heuristic = PieceValues::POS_INFINITY;
        for (size_t i = 0; i < moves.size(); ++i) {
            GameState &move = moves[i];
            best_heuristic = std::min(best_heuristic, GameUtils::alpha_beta_pruning_search(thread_state, move, ply_depth - 1, alpha, beta, true));
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
    for (int32_t i = 0; i < PIECES_PER_PLAYER; ++i) {
        switch (game_state.position.piece_codes[i]) {
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
            case PieceCodes::EMPTY:
            default: {
            }
        }
    }

    for (int32_t i = PIECES_PER_PLAYER; i < (PIECES_PER_PLAYER * 2); ++i) {
        switch (game_state.position.piece_codes[i]) {
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
            case PieceCodes::EMPTY:
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
