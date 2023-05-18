#pragma once

#include <string>
#include <vector>

#include "GameState.hpp"
#include "SearchThread.hpp"

namespace UCIUtils {

    /**
     * Processes STDIN input. UCI specification defined at
     * http://wbec-ridderkerk.nl/html/UCIProtocol.html.
     */
    auto loop() noexcept -> void;

    auto send_id() noexcept -> void;

    auto send_uci_ok() noexcept -> void;

    auto send_ready_ok() noexcept -> void;

    auto send_best_move(const std::string &move_str) noexcept -> void;

    auto send_copyright_protection() noexcept -> void;

    auto send_registration() noexcept -> void;

    auto send_info() noexcept -> void;

    auto send_option() noexcept -> void;

    auto handle_uci_command() noexcept -> void;

    auto handle_debug_commend() noexcept -> void;

    auto handle_isready_command() noexcept -> void;

    auto handle_setoption_command() noexcept -> void;

    auto handle_ucinewgame_command(GameState &game_state) noexcept -> void;

    auto handle_position_command(GameState &game_state, const std::vector<std::string> &line_split) noexcept -> void;

    auto handle_go_command(const GameState &game_state, SearchThread &search_thread) noexcept -> void;

    auto handle_stop_command(SearchThread &search_thread) noexcept -> void;

    auto handle_quit_command(SearchThread &search_thread) noexcept -> void;
}  // namespace UCIUtils