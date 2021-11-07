#include "UCIUtils.hpp"

#include <assert.h>

#include <functional>
#include <unordered_map>

#include "GameUtils.hpp"
#include "MoveSearch.hpp"
#include "StringUtils.hpp"

auto UCIUtils::process_input_command(GameState& game_state, const std::string& command) -> int32_t {
    auto line_split = StringUtils::split(command);
    assert(line_split.size() > 0);

    auto command_action = line_split[0];
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
        {"position", [&line_split, &game_state]() {
            if (line_split[1].compare("startpos") == 0) {
                GameUtils::init_standard(game_state);
            }
            for (size_t i = 3; i < line_split.size(); ++i) {
                std::string move_str = line_split[i];
                Move move = GameUtils::move_str_to_move(move_str);
                if (!GameUtils::process_user_move(game_state, move)) {
                    printf("info string cannot process move %s\n", move_str.c_str());
            }
        } }},
        {"go", [&game_state]() {
             Move best_move = MoveSearch::get_best_move(game_state);
             // TODO(EMU): Convert the move to a string
             std::string src_tile;
             UCIUtils::send_best_move(src_tile);
         }},
        {"stop", []() {}},
        {"ponderhit", []() {}},
        {"quit", [&rv]() { rv = 0; }},
    };

    if (command_map.contains(command_action)) {
        auto command_func = command_map.find(command_action)->second;
        command_func();
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