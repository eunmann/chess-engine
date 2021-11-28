
#pragma once

#include "GameUtils.hpp"
#include "Move.hpp"
#include "MoveGeneration.hpp"

static int64_t counter = 0;
static int64_t leaf_nodes_counter = 0;
static int64_t times_pruned = 0;

namespace MoveSearch {

  auto get_best_move(const GameState& game_state) noexcept ->Move;
  auto get_position_heuristic(const GameState& game_state) noexcept ->int32_t;

  template <const Color max_color>
  auto alpha_beta_pruning_search(const GameState& game_state, const int32_t ply_depth, int32_t alpha, int32_t beta) noexcept {
    if (ply_depth == 0) {
      leaf_nodes_counter++;
      return MoveSearch::get_position_heuristic(game_state);
    }

    Moves moves;
    MoveGeneration::get_moves<max_color>(game_state, moves);
    counter += moves.size();

    constexpr Color opponent_color = max_color == Colors::WHITE ? Colors::BLACK : Colors::WHITE;
    int32_t best_heuristic = max_color == Colors::WHITE ? PieceValues::NEG_INFINITY : PieceValues::POS_INFINITY;

    for (auto move : moves) {
      GameState check = game_state;
      check.apply_move(move);
      if (!check.is_legal()) {
        continue;
      }
      if constexpr (max_color == Colors::WHITE) {
        best_heuristic = std::max(best_heuristic, MoveSearch::alpha_beta_pruning_search<opponent_color>(check, ply_depth - 1, alpha, beta));
        if (best_heuristic >= beta) {
          times_pruned++;
          break;
        }
        alpha = std::max(alpha, best_heuristic);
      } else if constexpr (max_color == Colors::BLACK) {
        best_heuristic = std::min(best_heuristic, MoveSearch::alpha_beta_pruning_search<opponent_color>(check, ply_depth - 1, alpha, beta));
        if (best_heuristic <= alpha) {
          times_pruned++;
          break;
        }
        beta = std::min(beta, best_heuristic);
      }
    }
    return best_heuristic;
  }
}  // namespace MoveSearch