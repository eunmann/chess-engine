#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <algorithm>
#include <iostream>
#include <numeric>

#include "Board.hpp"
#include "Definitions.hpp"
#include "GameUtils.hpp"
#include "Position.hpp"
#include "UCIUtils.hpp"

auto print_bit_board(const Position &position) -> void {
    position.to_board().print();
}

constexpr auto init_pseduo_moves() -> void {
    std::iota(BitBoards::PSEDUO_MOVES_KNIGHT.begin(),
              BitBoards::PSEDUO_MOVES_KNIGHT.end(),
              1);

    std::transform(BitBoards::PSEDUO_MOVES_KNIGHT.begin(),
                   BitBoards::PSEDUO_MOVES_KNIGHT.end(),
                   BitBoards::PSEDUO_MOVES_KNIGHT.begin(),
                   [](auto el) {
                       return 0;
                   });
}

int main() {
    srand((unsigned)time(NULL));
    setbuf(stdout, NULL);
    setbuf(stdin, NULL);

    // Prints out the board after each move, for debugging
    auto console = true;

    init_pseduo_moves();

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