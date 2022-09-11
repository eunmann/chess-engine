#include "UCIUtils.hpp"

#include <functional>
#include <unordered_map>

#include "GameUtils.hpp"
#include "StringUtils.hpp"
#include "Assert.hpp"
#include "SearchThread.hpp"

namespace UCIUtils {

    auto loop() noexcept -> void {

        bool should_run = true;
        GameState game_state;
        SearchThread search_thread;

        do {
            std::string input_command = GameUtils::get_user_input();
            auto line_split = StringUtils::split(input_command);
            ASSERT(line_split.size() > 0);

            auto &command_action = line_split[0];
            const std::unordered_map<std::string, std::function<void()>> command_map{
                    {"uci",
                            [] {
                                UCIUtils::handle_uci_command();
                            }},
                    {"debug",
                            [] {}
                    },
                    {"isready",
                            [] {
                                UCIUtils::handle_isready_command();
                            }},
                    {"setoption",
                            [] {}
                    },
                    {"register",
                            [] {}
                    },
                    {"ucinewgame",
                            [&game_state] {
                                UCIUtils::handle_ucinewgame_command(game_state);
                            }},
                    {"position",
                            [&game_state, &line_split] {
                                UCIUtils::handle_position_command(game_state, line_split);
                            }},
                    {"go",
                            [&game_state, &search_thread] {
                                UCIUtils::handle_go_command(game_state, search_thread);
                            }},
                    {"stop",
                            [&search_thread] {
                                UCIUtils::handle_stop_command(search_thread);
                            }},
                    {"ponderhit",
                            [] {}},
                    {"quit",
                            [&should_run, &search_thread] {
                                UCIUtils::handle_quit_command(search_thread);
                                should_run = false;
                            }},
            };

            if (command_map.contains(command_action)) {
                auto &command_func = command_map.find(command_action)->second;
                command_func();
            }
        } while (should_run);
    }

    auto send_id() noexcept -> void {
        printf("id name ChessEngine\n");
        printf("id author Evan Unmann\n");
    }

    auto send_uci_ok() noexcept -> void {
        printf("uciok\n");
    }

    auto send_ready_ok() noexcept -> void {
        printf("readyok\n");
    }

    auto send_best_move(const std::string &move_str) noexcept -> void {
        printf("bestmove %s\n", move_str.c_str());
    }

    auto send_copyright_protection() noexcept -> void {
        printf("copyrightprotection checking\n");
        printf("copyrightprotection ok\n");
    }

    auto send_registration() noexcept -> void {
        printf("registration checking\n");
        printf("registration ok\n");
    }

    auto send_info() noexcept -> void {
    }

    auto send_option() noexcept -> void {
    }

    auto handle_uci_command() noexcept -> void {
        send_id();
        send_option();
        send_uci_ok();
    }

    auto handle_debug_commend() noexcept -> void {

    }

    auto handle_isready_command() noexcept -> void {
        send_ready_ok();
    }

    auto handle_setoption_command() noexcept -> void {

    }

    auto handle_ucinewgame_command(GameState &game_state) noexcept -> void {
        game_state.init();
    }

    auto handle_position_command(GameState &game_state, const std::vector<std::string> &line_split) noexcept -> void {
        if (line_split[1] == "startpos") {
            game_state.init();
        }
        for (size_t i = 3; i < line_split.size(); ++i) {
            std::string move_str = line_split[i];
            if (!GameUtils::process_user_move(game_state, move_str)) {
                printf("info string cannot process move %s\n", move_str.c_str());
            }
        }
    }

    auto handle_go_command(const GameState &game_state, SearchThread &search_thread) noexcept -> void {
        search_thread.start_search(game_state, 7);
    }

    auto handle_stop_command(SearchThread &search_thread) noexcept -> void {
        search_thread.stop_search();
    }

    auto handle_quit_command(SearchThread &search_thread) noexcept -> void {
        search_thread.stop();
    }
}