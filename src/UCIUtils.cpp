#include "UCIUtils.hpp"

#include <iostream>
#include <thread>

#include "GameUtils.hpp"
#include "StringUtils.hpp"

int32_t UCIUtils::process_input_command(GameState& game_state) {
    std::string line;
    std::getline(std::cin, line);
    std::vector<std::string> line_split = StringUtils::split(line);
    std::string command = line_split[0];
    int32_t rv = 1;

    if (command.compare("uci") == 0) {
        UCIUtils::send_id();
        UCIUtils::send_option();
        UCIUtils::send_uci_ok();
    } else if (command.compare("debug") == 0) {
    } else if (command.compare("isready") == 0) {
        UCIUtils::send_ready_ok();
    } else if (command.compare("setoption") == 0) {
    } else if (command.compare("register") == 0) {
    } else if (command.compare("ucinewgame") == 0) {
        GameUtils::init_standard(game_state);
    } else if (command.compare("position") == 0) {
        if (line_split[1].compare("startpos") == 0) {
            GameUtils::init_standard(game_state);
        }
        for (size_t i = 3; i < line_split.size(); ++i) {
            std::string move_str = line_split[i];
            if (!UCIUtils::process_user_move(game_state, move_str)) {
                printf("info string Cannot process move %s\n", move_str.c_str());
            }
        }
    } else if (command.compare("go") == 0) {
        UCIUtils::send_ai_move();
    } else if (command.compare("stop") == 0) {
    } else if (command.compare("ponderhit") == 0) {
    } else if (command.compare("quit") == 0) {
        rv = 0;
    }

    return rv;
}

int32_t UCIUtils::process_user_move(GameState& game_state, const std::string& move_str) {
    int32_t rv = 1;

    if (move_str.size() < 4) {
        rv = 0;
        return rv;
    }

    char column_name = move_str[0];
    char row_name = move_str[1];

    int32_t selected_col = column_name - 'a';
    int32_t selected_row = row_name - '1';

    if ((selected_col < 0 || selected_col >= 8) ||
        (selected_row < 0 || selected_row >= 8)) {
        rv = 0;
        return rv;
    }

    BitBoard curr_position = GameUtils::move(0x1ULL, selected_row, selected_col);
    int32_t piece_index = -1;

    for (int32_t i = 0; i < PIECES_PER_PLAYER * 2; ++i) {
        if (curr_position == game_state.position.get_piece_bit_board(i)) {
            piece_index = i;
            break;
        }
    }

    if (piece_index == -1) {
        rv = 0;
        return rv;
    }

    int32_t dest_col = move_str[2] - 'a';
    int32_t dest_row = move_str[3] - '1';

    if ((dest_col < 0 || dest_col >= 8) ||
        (dest_row < 0 || dest_row >= 8)) {
        rv = 0;
        return rv;
    }

    int32_t piece_code = game_state.position.get_piece_bit_board(piece_index);

    if (move_str.size() == 5) {
        switch (move_str[4]) {
            case 'n': {
                piece_code = PieceCodes::KNIGHT;
                break;
            }
            case 'b': {
                piece_code = PieceCodes::BISHOP;
                break;
            }
            case 'r': {
                piece_code = PieceCodes::ROOK;
                break;
            }
            case 'q': {
                piece_code = PieceCodes::QUEEN;
                break;
            }
            default: {
            }
        }
    }

    BitBoard next_position = GameUtils::move(0x1ULL, dest_row, dest_col);

    Moves moves;
    GameUtils::get_piece_moves(game_state, piece_index, moves);

    if (moves.size() == 0) {
        return rv;
    }

    bool legal_move = false;

    for (size_t i = 0; i < moves.size(); ++i) {
        Move& move = moves[i];
        if (move.position.get_piece_bit_board(piece_index) == next_position &&
            move.position.get_piece_bit_board(piece_index) == piece_code) {
            game_state = move;
            legal_move = true;
            break;
        }
    }

    if (!legal_move) {
        rv = 0;
        return rv;
    }

    return rv;
}

void UCIUtils::send_id() {
    printf("id name ChessEngine\n");
    printf("id author Evan Unmann\n");
}

void UCIUtils::send_uci_ok() {
    printf("uciok\n");
}

void UCIUtils::send_ready_ok() {
    printf("readyok\n");
}

void UCIUtils::send_best_move(const std::string& move_str) {
    printf("bestmove %s\n", move_str.c_str());
}
void UCIUtils::send_copyright_protection() {
    printf("copyrightprotection checking\n");
    printf("copyrightprotection ok\n");
}
void UCIUtils::send_registration() {
    printf("registration checking\n");
    printf("registration ok\n");
}
void UCIUtils::send_info() {
}

void UCIUtils::send_option() {
}

void UCIUtils::send_ai_move(GameState& game_state) {
    Move best_move = GameUtils::get_best_move(game_state);

    int32_t piece_index = 0;

    for (int i = 0; i < PIECES_PER_PLAYER * 2; ++i) {
        if (game_state.position.get_piece_bit_board(i) != best_move.position.get_piece_bit_board(i) &&
            best_move.position.get_piece_bit_board(i) != PieceCodes::NONE) {
            piece_index = i;
            break;
        }
    }

    std::string src_tile = GameUtils::get_tile_name(game_state.position.get_piece_bit_board(piece_index));
    src_tile += GameUtils::get_tile_name(best_move.position.get_piece_bit_board(piece_index));

    /* TODO(EMU): Promotions */

    game_state = thread_state.best_move;
    UCIUtils::send_best_move(src_tile);
}