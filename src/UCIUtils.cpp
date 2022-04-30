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
                                send_id();
                                send_option();
                                send_uci_ok();
                            }},
                    {"debug",
                            [] {}
                    },
                    {"isready",
                            [] {
                                send_ready_ok();
                            }},
                    {"setoption",
                            [] {}
                    },
                    {"register",
                            [] {}
                    },
                    {"ucinewgame",
                            [&game_state] {
                                game_state.init();
                            }},
                    {"position",
                            [&line_split, &game_state] {
                                if (line_split[1] == "startpos") {
                                    game_state.init();
                                }
                                for (size_t i = 3; i < line_split.size(); ++i) {
                                    std::string move_str = line_split[i];
                                    if (!GameUtils::process_user_move(game_state, move_str)) {
                                        printf("info string cannot process move %s\n", move_str.c_str());
                                    }
                                }
                            }},
                    {"go",
                            [&game_state, &search_thread] {
                                search_thread.start_search(game_state, 6);
                            }},
                    {"stop",
                            [&search_thread] {
                                search_thread.stop_search();
                            }},
                    {"ponderhit",
                            [] {}},
                    {"quit",
                            [&should_run, &search_thread] {
                                search_thread.stop();
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
}