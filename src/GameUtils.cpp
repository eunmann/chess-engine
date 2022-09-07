#include "GameUtils.hpp"

#include "Assert.hpp"
#include <cstdio>
#include <intrin.h>

#include <algorithm>
#include <bit>
#include <iostream>
#include <string>

#include "CommonHeaders.hpp"
#include "Move.hpp"
#include "MoveGeneration.hpp"
#include "Moves.hpp"
#include "BitBoardUtils.hpp"

namespace GameUtils {

    auto perform_user_move(GameState &game_state) noexcept -> bool {
        bool need_input = true;

        while (need_input) {
            printf("Input move: ");
            auto input = get_user_input();

            if (input.size() < 4) {
                printf("Invalid input.\n");
                continue;
            }

            if (input == "exit") {
                return false;
            }

            Moves moves;
            Color color_to_move = Colors::bool_to_color(game_state.is_white_to_move());
            if (color_to_move == Colors::WHITE) {
                MoveGeneration::get_moves<Colors::WHITE>(game_state, moves);
            } else {
                MoveGeneration::get_moves<Colors::BLACK>(game_state, moves);
            }
            printf("Found %llu psuedo-legal moves for %s\n", moves.size(),
                   color_to_move == Colors::WHITE ? "white" : "black");

            Moves legal_moves;
            for (auto &move: moves) {
                if (GameUtils::is_move_legal(game_state, move, color_to_move)) {
                    legal_moves.push_back(move);
                } else {
                    printf("Illegal Move: %s\n", move.to_string().c_str());
                }
            }

            printf("Found %llu legal moves for %s\n", legal_moves.size(),
                   color_to_move == Colors::WHITE ? "white" : "black");

            for (auto &legal_move: legal_moves) {
                printf("Legal Move: %s\n", legal_move.to_string().c_str());
                if (legal_move.to_string() == input) {
                    if (color_to_move == Colors::WHITE) {
                        game_state.apply_move<Colors::WHITE>(legal_move);
                    } else {
                        game_state.apply_move<Colors::BLACK>(legal_move);
                    }
                    need_input = false;
                    break;
                }
            }

            if (need_input) {
                printf("Invalid input.\n");
            } else {
                break;
            }
        }

        return true;
    }

    auto get_user_input() noexcept -> std::string {
        std::string input;
        std::getline(std::cin, input);
        return input;
    }

    auto process_user_move(GameState &game_state, const std::string &move_str) noexcept -> bool {

        Moves moves;
        Color color_to_move = Colors::bool_to_color(game_state.is_white_to_move());
        if (color_to_move == Colors::WHITE) {
            MoveGeneration::get_moves<Colors::WHITE>(game_state, moves);
        } else {
            MoveGeneration::get_moves<Colors::BLACK>(game_state, moves);
        }

        for (auto &move: moves) {
            auto is_legal = GameUtils::is_move_legal(game_state, move, color_to_move);
            if (is_legal && move.to_string() == move_str) {
                if (color_to_move == Colors::WHITE) {
                    game_state.apply_move<Colors::WHITE>(move);
                } else {
                    game_state.apply_move<Colors::BLACK>(move);
                }
                return true;
            }
        }

        printf("info Unable to process move %s\n", move_str.c_str());
        return false;
    }

    auto square_name_to_square(const std::string &square_name) noexcept -> Square {
        // Check string length, min 2 characters required for a move
        if (square_name.size() < 2) {
            return Squares::INVALID;
        }

        int32_t col_index = square_name[0] - 'a';
        int32_t row_index = square_name[1] - '1';

        auto invalid_input_range = [](int32_t num) { return (num < 0 || num > 7); };

        if (invalid_input_range(col_index) || invalid_input_range(row_index)) {
            return Squares::INVALID;
        }

        return Square(row_index * 8 + col_index);
    }

    auto move_str_to_move(const std::string &move_str) noexcept -> Move {
        // Check string length, min 4 characters required for a move
        if (move_str.size() < 4) {
            // TODO(EMU): This might cause problems. It might think this is a valid move
            return {};
        }

        auto source_square = square_name_to_square(move_str);
        auto destination_square = square_name_to_square(move_str.substr(2, 2));

        Move move(source_square, destination_square);

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
                    break;
                }
            }
            move.set_promotion(promotion_piece_code);
        }

        return move;
    }

    auto is_move_legal(const GameState &game_state, const Move move, const Color color_to_move) -> bool {
        GameState check = game_state;
        if (color_to_move == Colors::WHITE) {
            check.apply_move<Colors::WHITE>(move);
        } else {
            check.apply_move<Colors::BLACK>(move);
        }
        return check.is_legal();
    }
}

