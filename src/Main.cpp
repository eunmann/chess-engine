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
#include <ranges>

#include "Board.hpp"
#include "Definitions.hpp"
#include "GameUtils.hpp"
#include "Log.hpp"
#include "Position.hpp"
#include "Tests.hpp"
#include "UCIUtils.hpp"
#include "MoveGeneration.hpp"

auto print_bit_board(const Position& position) noexcept -> void {
  position.to_board().print();
}

auto init_psuedo_moves() {
  for (auto square : std::views::iota(0, Squares::NUM)) {
    const BitBoard bit_board = GameUtils::square_to_bit_board(square);
    PSEDUO_MOVES_KNIGHT[square] = MoveGeneration::get_knight_capture_positions(bit_board);
    PSEDUO_MOVES_KING[square] = MoveGeneration::get_king_capture_positions(bit_board);
  }
}

int main() {
  srand((unsigned)time(NULL));
  setbuf(stdout, NULL);
  setbuf(stdin, NULL);

  init_psuedo_moves();

  auto run_tests = false;
  if (run_tests) {
    Tests::run_tests();
    return 0;
  }

  GameState game_state;
  game_state.init();

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