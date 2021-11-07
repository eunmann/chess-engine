#include "MoveGeneration.hpp"

#include "GameUtils.hpp"

auto MoveGeneration::get_moves_in_direction(const GameState &game_state, BitBoard bit_board, int32_t vertical, int32_t horizontal, Moves &moves, Color color) -> void {
    Square source_square = GameUtils::bit_board_to_square(bit_board);

    for (int i = 0; i < 7; ++i) {
        if ((GameUtils::is_piece_in_top_row(bit_board) && vertical > 0) ||
            (GameUtils::is_piece_in_bottom_row(bit_board) && vertical < 0) ||
            (GameUtils::is_piece_in_right_col(bit_board) && horizontal > 0) ||
            (GameUtils::is_piece_in_left_col(bit_board) && horizontal < 0)) {
            break;
        }

        bit_board = GameUtils::shift_bit_board(bit_board, vertical, horizontal);

        if (game_state.position.is_empty(bit_board)) {
            moves.push_back(Move(source_square, GameUtils::bit_board_to_square(bit_board)));

        } else if ((color == Colors::WHITE && ((game_state.position.get_white_bit_board() & bit_board) == 0)) ||
                   (color == Colors::BLACK && ((game_state.position.get_black_bit_board() & bit_board) == 0))) {
            moves.push_back(Move(source_square, GameUtils::bit_board_to_square(bit_board)));
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
    const BitBoard pawns_bit_board = game_state.position.get_piece_color_bit_board(PieceCodes::PAWN, color);

    GameUtils::for_each_bit_board(pawns_bit_board, [&game_state, &moves, pawn_dir, color](const BitBoard pawn_bit_board) {
        Square source_square = GameUtils::bit_board_to_square(pawn_bit_board);
        // Up
        BitBoard next_pawn_bit_board = GameUtils::shift_bit_board(pawn_bit_board, 1 * pawn_dir, 0);
        if (game_state.position.is_empty(next_pawn_bit_board)) {
            // Promotion
            if ((color == Colors::WHITE && GameUtils::is_piece_in_top_row(next_pawn_bit_board)) ||
                (color == Colors::BLACK && GameUtils::is_piece_in_bottom_row(next_pawn_bit_board))) {
                for (int32_t i = PieceCodes::KNIGHT; i <= PieceCodes::QUEEN; ++i) {
                    moves.push_back(0);  // TODO(EMU): PLACEHOLDER VALUES
                }
            } else {
                moves.push_back(Move(source_square, GameUtils::bit_board_to_square(next_pawn_bit_board)));
            }

            // Up 2 first-move
            if ((color == Colors::WHITE && GameUtils::is_piece_in_row(pawn_bit_board, 1)) ||
                (color == Colors::BLACK && GameUtils::is_piece_in_row(pawn_bit_board, 6))) {
                next_pawn_bit_board = GameUtils::shift_bit_board(pawn_bit_board, 2 * pawn_dir, 0);
                if (game_state.position.is_empty(next_pawn_bit_board)) {
                    moves.push_back(Move(source_square, GameUtils::bit_board_to_square(next_pawn_bit_board)));
                    // TODO(EMU): Set enpassant-able flag
                }
            }
        }

        // Capture Left and Right
        BitBoard pawn_bit_board_left_capture = GameUtils::is_piece_in_left_col(pawn_bit_board) ? 0 : GameUtils::shift_bit_board(pawn_bit_board, 1 * pawn_dir, -1);
        BitBoard pawn_bit_board_right_capture = GameUtils::is_piece_in_right_col(pawn_bit_board) ? 0 : GameUtils::shift_bit_board(pawn_bit_board, 1 * pawn_dir, 1);

        if ((color == Colors::WHITE && ((game_state.position.get_black_bit_board() & pawn_bit_board_left_capture) != 0)) ||
            (color == Colors::BLACK && ((game_state.position.get_white_bit_board() & pawn_bit_board_left_capture) != 0))) {
            if ((color == Colors::WHITE && GameUtils::is_piece_in_top_row(pawn_bit_board_left_capture)) ||
                (color == Colors::BLACK && GameUtils::is_piece_in_bottom_row(pawn_bit_board_left_capture))) {
                for (int32_t i = PieceCodes::KNIGHT; i <= PieceCodes::QUEEN; ++i) {
                    moves.push_back(Move(source_square, GameUtils::bit_board_to_square(pawn_bit_board_left_capture)));
                }
            } else {
                moves.push_back(Move(source_square, GameUtils::bit_board_to_square(pawn_bit_board_left_capture)));
            }
        }

        if ((color == Colors::WHITE && ((game_state.position.get_black_bit_board() & pawn_bit_board_right_capture) != 0)) ||
            (color == Colors::BLACK && ((game_state.position.get_white_bit_board() & pawn_bit_board_right_capture) != 0))) {
            if ((color == Colors::WHITE && GameUtils::is_piece_in_top_row(pawn_bit_board_right_capture)) ||
                (color == Colors::BLACK && GameUtils::is_piece_in_bottom_row(pawn_bit_board_right_capture))) {
                for (int32_t i = PieceCodes::KNIGHT; i <= PieceCodes::QUEEN; ++i) {
                    moves.push_back(Move(source_square, GameUtils::bit_board_to_square(pawn_bit_board_right_capture)));
                }
            } else {
                moves.push_back(Move(source_square, GameUtils::bit_board_to_square(pawn_bit_board_right_capture)));
            }
        }

        // En Passant
        int32_t row, col;
        GameUtils::get_row_col(pawn_bit_board, row, col);
        const int32_t left_ep = color == Colors::WHITE ? col - 1 + PIECES_PER_PLAYER : col - 1 - PIECES_PER_PLAYER;
        const int32_t right_ep = color == Colors::WHITE ? col + 1 + PIECES_PER_PLAYER : col + 1 - PIECES_PER_PLAYER;
        const int32_t en_row = color == Colors::WHITE ? 4 : 3;

        if (game_state.pawn_ep == left_ep && GameUtils::is_piece_in_row(pawn_bit_board, en_row)) {
            moves.push_back(0);  // TODO(EMU): PLACEHOLDER VALUES
        }

        if (game_state.pawn_ep == right_ep && GameUtils::is_piece_in_row(pawn_bit_board, en_row)) {
            moves.push_back(0);  // TODO(EMU): PLACEHOLDER VALUES
        }
    });
}

auto MoveGeneration::get_knight_moves(const GameState &game_state, Moves &moves, Color color) -> void {
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

auto MoveGeneration::get_bishop_moves(const GameState &game_state, Moves &moves, Color color) -> void {
    BitBoard bishops_bit_board = game_state.position.get_piece_color_bit_board(PieceCodes::BISHOP, color);

    GameUtils::for_each_bit_board(bishops_bit_board, [&game_state, &moves, color](const BitBoard bishop_bit_board) {
        MoveGeneration::get_moves_in_direction(game_state, bishop_bit_board, 1, 1, moves, color);
        MoveGeneration::get_moves_in_direction(game_state, bishop_bit_board, -1, 1, moves, color);
        MoveGeneration::get_moves_in_direction(game_state, bishop_bit_board, 1, -1, moves, color);
        MoveGeneration::get_moves_in_direction(game_state, bishop_bit_board, -1, -1, moves, color);
    });
}

auto MoveGeneration::get_rook_moves(const GameState &game_state, Moves &moves, Color color) -> void {
    BitBoard rooks_bit_board = game_state.position.get_piece_color_bit_board(PieceCodes::ROOK, color);

    GameUtils::for_each_bit_board(rooks_bit_board, [&game_state, &moves, color](const BitBoard rook_bit_board) {
        MoveGeneration::get_moves_in_direction(game_state, rook_bit_board, 1, 0, moves, color);
        MoveGeneration::get_moves_in_direction(game_state, rook_bit_board, -1, 0, moves, color);
        MoveGeneration::get_moves_in_direction(game_state, rook_bit_board, 0, -1, moves, color);
        MoveGeneration::get_moves_in_direction(game_state, rook_bit_board, 0, 1, moves, color);
    });
}

auto MoveGeneration::get_queen_moves(const GameState &game_state, Moves &moves, Color color) -> void {
    BitBoard queens_bit_board = game_state.position.get_piece_color_bit_board(PieceCodes::QUEEN, color);
    GameUtils::for_each_bit_board(queens_bit_board, [&game_state, &moves, color](BitBoard queen_bit_board) {
        // Diagonal
        MoveGeneration::get_moves_in_direction(game_state, queen_bit_board, 1, 1, moves, color);
        MoveGeneration::get_moves_in_direction(game_state, queen_bit_board, -1, 1, moves, color);
        MoveGeneration::get_moves_in_direction(game_state, queen_bit_board, 1, -1, moves, color);
        MoveGeneration::get_moves_in_direction(game_state, queen_bit_board, -1, -1, moves, color);

        // Horizontal and vertical
        MoveGeneration::get_moves_in_direction(game_state, queen_bit_board, 1, 0, moves, color);
        MoveGeneration::get_moves_in_direction(game_state, queen_bit_board, -1, 0, moves, color);
        MoveGeneration::get_moves_in_direction(game_state, queen_bit_board, 0, -1, moves, color);
        MoveGeneration::get_moves_in_direction(game_state, queen_bit_board, 0, 1, moves, color);
    });
}

auto MoveGeneration::get_king_moves(const GameState &game_state, Moves &moves, Color color) -> void {
    BitBoard kings_bit_board = game_state.position.get_piece_color_bit_board(PieceCodes::KING, color);

    GameUtils::for_each_bit_board(kings_bit_board, [&game_state, &moves, color](BitBoard king_bit_board) {
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

        // Castling
        if ((color == Colors::WHITE && !game_state.white_king_moved && !game_state.white_king_in_check) ||
            (color == Colors::BLACK && !game_state.black_king_moved && !game_state.black_king_in_check)) {
            // Queen Side
            if ((color == Colors::WHITE && !game_state.white_rook_1_moved) || (color == Colors::BLACK && !game_state.black_rook_1_moved)) {
                BitBoard next_king_bit_board = GameUtils::shift_bit_board<0, -2>(king_bit_board);
                BitBoard next_rook_position = GameUtils::shift_bit_board<0, 1>(next_king_bit_board);
                BitBoard empty_position = GameUtils::shift_bit_board<0, -1>(next_king_bit_board);

                BitBoard castle_threw_positions = next_rook_position | next_king_bit_board;
                BitBoard attack_positions = MoveGeneration::get_capture_positions(game_state.position, !game_state.white_to_move);

                if (((castle_threw_positions & attack_positions) == 0) &&
                    game_state.position.is_empty(castle_threw_positions | empty_position)) {
                    //moves.push_back(0);  // TODO(EMU): PLACEHOLDER VALUES
                }
            }

            // King Side
            if ((color == Colors::WHITE && !game_state.white_rook_2_moved) || (color == Colors::BLACK && !game_state.black_rook_2_moved)) {
                BitBoard next_king_bit_board = GameUtils::shift_bit_board<0, 2>(king_bit_board);
                BitBoard next_rook_position = GameUtils::shift_bit_board<0, -1>(next_king_bit_board);

                BitBoard castle_threw_positions = next_rook_position | next_king_bit_board;
                BitBoard attack_positions = MoveGeneration::get_capture_positions(game_state.position, !game_state.white_to_move);

                if (((castle_threw_positions & attack_positions) == 0) &&
                    game_state.position.is_empty(castle_threw_positions)) {
                    //moves.push_back(0);  // TODO(EMU): PLACEHOLDER VALUES
                }
            }
        }
    });
}

auto MoveGeneration::get_captures_in_direction(const Position &position, BitBoard bit_board, int32_t vertical, int32_t horizontal) -> BitBoard {
    BitBoard capturable_bit_board = BitBoards::EMPTY;

    for (int i = 0; i < 8; ++i) {
        if ((GameUtils::is_piece_in_top_row(bit_board) && vertical > 0) ||
            (GameUtils::is_piece_in_bottom_row(bit_board) && vertical < 0) ||
            (GameUtils::is_piece_in_right_col(bit_board) && horizontal > 0) ||
            (GameUtils::is_piece_in_left_col(bit_board) && horizontal < 0)) {
            break;
        }

        bit_board = GameUtils::shift_bit_board(bit_board, vertical, horizontal);
        capturable_bit_board |= bit_board;

        if (position.is_occupied(bit_board)) {
            break;
        }
    }

    return capturable_bit_board;
}

auto MoveGeneration::get_capture_positions(const Position &position, Color color) -> BitBoard {
    BitBoard capturable_bit_board = BitBoards::EMPTY;
    capturable_bit_board |= MoveGeneration::get_pawn_capture_positions(position, color);
    capturable_bit_board |= MoveGeneration::get_knight_capture_positions(position, color);
    capturable_bit_board |= MoveGeneration::get_bishop_capture_positions(position, color);
    capturable_bit_board |= MoveGeneration::get_rook_capture_positions(position, color);
    capturable_bit_board |= MoveGeneration::get_queen_capture_positions(position, color);
    capturable_bit_board |= MoveGeneration::get_king_capture_positions(position, color);
    return capturable_bit_board;
}

auto MoveGeneration::get_pawn_capture_positions(const Position &position, Color color) -> BitBoard {
    const int64_t pawn_dir = color == Colors::WHITE ? 1 : -1;
    BitBoard capturable_bit_board = BitBoards::EMPTY;
    BitBoard pawns_bit_board = position.get_piece_color_bit_board(PieceCodes::PAWN, color);
    GameUtils::for_each_bit_board(pawns_bit_board, [&capturable_bit_board, pawn_dir](BitBoard pawn_bit_board) {
        // Capture Left and Right
        BitBoard pawn_bit_board_left_capture = GameUtils::is_piece_in_left_col(pawn_bit_board) ? 0 : GameUtils::shift_bit_board(pawn_bit_board, 1 * pawn_dir, -1);
        BitBoard pawn_bit_board_right_capture = GameUtils::is_piece_in_right_col(pawn_bit_board) ? 0 : GameUtils::shift_bit_board(pawn_bit_board, 1 * pawn_dir, 1);
        capturable_bit_board |= pawn_bit_board_left_capture | pawn_bit_board_right_capture;
    });
    return capturable_bit_board;
}

auto MoveGeneration::get_knight_capture_positions(const Position &position, Color color) -> BitBoard {
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

auto MoveGeneration::get_bishop_capture_positions(const Position &position, Color color) -> BitBoard {
    BitBoard capturable_bit_board = BitBoards::EMPTY;
    BitBoard bishops_bit_board = position.get_piece_color_bit_board(PieceCodes::BISHOP, color);
    GameUtils::for_each_bit_board(bishops_bit_board, [&capturable_bit_board, &position](BitBoard bishop_bit_board) {
        capturable_bit_board |= MoveGeneration::get_captures_in_direction(position, bishop_bit_board, 1, 1);
        capturable_bit_board |= MoveGeneration::get_captures_in_direction(position, bishop_bit_board, -1, 1);
        capturable_bit_board |= MoveGeneration::get_captures_in_direction(position, bishop_bit_board, 1, -1);
        capturable_bit_board |= MoveGeneration::get_captures_in_direction(position, bishop_bit_board, -1, -1);
    });
    return capturable_bit_board;
}

auto MoveGeneration::get_rook_capture_positions(const Position &position, Color color) -> BitBoard {
    BitBoard capturable_bit_board = BitBoards::EMPTY;
    BitBoard rooks_bit_board = position.get_piece_color_bit_board(PieceCodes::ROOK, color);
    GameUtils::for_each_bit_board(rooks_bit_board, [&capturable_bit_board, &position](BitBoard rook_bit_board) {
        capturable_bit_board |= MoveGeneration::get_captures_in_direction(position, rook_bit_board, 1, 0);
        capturable_bit_board |= MoveGeneration::get_captures_in_direction(position, rook_bit_board, -1, 0);
        capturable_bit_board |= MoveGeneration::get_captures_in_direction(position, rook_bit_board, 0, -1);
        capturable_bit_board |= MoveGeneration::get_captures_in_direction(position, rook_bit_board, 0, 1);
    });
    return capturable_bit_board;
}
auto MoveGeneration::get_queen_capture_positions(const Position &position, Color color) -> BitBoard {
    BitBoard capturable_bit_board = BitBoards::EMPTY;
    BitBoard queens_bit_board = position.get_piece_color_bit_board(PieceCodes::QUEEN, color);
    GameUtils::for_each_bit_board(queens_bit_board, [&capturable_bit_board, &position](BitBoard queen_bit_board) {
        // Diagonal
        capturable_bit_board |= MoveGeneration::get_captures_in_direction(position, queen_bit_board, 1, 1);
        capturable_bit_board |= MoveGeneration::get_captures_in_direction(position, queen_bit_board, -1, 1);
        capturable_bit_board |= MoveGeneration::get_captures_in_direction(position, queen_bit_board, 1, -1);
        capturable_bit_board |= MoveGeneration::get_captures_in_direction(position, queen_bit_board, -1, -1);

        // Vertical/Horizontal
        capturable_bit_board |= MoveGeneration::get_captures_in_direction(position, queen_bit_board, 1, 0);
        capturable_bit_board |= MoveGeneration::get_captures_in_direction(position, queen_bit_board, -1, 0);
        capturable_bit_board |= MoveGeneration::get_captures_in_direction(position, queen_bit_board, 0, -1);
        capturable_bit_board |= MoveGeneration::get_captures_in_direction(position, queen_bit_board, 0, 1);
    });
    return capturable_bit_board;
}
auto MoveGeneration::get_king_capture_positions(const Position &position, Color color) -> BitBoard {
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