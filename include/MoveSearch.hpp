
#pragma once

#include "GameUtils.hpp"
#include "Move.hpp"

namespace MoveSearch {
auto get_best_move(const GameState &game_state) -> Move;
auto alpha_beta_pruning_search(const GameState &game_state, int32_t ply_depth, int32_t alpha, int32_t beta, bool max_white) -> int32_t;
auto get_position_heuristic(const GameState &game_state) -> int32_t;
}  // namespace MoveSearch