#include "GameUtils.hpp"

#include <assert.h>
#include <stdio.h>

#include <bit>
#include <iostream>
#include <string>

#include "Definitions.hpp"
#include "MoveGeneration.hpp"

auto GameUtils::square_to_bit_board(const Square square) -> BitBoard {
    return 0b1ULL << square;
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

auto GameUtils::get_row(const BitBoard bit_board) -> int32_t {
    BitBoard row_mask = BitBoards::ROW_1;

    for (int i = 0; i < 8; ++i) {
        if ((bit_board & row_mask) != 0) {
            return i;
        }
        row_mask = row_mask << 8;
    }

    return -1;
}

auto GameUtils::get_col(const BitBoard bit_board) -> int32_t {
    BitBoard col_mask = BitBoards::COL_A;

    for (int i = 0; i < 8; ++i) {
        if ((bit_board & col_mask) != 0) {
            return i;
        }
        col_mask = col_mask << 1;
    }

    return -1;
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

auto GameUtils::do_bit_boards_overlap(const BitBoard bit_board_1, const BitBoard bit_board_2) -> bool {
    return (bit_board_1 & bit_board_2) != 0;
}

auto GameUtils::is_piece_in_row(const BitBoard bit_board, const int32_t row) -> bool {
    const BitBoard row_mask = BitBoards::ROW_1 << (row * 8);
    return GameUtils::do_bit_boards_overlap(row_mask, bit_board);
}

auto GameUtils::is_piece_in_col(const BitBoard bit_board, const int32_t col) -> bool {
    const BitBoard col_mask = BitBoards::COL_A << col;
    return GameUtils::do_bit_boards_overlap(col_mask, bit_board);
}

auto GameUtils::is_piece_in_top_row(const BitBoard bit_board) -> bool {
    return GameUtils::do_bit_boards_overlap(BitBoards::ROW_8, bit_board);
}

auto GameUtils::is_piece_in_top_2_row(const BitBoard bit_board) -> bool {
    return GameUtils::do_bit_boards_overlap(BitBoards::ROW_8 | BitBoards::ROW_7, bit_board);
}

auto GameUtils::is_piece_in_bottom_row(const BitBoard bit_board) -> bool {
    return GameUtils::do_bit_boards_overlap(BitBoards::ROW_1, bit_board);
}

auto GameUtils::is_piece_in_bottom_2_row(const BitBoard bit_board) -> bool {
    return GameUtils::do_bit_boards_overlap(BitBoards::ROW_1 | BitBoards::ROW_2, bit_board);
}

auto GameUtils::is_piece_in_left_col(const BitBoard bit_board) -> bool {
    return GameUtils::do_bit_boards_overlap(BitBoards::COL_A, bit_board);
}

auto GameUtils::is_piece_in_left_2_col(const BitBoard bit_board) -> bool {
    return GameUtils::do_bit_boards_overlap(BitBoards::COL_A | BitBoards::COL_B, bit_board);
}

auto GameUtils::is_piece_in_right_col(const BitBoard bit_board) -> bool {
    return GameUtils::do_bit_boards_overlap(BitBoards::COL_H, bit_board);
}

auto GameUtils::is_piece_in_right_2_col(const BitBoard bit_board) -> bool {
    return GameUtils::do_bit_boards_overlap(BitBoards::COL_H | BitBoards::COL_G, bit_board);
}

auto GameUtils::perform_user_move(GameState &game_state) -> int32_t {
    bool need_input = true;

    while (need_input) {
        printf("Select piece: ");
        auto input = GameUtils::get_user_input();

        BitBoard selected_col;
        BitBoard selected_row;

        if (input.size() == 2) {
            char column_name = input[0];
            char row_name = input[1];

            selected_col = column_name - 'a';
            selected_row = row_name - '1';

            const BitBoard selected_position_bit_board = GameUtils::shift_bit_board(0b1ULL, selected_row, selected_col);

            if (!game_state.position.is_occupied(selected_position_bit_board)) {
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
        input = GameUtils::get_user_input();

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

        BitBoard next_position = GameUtils::shift_bit_board(0b1ULL, dest_row, dest_col);

        Moves moves;
        Color color_to_move = Colors::bool_to_color(game_state.white_to_move);
        if (color_to_move == Colors::WHITE) {
            MoveGeneration::get_moves<Colors::WHITE>(game_state, moves);
        } else {
            MoveGeneration::get_moves<Colors::BLACK>(game_state, moves);
        }

        for (auto &move : moves) {
            printf("Move: %s\n", move.to_string().c_str());

            if (move.get_destination_bit_board() == next_position) {
                GameState check = game_state;
                check.apply_move(move);
                if (check.is_legal) {
                    game_state = check;
                    need_input = false;
                }
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

    std::any_of(legal_moves.begin(), legal_moves.end(), [&game_state, &input_move_legal, move](Move legal_move) {
        if (move == legal_move) {
            input_move_legal = true;
            game_state.apply_move(move);
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
        // TODO(EMU): This might cause problems. It might think this is a valid move
        return Move();
    }

    auto source_square = GameUtils::square_name_to_square(move_str);
    auto destintion_square = GameUtils::square_name_to_square(move_str.substr(2, 2));

    Move move(source_square, destintion_square);

    // Check if moves promotes a pawn
    if (move_str.size() == 5) {
        PieceCode promotion_piece_code = PieceCodes::NUM;
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
        move.set_promotion(promotion_piece_code);
    }

    return move;
}

auto GameUtils::bit_board_to_square(const BitBoard bit_board) -> Square {
    for (int i = 0; i < sizeof(BitBoard) * 8; i++) {
        BitBoard test = 0b1ULL << i;
        if ((test & bit_board) != 0) {
            return i;
        }
    }
    return -1;
}

auto GameUtils::for_each_set_square(const BitBoard bit_board, const std::function<void(Square square)> &func) -> void {
    BitBoard temp_bit_board = bit_board;
    while (true) {
        Square square = GameUtils::bit_board_to_square(temp_bit_board);
        if (square == -1) {
            break;
        }
        func(square);
        temp_bit_board &= ~GameUtils::square_to_bit_board(square);
    };
}

auto GameUtils::for_each_bit_board(const BitBoard bit_board, const std::function<void(BitBoard bit_board)> &func) -> void {
    BitBoard temp_bit_board = bit_board;
    while (true) {
        Square square = GameUtils::bit_board_to_square(temp_bit_board);
        if (square == -1) {
            break;
        }
        BitBoard single_bit_board = GameUtils::square_to_bit_board(square);
        func(single_bit_board);
        temp_bit_board &= ~single_bit_board;
    };
}
