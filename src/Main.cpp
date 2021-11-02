#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <iostream>

#include "Board.hpp"
#include "Definitions.hpp"
#include "GameUtils.hpp"
#include "Position.hpp"
#include "UCIUtils.hpp"

auto print_bit_board(const Position &bit_board) -> void {
    GameUtils::position_to_board(bit_board).print();
}

int main() {
    srand((unsigned)time(NULL));
    setbuf(stdout, NULL);
    setbuf(stdin, NULL);

    // Prints out the board after each move, for debugging
    auto console = false;

    GameState game_state;

    if (console) {
        GameUtils::init_standard(game_state);
        print_bit_board(game_state.position);
    }

    // Game Loop
    while (true) {
        auto input_command = GameUtils::get_user_input();
        if (!UCIUtils::process_input_command(game_state, input_command)) {
            break;
        };
        if (console) {
            print_bit_board(game_state.position);
        }
    }

    return 0;
}