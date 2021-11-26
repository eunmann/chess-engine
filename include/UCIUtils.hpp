#pragma once

#include <string>

#include "GameState.hpp"

namespace UCIUtils {
  auto process_input_command(GameState& game_State, const std::string& command) noexcept -> int32_t;

  auto send_id() noexcept -> void;
  auto send_uci_ok() noexcept -> void;
  auto send_ready_ok() noexcept -> void;
  auto send_best_move(const std::string& move_str) noexcept -> void;
  auto send_copyright_protection() noexcept -> void;
  auto send_registration() noexcept -> void;
  auto send_info() noexcept -> void;
  auto send_option() noexcept -> void;
}  // namespace UCIUtils