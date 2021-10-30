#include "UCIUtils.hpp"

#include <assert.h>

#include <functional>
#include <iostream>
#include <thread>
#include <unordered_map>

#include "GameUtils.hpp"
#include "MoveGeneration.hpp"
#include "MoveSearch.hpp"
#include "StringUtils.hpp"

auto UCIUtils::process_input_command(GameState& game_state) -> int32_t {
    std::string line;
    std::getline(std::cin, line);
    auto line_split = StringUtils::split(line);
    assert(line_split.size() > 0);

    auto command = line_split[0];
    int32_t rv = 1;

    const std::unordered_map<std::string, std::function<void()>> command_map = {
        {"uci", []() {
             UCIUtils::send_id();
             UCIUtils::send_option();
             UCIUtils::send_uci_ok();
         }},
        {"debug", []() {}},
        {"isready", []() { UCIUtils::send_ready_ok(); }},
        {"setoption", []() {}},
        {"register", []() {}},
        {"ucinewgame", [&game_state]() { GameUtils::init_standard(game_state); }},
        {"position", [&line_split, &game_state]() {if (line_split[1].compare("startpos") == 0) {
            GameUtils::init_standard(game_state);
        }
        for (size_t i = 3; i < line_split.size(); ++i) {
            std::string move_str = line_split[i];
            if (!UCIUtils::process_user_move(game_state, move_str)) {
                printf("info string Cannot process move %s\n", move_str.c_str());
            }
        } }},
        {"go", [&game_state]() { UCIUtils::send_ai_move(game_state); }},
        {"stop", []() {}},
        {"ponderhit", []() {}},
        {"quit", [&rv]() { rv = 0; }},
    };

    if (command_map.contains(command)) {
        auto command_func = command_map.find(command)->second;
        command_func();
    }

    return rv;
}

auto UCIUtils::process_user_move(GameState& game_state, const std::string& move_str) -> int32_t {
    int32_t rv = 1;

    // Check string length, min 4 characters required for a move
    if (move_str.size() < 4) {
        rv = 0;
        return rv;
    }

    int32_t source_col = move_str[0] - 'a';
    int32_t source_row = move_str[1] - '1';
    int32_t dest_col = move_str[2] - 'a';
    int32_t dest_row = move_str[3] - '1';

    auto invalid_input_range = [](int32_t num) { return (num < 0 || num > 7); };

    if (invalid_input_range(source_col) ||
        invalid_input_range(source_row) ||
        invalid_input_range(dest_col) ||
        invalid_input_range(dest_row)) {
        rv = 0;
        return rv;
    }

    auto input_to_square_num = [](int32_t row, int32_t col) { return row * 8 + col; };

    Move input_move(input_to_square_num(source_row, source_col),
                    input_to_square_num(dest_row, dest_col));

    // Check if moves promotes a pawn
    PieceCode promotion_piece_code = PieceCodes::NONE;
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

    Moves moves;
    MoveGeneration::get_moves(game_state, moves);

    // TODO(EMU): No moves, should return value to indicate?
    if (moves.size() == 0) {
        return rv;
    }

    bool legal_move = false;

    std::any_of(moves.begin(), moves.end(), [&legal_move, input_move](Move move) {
        if (move == input_move) {
            legal_move = true;
            // TODO(EMU): Apply the move here
            return false;
        } else {
            return true;
        }
    });

    if (!legal_move) {
        rv = 0;
        return rv;
    }

    return rv;
}

auto UCIUtils::send_id() -> void {
    printf("id name ChessEngine\n");
    printf("id author Evan Unmann\n");
}

auto UCIUtils::send_uci_ok() -> void {
    printf("uciok\n");
}

auto UCIUtils::send_ready_ok() -> void {
    printf("readyok\n");
}

auto UCIUtils::send_best_move(const std::string& move_str) -> void {
    printf("bestmove %s\n", move_str.c_str());
}
auto UCIUtils::send_copyright_protection() -> void {
    printf("copyrightprotection checking\n");
    printf("copyrightprotection ok\n");
}
auto UCIUtils::send_registration() -> void {
    printf("registration checking\n");
    printf("registration ok\n");
}
auto UCIUtils::send_info() -> void {
}

auto UCIUtils::send_option() -> void {
}

auto UCIUtils::send_ai_move(GameState& game_state) -> void {
    Move best_move = MoveSearch::get_best_move(game_state);

    int32_t piece_index = 0;

    /*
    for (int i = 0; i < PIECES_PER_PLAYER * 2; ++i) {
        if (game_state.position.get_piece_bit_board(i) != best_move.position.get_piece_bit_board(i) &&
            best_move.position.get_piece_bit_board(i) != PieceCodes::NONE) {
            piece_index = i;
            break;
        }
    }

    std::string src_tile = GameUtils::get_tile_name(game_state.position.get_piece_bit_board(piece_index));
    src_tile += GameUtils::get_tile_name(best_move.position.get_piece_bit_board(piece_index));

    // TODO(EMU): Promotions

    game_state = thread_state.best_move;
    UCIUtils::send_best_move(src_tile);
    */
}