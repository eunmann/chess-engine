#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <iostream>

#include "Board.hpp"
#include "Definitions.hpp"
#include "GameUtils.hpp"
#include "Position.hpp"

#include "UCIUtils.hpp"

void print_bit_board(const Position &bit_board) {
    Board board;
    GameUtils::copy(bit_board, board);
    board.print();
}

int main() {
    srand((unsigned)time(NULL));
    setbuf(stdout, NULL);
    setbuf(stdin, NULL);

    bool console = false;

    GameState game_state;

    if (console) {
        GameUtils::init_standard(game_state);
        print_bit_board(game_state.position);
    }

    while (UCIUtils::process_input_command(game_state)) {
        if (console) {
            print_bit_board(game_state.position);
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

            GPU?
                What would be useful here?
                    Generating moves?
                    Computing the heuristic?
            
            Need to profile what is taking so long

            Searching needs load balancing as well

            Threads need to stop on stop command from GUI

            GameUtils::is_valid is computationally expensive

        Engine Things:
            Accept clock time and obey it

            Print info strings for in-game info and also debugging strings
    */

    return 0;
}