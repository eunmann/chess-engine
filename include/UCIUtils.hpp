#pragma once

#include <string>

#include "GameState.hpp"

namespace UCIUtils {
auto process_input_command(GameState& game_State, const std::string& command)
    -> int32_t;

auto send_id() -> void;
auto send_uci_ok() -> void;
auto send_ready_ok() -> void;
auto send_best_move(const std::string& move_str) -> void;
auto send_copyright_protection() -> void;
auto send_registration() -> void;
auto send_info() -> void;
auto send_option() -> void;
}  // namespace UCIUtils