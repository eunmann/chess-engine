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
<<<<<<< HEAD
#include "ThreadState.hpp"
=======
#include "Tests.hpp"
>>>>>>> good
#include "UCIUtils.hpp"
#include "MoveGeneration.hpp"

auto print_bit_board(const Position& position) noexcept -> void {
  position.to_board().print();
}

<<<<<<< HEAD
int main() {
    srand((unsigned)time(NULL));
    setbuf(stdout, NULL);
    setbuf(stdin, NULL);

    bool console = false;
    ThreadState thread_state;
    std::vector<std::thread> spawned_threads;

    if (console) {
        GameUtils::init_standard(thread_state.game_state);
        print_bit_board(thread_state.game_state.position);
    }

    while (UCIUtils::process_input_command(thread_state, spawned_threads)) {
        if (console) {
            print_bit_board(thread_state.game_state.position);
        }
    }

    /*  TODO(EMU):

        Code:
            Probably need to refactor data structures and code
                Maybe put some functions into BitBoard and GameState
                Use templates
                Create an array of psuedo legal moves
                Creates a Moves structure

        AI:
            Book openings?
        
        Performance:
            Maybe setup bitboards as a structure of arrays?
                SIMD for generating moves?
                SIMD for computing heuristics?

            Might be able to swap if-statements with bit operations
=======
auto init_psuedo_moves() {
  for (auto square : std::views::iota(0, Squares::NUM)) {
    const BitBoard bit_board = GameUtils::square_to_bit_board(square);
    PSEDUO_MOVES_KNIGHT[square] = MoveGeneration::get_knight_capture_positions(bit_board);
    PSEDUO_MOVES_KING[square] = MoveGeneration::get_king_capture_positions(bit_board);
  }
}
>>>>>>> good

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