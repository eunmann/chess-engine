#include "SearchThread.hpp"
#include <cstdio>
#include "UCIUtils.hpp"
#include "Moves.hpp"
#include "Timer.hpp"
#include "MoveGeneration.hpp"

SearchThread::SearchThread() : m_search_thread(&SearchThread::work, this),
                               thread_pool(std::thread::hardware_concurrency()) {
}

auto SearchThread::start_search(const GameState &game_state, const uint32_t search_depth) noexcept -> void {
    this->m_start_search = true;
    this->m_game_state = game_state;
    this->m_search_depth = search_depth;
    this->trigger_search_event();
}

auto SearchThread::stop_search() noexcept -> void {
    this->m_start_search = false;
}

auto SearchThread::stop() noexcept -> void {
    this->m_run_search_thread = false;
    this->stop_search();
    this->trigger_search_event();
    this->m_search_thread.join();
    this->thread_pool.stop();
}

auto SearchThread::work() -> void {

    printf("info Starting Search Thread\n");

    while (this->m_run_search_thread) {

        this->wait_for_search_event();
        if (!this->m_start_search) {
            continue;
        }

        printf("info Event to search\n");

        Timer timer;
        Moves moves;
        Color color_to_move = Colors::bool_to_color(this->m_game_state.is_white_to_move());
        if (color_to_move == Colors::WHITE) {
            MoveGeneration::get_moves<Colors::WHITE>(this->m_game_state, moves);
        } else {
            MoveGeneration::get_moves<Colors::BLACK>(this->m_game_state, moves);
        }

        std::vector<SearchResult> search_results(moves.size());
        size_t index = 0;
        for (auto &move: moves) {
            this->thread_pool.enqueue([this, move, color_to_move, &search_results, index] {
                GameState check = this->m_game_state;
                SearchResult &search_result = search_results[index];
                if (color_to_move == Colors::WHITE) {
                    check.apply_move<Colors::WHITE>(move);
                } else {
                    check.apply_move<Colors::BLACK>(move);
                }
                if (check.is_legal()) {
                    search_result.is_legal = true;
                    if (color_to_move == Colors::WHITE) {
                        search_result.heuristic = this->alpha_beta_pruning_search<Colors::BLACK>(check,
                                                                                                 this->m_search_depth -
                                                                                                 1,
                                                                                                 PieceValues::NEG_INFINITY.value(),
                                                                                                 PieceValues::POS_INFINITY.value(),
                                                                                                 search_result);
                    } else {
                        search_result.heuristic = this->alpha_beta_pruning_search<Colors::WHITE>(check,
                                                                                                 this->m_search_depth -
                                                                                                 1,
                                                                                                 PieceValues::NEG_INFINITY.value(),
                                                                                                 PieceValues::POS_INFINITY.value(),
                                                                                                 search_result);
                    }
                }
                search_result.notify();
            });
            index++;
        }

        // Wait for all the tasks to finish
        for (auto &search_result: search_results) {
            search_result.wait();
        }

        timer.end();

        SearchResult accumulated_search_results;
        Move best_move;
        int32_t best_heuristic = color_to_move == Colors::WHITE ? PieceValues::NEG_INFINITY.value() : PieceValues::POS_INFINITY.value();

        accumulated_search_results.moves_counter += moves.size();

        for (int i = 0; i < moves.size(); i++) {
            const Move move = moves[i];
            const SearchResult &search_result = search_results[i];

            if (!search_result.is_legal) {
                continue;
            }

            accumulated_search_results.moves_counter += search_result.moves_counter;
            accumulated_search_results.leaf_nodes_counter += search_result.leaf_nodes_counter;
            accumulated_search_results.times_pruned_counter += search_result.times_pruned_counter;

            if (color_to_move == Colors::WHITE) {
                if (best_heuristic < search_result.heuristic) {
                    best_heuristic = search_result.heuristic;
                    best_move = move;
                }
            } else {
                if (best_heuristic > search_result.heuristic) {
                    best_heuristic = search_result.heuristic;
                    best_move = move;
                }
            }
        }

        printf("info Time to Search: %s\n", timer.time_string().c_str());
        printf("info Moves considered: %llu\n", accumulated_search_results.moves_counter);
        printf("info Leaf nodes: %llu\n", accumulated_search_results.leaf_nodes_counter);
        printf("info Time Pruned: %llu \n", accumulated_search_results.times_pruned_counter);
        printf("info Nodes per Second: %3.2fM\n",
               (accumulated_search_results.leaf_nodes_counter / (timer.get_time_elapsed() / 1e9)) / 1e6);

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
    this->m_cv.wait(lock, [this] { return this->should_search() || !this->m_run_search_thread; });
}

auto SearchThread::trigger_search_event() -> void {
    this->m_cv.notify_all();
}
