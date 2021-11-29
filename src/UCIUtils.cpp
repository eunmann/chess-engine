#include "UCIUtils.hpp"

#include <assert.h>

#include <functional>
#include <unordered_map>

#include "GameUtils.hpp"
#include "MoveSearch.hpp"
#include "StringUtils.hpp"

auto UCIUtils::process_input_command(GameState& game_state,
  const std::string& command) noexcept -> int32_t {
  auto line_split = StringUtils::split(command);
  assert(line_split.size() > 0);

  auto command_action = line_split[0];
  int32_t rv = 1;

  const std::unordered_map<std::string, std::function<void()>> command_map = {
      {"uci",
       []() {
         UCIUtils::send_id();
         UCIUtils::send_option();
         UCIUtils::send_uci_ok();
       }},
      {"debug", []() {}},
      {"isready", []() { UCIUtils::send_ready_ok(); }},
      {"setoption", []() {}},
      {"register", []() {}},
      {"ucinewgame", [&game_state]() { game_state.init(); }},
      {"position",
       [&line_split, &game_state]() {
         if (line_split[1].compare("startpos") == 0) {
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
       [&game_state]() {
         Move best_move = MoveSearch::get_best_move(game_state);
         UCIUtils::send_best_move(best_move.to_string());
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

auto UCIUtils::send_id() noexcept -> void {
  printf("id name ChessEngine\n");
  printf("id author Evan Unmann\n");
}

auto UCIUtils::send_uci_ok() noexcept -> void {
  printf("uciok\n");
}

auto UCIUtils::send_ready_ok() noexcept -> void {
  printf("readyok\n");
}

auto UCIUtils::send_best_move(const std::string& move_str) noexcept -> void {
  printf("bestmove %s\n", move_str.c_str());
}
auto UCIUtils::send_copyright_protection() noexcept -> void {
  printf("copyrightprotection checking\n");
  printf("copyrightprotection ok\n");
}
auto UCIUtils::send_registration() noexcept -> void {
  printf("registration checking\n");
  printf("registration ok\n");
}
auto UCIUtils::send_info() noexcept -> void {
}

auto UCIUtils::send_option() noexcept -> void {
}