#include "SearchThread.hpp"
#include <stdio.h>
#include "UCIUtils.hpp"
#include "Moves.hpp"
#include "Timer.hpp"
#include "MoveGeneration.hpp"

SearchThread::SearchThread() : m_thread(&SearchThread::work, this) {
}

auto SearchThread::start_search(const GameState& game_state, const uint32_t search_depth) noexcept -> void {
  this->m_start_search = true;
  this->m_game_state = game_state;
  this->m_search_depth = search_depth;
  this->m_cv.notify_all();
}

auto SearchThread::stop_search() noexcept -> void {
  this->m_start_search = false;
}

auto SearchThread::stop() noexcept -> void {
  this->m_run_thread = false;
  this->stop_search();
}

auto SearchThread::work() -> void {

  printf("info Starting Search Thread\n");

  while (this->m_run_thread) {

    this->wait_for_search_event();

    printf("info Event to search\n");

    Timer timer;
    Moves moves;
    Color color_to_move = Colors::bool_to_color(this->m_game_state.is_white_to_move());
    if (color_to_move == Colors::WHITE) {
      MoveGeneration::get_moves<Colors::WHITE>(this->m_game_state, moves);
    } else {
      MoveGeneration::get_moves<Colors::BLACK>(this->m_game_state, moves);
    }

    int32_t best_heuristic = color_to_move == Colors::WHITE ? PieceValues::NEG_INFINITY : PieceValues::POS_INFINITY;
    Move best_move;

    this->m_counter = moves.size();
    this->m_leaf_nodes_counter = 0;
    this->m_times_pruned = 0;
    for (auto& move : moves) {
      GameState check = this->m_game_state;
      if (color_to_move == Colors::WHITE) {
        check.apply_move<Colors::WHITE>(move);
      } else {
        check.apply_move<Colors::BLACK>(move);
      }
      if (check.is_legal()) {

        if (color_to_move == Colors::WHITE) {
          int32_t heuristic = this->alpha_beta_pruning_search<Colors::BLACK>(check, this->m_search_depth, PieceValues::NEG_INFINITY, PieceValues::POS_INFINITY);
          if (best_heuristic < heuristic) {
            best_heuristic = heuristic;
            best_move = move;
          }
        } else {
          int32_t heuristic = this->alpha_beta_pruning_search<Colors::WHITE>(check, this->m_search_depth, PieceValues::NEG_INFINITY, PieceValues::POS_INFINITY);
          if (best_heuristic > heuristic) {
            best_heuristic = heuristic;
            best_move = move;
          }
        }
      }
    }

    timer.end();
    printf("info Time to Search: %s\n", timer.time_string().c_str());
    printf("info Moves considered: %llu\n", this->m_counter);
    printf("info Leaf nodes: %llu\n", this->m_leaf_nodes_counter);
    printf("info Time Pruned: %llu \n", this->m_times_pruned);
    printf("info Nodes per Second: %3.2fM\n", (this->m_leaf_nodes_counter / (timer.get_time_elapsed() / 1e9)) / 1e6);

    UCIUtils::send_best_move(best_move.to_string());

    this->m_start_search = false;
  }

  printf("info Stopping Search Thread\n");
}

auto SearchThread::should_search() const noexcept -> bool {
  return this->m_start_search;
}

auto SearchThread::wait_for_search_event() -> void {
  std::unique_lock<std::mutex> lock(this->m_mutex_cv);
  this->m_cv.wait(lock, [this] {return this->should_search(); });
  lock.unlock();
}