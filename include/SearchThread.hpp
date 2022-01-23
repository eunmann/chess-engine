#pragma once

#include <thread>
#include <condition_variable>
#include "Move.hpp"
#include <inttypes.h>
#include "GameState.hpp"
#include "MoveSearch.hpp"

class SearchThread {
  public:

  SearchThread();
  auto start_search(const GameState& game_state, const uint32_t search_depth) noexcept -> void;
  auto stop_search() noexcept -> void;
  auto stop() noexcept -> void;

  private:

  template <const Color max_color>
  auto alpha_beta_pruning_search(const GameState& game_state, const int32_t ply_depth, int32_t alpha, int32_t beta) noexcept -> int32_t {

    if (!this->should_search()) {
      return 0;
    }

    if (ply_depth == 0) {
      this->m_leaf_nodes_counter++;
      return MoveSearch::get_position_heuristic(game_state);
    }

    Moves moves;
    MoveGeneration::get_moves<max_color>(game_state, moves);
    this->m_counter += moves.size();

    constexpr Color opponent_color = max_color == Colors::WHITE ? Colors::BLACK : Colors::WHITE;
    int32_t best_heuristic = max_color == Colors::WHITE ? PieceValues::NEG_INFINITY : PieceValues::POS_INFINITY;

    for (auto& move : moves) {
      GameState check = game_state;
      check.apply_move<max_color>(move);
      if (!check.is_legal()) {
        continue;
      }

      if constexpr (max_color == Colors::WHITE) {
        best_heuristic = std::max(best_heuristic, this->alpha_beta_pruning_search<opponent_color>(check, ply_depth - 1, alpha, beta));
        if (best_heuristic >= beta) {
          this->m_times_pruned++;
          break;
        }
        alpha = std::max(alpha, best_heuristic);
      } else if constexpr (max_color == Colors::BLACK) {
        best_heuristic = std::min(best_heuristic, this->alpha_beta_pruning_search<opponent_color>(check, ply_depth - 1, alpha, beta));
        if (best_heuristic <= alpha) {
          this->m_times_pruned++;
          break;
        }
        beta = std::min(beta, best_heuristic);
      }
    }
    return best_heuristic;
  }

  auto work() -> void;
  auto should_search() const noexcept -> bool;
  auto wait_for_search_event() -> void;

  std::thread m_thread;

  std::mutex m_mutex_cv;
  std::condition_variable m_cv;
  bool m_start_search = false;

  bool m_run_thread = true;

  GameState m_game_state;
  uint32_t m_search_depth = 1;

  int64_t m_counter = 0;
  int64_t m_leaf_nodes_counter = 0;
  int64_t m_times_pruned = 0;
};