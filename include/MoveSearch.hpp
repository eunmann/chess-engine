
#pragma once

#include "GameUtils.hpp"
#include "Move.hpp"
#include "MoveGeneration.hpp"

namespace MoveSearch {

auto get_best_move(const GameState &game_state) -> Move;
auto alpha_beta_pruning_search(const GameState &game_state, const int32_t ply_depth, int32_t alpha, int32_t beta, const Color max_color);
auto get_position_heuristic(const GameState &game_state) -> int32_t;

// Templates
template <const Color max_color>
constexpr auto alpha_beta_pruning_search(const GameState &game_state, const int32_t ply_depth, int32_t alpha, int32_t beta) {
    if (ply_depth == 0) {
        return MoveSearch::get_position_heuristic(game_state);
    }

    Moves moves;
    MoveGeneration::get_moves<max_color>(game_state, moves);

    if (max_color == Colors::WHITE) {
        int32_t best_heuristic = PieceValues::NEG_INFINITY;
        for (size_t i = 0; i < moves.size(); ++i) {
            Move &move = moves[i];
            // TODO(EMU): Check if move is legal, apply move to a copy of GameState with move
            best_heuristic = std::max(best_heuristic, MoveSearch::alpha_beta_pruning_search<Colors::BLACK>(game_state, ply_depth - 1, alpha, beta));
            if (best_heuristic >= beta) {
                break;
            }
            alpha = std::max(alpha, best_heuristic);
        }
        return best_heuristic;
    } else {
        int32_t best_heuristic = PieceValues::POS_INFINITY;
        for (size_t i = 0; i < moves.size(); ++i) {
            Move &move = moves[i];
            // TODO(EMU): Check if move is legal, apply move to a copy of GameState with move
            best_heuristic = std::min(best_heuristic, MoveSearch::alpha_beta_pruning_search<Colors::WHITE>(game_state, ply_depth - 1, alpha, beta));
            if (best_heuristic <= alpha) {
                break;
            }
            beta = std::min(beta, best_heuristic);
        }
        return best_heuristic;
    }
}
}  // namespace MoveSearch