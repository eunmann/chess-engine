#pragma once

#include <string>

#include "GameState.hpp"
<<<<<<< HEAD
#include "ThreadState.hpp"

namespace UCIUtils {
int32_t process_input_command(ThreadState& thread_state, std::vector<std::thread>& spawned_threads);
int32_t process_user_move(GameState& game_state, const std::string& move_str);

void send_id();
void send_uci_ok();
void send_ready_ok();
void send_best_move(const std::string& move_str);
void send_copyright_protection();
void send_registration();
void send_info();
void send_option();

void send_ai_move(ThreadState& thread_state);
=======

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
>>>>>>> good
}  // namespace UCIUtils