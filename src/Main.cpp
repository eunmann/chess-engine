
#include "Board.hpp"
#include "Definitions.hpp"
#include "GameUtils.hpp"
#include "Log.hpp"
#include "Position.hpp"
#include "Tests.hpp"
#include "UCIUtils.hpp"
#include "MoveGeneration.hpp"
#include "CachedMoves.hpp"
#include "MagicBitBoards.hpp"

auto print_bit_board(const Position& position) noexcept -> void {
  position.to_board().print();
}

int main() {

  CachedMoves::init();
  MagicBitBoards::init();

  auto run_tests = true;
  if (run_tests) {
    Tests::run_tests();
    return 0;
  }

  GameState game_state;

  // Prints out the board after each move, for debugging
  auto console = false;

  if (console) {
    do {
      print_bit_board(game_state.position);
    } while (GameUtils::perform_user_move(game_state));
  } else {
    std::string input_command;
    do {
      input_command = GameUtils::get_user_input();
    } while (UCIUtils::process_input_command(game_state, input_command));
  }

  return 0;
}