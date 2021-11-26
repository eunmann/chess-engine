#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <algorithm>
#include <functional>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

#include "Board.hpp"
#include "Definitions.hpp"
#include "GameUtils.hpp"
#include "Log.hpp"
#include "Position.hpp"
#include "Tests.hpp"
#include "UCIUtils.hpp"

auto print_bit_board(const Position& position) noexcept -> void {
  position.to_board().print();
}

constexpr auto init_pseduo_moves() noexcept -> void {
  std::iota(BitBoards::PSEDUO_MOVES_KNIGHT.begin(),
            BitBoards::PSEDUO_MOVES_KNIGHT.end(), 1);

  std::transform(BitBoards::PSEDUO_MOVES_KNIGHT.begin(),
                 BitBoards::PSEDUO_MOVES_KNIGHT.end(),
                 BitBoards::PSEDUO_MOVES_KNIGHT.begin(),
                 [](auto el) { return 0; });
}

int main() {
  srand((unsigned)time(NULL));
  setbuf(stdout, NULL);
  setbuf(stdin, NULL);

  init_pseduo_moves();

  auto run_tests = true;
  if(run_tests) {
    Tests::run_tests();
    return 0;
  }

  GameState game_state;
  game_state.init();

  // Prints out the board after each move, for debugging
  auto console = false;
  if(console) {
    do {
      print_bit_board(game_state.position);
    } while(GameUtils::perform_user_move(game_state));
  } else {
    std::string input_command;
    do {
      input_command = GameUtils::get_user_input();
    } while(UCIUtils::process_input_command(game_state, input_command));
  }

  return 0;
}