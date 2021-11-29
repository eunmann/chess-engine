#pragma once

#include <thread>
#include <vector>

#include "GameState.hpp"

class ThreadState {
   public:
    bool should_search;
    GameState game_state;
    GameState best_move;
};