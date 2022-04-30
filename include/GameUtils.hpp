#pragma once

#include <functional>
#include <string>
#include <vector>

#include "Board.hpp"
#include "GameState.hpp"
#include "Move.hpp"
#include "Position.hpp"
#include "Moves.hpp"

namespace GameUtils {

    // GameState Modifiers
    auto perform_user_move(GameState &game_state) noexcept -> bool;

    auto process_user_move(GameState &game_state, const std::string &move_str) noexcept -> bool;

    // Input
    auto get_user_input() noexcept -> std::string;

    auto square_name_to_square(const std::string &square_name) noexcept -> Square;

    auto move_str_to_move(const std::string &move_str) noexcept -> Move;

    template<const Color color>
    auto for_each_legal_move(const GameState &game_state, const Moves &moves, const auto &func) noexcept -> void {
        for (const auto &move: moves) {
            GameState check = game_state;
            check.apply_move<color>(move);
            if (check.is_legal()) {
                func(move);
            }
        }
    }
}  // namespace GameUtils