#include "GameUtils.hpp"

#include <assert.h>
#include <bits/stdc++.h>
#include <stdio.h>

#include <iostream>
#include <string>

#include "Definitions.hpp"
#include "MoveGeneration.hpp"

auto GameUtils::square_to_bit_board(const Square square) -> BitBoard {
    return 0b1 << (square - 1);
}

auto GameUtils::print_position(const BitBoard bit_board) -> void {
    const BitBoard left_bit = 1ULL << 63;
    BitBoard temp_bit_board = bit_board;
    std::string output;
    output.reserve(64);

    for (int32_t j = 0; j < 8; ++j) {
        for (int32_t i = 0; i < 8; ++i) {
            if (left_bit & temp_bit_board) {
                output += '1';
            } else {
                output += '0';
            }
            temp_bit_board <<= 1;
        }
        output += '\n';
    }

    printf("%s", output.c_str());
}

auto GameUtils::shift_bit_board(const BitBoard bit_board, const int32_t vertical, const int32_t horizontal) -> BitBoard {
    assert(vertical > -8 && vertical < 8);
    assert(horizontal > -8 && horizontal < 8);

    int32_t shift = vertical * 8 + horizontal;
    if (shift >= 0) {
        return bit_board << shift;
    } else {
        return bit_board >> (shift * -1);
    }
}

auto GameUtils::get_row_col(const BitBoard bit_board, int32_t &row, int32_t &col) -> void {
    BitBoard row_mask = BitBoards::ROW_1;
    BitBoard col_mask = BitBoards::COL_A;

    row = -1;
    col = -1;

    for (int i = 0; i < 8; ++i) {
        if ((bit_board & row_mask) != 0) {
            row = i;
        }
        if ((bit_board & col_mask) != 0) {
            col = i;
        }
        row_mask = row_mask << 8;
        col_mask = col_mask << 1;
    }
}

auto GameUtils::get_tile_name(const BitBoard bit_board) -> std::string {
    int32_t row, col;
    GameUtils::get_row_col(bit_board, row, col);

    std::string name = "";
    name += (char)(col + 'a');
    name += (char)(row + '1');

    return name;
}

auto GameUtils::init_standard(GameState &game_state) -> void {
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

auto do_bit_boards_overlap(const BitBoard bit_board_1, const BitBoard bit_board_2) -> bool {
    return (bit_board_1 & bit_board_2) != 0;
}

auto GameUtils::is_piece_in_row(const BitBoard bit_board, const int32_t row) -> bool {
    const BitBoard row_mask = BitBoards::ROW_1 << (row * 8);
    return (bit_board & row_mask) != 0;
}

auto GameUtils::is_piece_in_col(const BitBoard bit_board, const int32_t col) -> bool {
    const BitBoard col_mask = BitBoards::COL_A << col;
    return (bit_board & col_mask) != 0;
}

auto GameUtils::is_piece_in_top_row(const BitBoard bit_board) -> bool {
    return GameUtils::is_piece_in_row(bit_board, 7);
}

auto GameUtils::is_piece_in_top_2_row(const BitBoard bit_board) -> bool {
    return GameUtils::is_piece_in_row(bit_board, 7) || GameUtils::is_piece_in_row(bit_board, 6);
}

auto GameUtils::is_piece_in_bottom_row(const BitBoard bit_board) -> bool {
    return GameUtils::is_piece_in_row(bit_board, 0);
}

auto GameUtils::is_piece_in_bottom_2_row(const BitBoard bit_board) -> bool {
    return GameUtils::is_piece_in_row(bit_board, 0) || GameUtils::is_piece_in_row(bit_board, 1);
}

auto GameUtils::is_piece_in_left_col(const BitBoard bit_board) -> bool {
    return GameUtils::is_piece_in_col(bit_board, 0);
}

auto GameUtils::is_piece_in_left_2_col(const BitBoard bit_board) -> bool {
    return GameUtils::is_piece_in_col(bit_board, 0) || GameUtils::is_piece_in_col(bit_board, 1);
}

auto GameUtils::is_piece_in_right_col(const BitBoard bit_board) -> bool {
    return GameUtils::is_piece_in_col(bit_board, 7);
}

auto GameUtils::is_piece_in_right_2_col(const BitBoard bit_board) -> bool {
    return GameUtils::is_piece_in_col(bit_board, 7) || GameUtils::is_piece_in_col(bit_board, 6);
}

auto GameUtils::perform_user_move(GameState &game_state) -> int32_t {
    bool need_input = true;

    while (need_input) {
        printf("Select piece: ");
        auto input = GameUtils::get_user_input();

        BitBoard selected_col;
        BitBoard selected_row;
        int32_t piece_index = -1;

        if (input.size() == 2) {
            char column_name = input[0];
            char row_name = input[1];

            selected_col = column_name - 'a';
            selected_row = row_name - '1';

            BitBoard curr_position = GameUtils::shift_bit_board(0b1, selected_row, selected_col);

            for (uint64_t i = 0; i < PIECES_PER_PLAYER; ++i) {
                BitBoard bit_board = game_state.position.get_piece_bit_board(i);
                if (curr_position == bit_board) {
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

        BitBoard next_position = GameUtils::shift_bit_board(0b1, dest_row, dest_col);

        Moves moves;
        Color color_to_move = Colors::bool_to_color(game_state.white_to_move);
        if (color_to_move == Colors::WHITE) {
            MoveGeneration::get_moves<Colors::WHITE>(game_state, moves);
        } else {
            MoveGeneration::get_moves<Colors::BLACK>(game_state, moves);
        }

        for (size_t i = 0; i < moves.size(); ++i) {
            Move &move = moves[i];
            // TODO(EMU): Wrong, need to convert next_position to move
            if (move.get_destination_bit_board() == next_position) {
                game_state.apply_move(move);
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

auto GameUtils::get_user_input() -> std::string {
    std::string input;
    std::getline(std::cin, input);
    return input;
}

auto GameUtils::process_user_move(GameState &game_state, const Move move) -> int32_t {
    int32_t rv = 1;

    Moves legal_moves;
    Color color_to_move = Colors::bool_to_color(game_state.white_to_move);
    if (color_to_move == Colors::WHITE) {
        MoveGeneration::get_moves<Colors::WHITE>(game_state, legal_moves);
    } else {
        MoveGeneration::get_moves<Colors::BLACK>(game_state, legal_moves);
    }

    // TODO(EMU): No moves, should return value to indicate?
    if (legal_moves.size() == 0) {
        return rv;
    }

    bool input_move_legal = false;

    std::any_of(legal_moves.begin(), legal_moves.end(), [&input_move_legal, move](Move legal_move) {
        if (move == legal_move) {
            input_move_legal = true;
            // TODO(EMU): Apply the move here
            return false;
        } else {
            return true;
        }
    });

    if (!input_move_legal) {
        rv = 0;
        return rv;
    }

    return rv;
}

auto GameUtils::square_name_to_square(const std::string &square_name) -> Square {
    // Check string length, min 2 characters required for a move
    if (square_name.size() < 2) {
        return 0;
    }

    int32_t col_index = square_name[0] - 'a';
    int32_t row_index = square_name[1] - '1';

    auto invalid_input_range = [](int32_t num) { return (num < 0 || num > 7); };

    if (invalid_input_range(col_index) ||
        invalid_input_range(row_index)) {
        return 0;
    }

    return row_index * 8 + col_index;
}

auto GameUtils::move_str_to_move(const std::string &move_str) -> Move {
    // Check string length, min 4 characters required for a move
    if (move_str.size() < 4) {
        return Move();
    }

    auto source_square = GameUtils::square_name_to_square(move_str);
    auto destintion_square = GameUtils::square_name_to_square(move_str.substr(2, 2));

    // TODO(EMU): Deal with promotion
    // Check if moves promotes a pawn
    PieceCode promotion_piece_code = PieceCodes::NUM;
    if (move_str.size() == 5) {
        switch (move_str[4]) {
            case 'n': {
                promotion_piece_code = PieceCodes::KNIGHT;
                break;
            }
            case 'b': {
                promotion_piece_code = PieceCodes::BISHOP;
                break;
            }
            case 'r': {
                promotion_piece_code = PieceCodes::ROOK;
                break;
            }
            case 'q': {
                promotion_piece_code = PieceCodes::QUEEN;
                break;
            }
            default: {
            }
        }
    }

    return Move(source_square, destintion_square);
}

auto GameUtils::bit_board_to_square(const BitBoard bit_board) -> Square {
    return ffsl(bit_board) - 1;
}

auto GameUtils::for_each_set_bit(const BitBoard bit_board, const std::function<void(Square square)> &func) -> void {
    BitBoard temp_bit_board = bit_board;
    while (int32_t index = ffsl(temp_bit_board)) {
        func(index);
        temp_bit_board ^= GameUtils::square_to_bit_board(index);
    };
}

auto GameUtils::for_each_bit_board(const BitBoard bit_board, const std::function<void(BitBoard bit_board)> &func) -> void {
    BitBoard temp_bit_board = bit_board;
    while (int32_t index = ffsl(temp_bit_board)) {
        BitBoard single_bit_board = GameUtils::square_to_bit_board(index);
        func(single_bit_board);
        temp_bit_board ^= single_bit_board;
    };
}
