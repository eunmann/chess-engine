#include "TestFW.hpp"

#include "Definitions.hpp"
#include "BitBoardUtils.hpp"
#include "Moves.hpp"
#include <cassert>
#include "GameState.hpp"
#include <unordered_set>
#include "MoveSearch.hpp"

namespace Tests {
    auto add_move_search_tests(TestFW::UnitTest &unit_tests) -> void {

        TestFW::TestCase move_search_test_case("Move Search");
        move_search_test_case.tests.emplace_back("MoveSearch::get_best_move", []() {
            GameState game_state;
            game_state.init();

            GameUtils::process_user_move(game_state, "e2e4");
            TFW_ASSERT_EQ(true, game_state.is_legal());
            TFW_ASSERT_EQ(false, game_state.is_white_to_move());

            Move move = MoveSearch::get_best_move(game_state);

            std::unordered_set<std::string> expected_moves{"a7a6", "a7a5",
                                                           "b7b6", "b7b5",
                                                           "c7c6", "c7c5",
                                                           "d7d6", "d7d5",
                                                           "e7e6", "e7e5",
                                                           "f7f6", "f7f5",
                                                           "g7g6", "g7g5",
                                                           "h7h6", "h7h5",
                                                           "b8a6", "b8c6",
                                                           "g8f6", "g8h6"};

            TFW_ASSERT_EQ(true, expected_moves.contains(move.to_string()));

            GameUtils::process_user_move(game_state, move.to_string());
            TFW_ASSERT_EQ(true, game_state.is_legal());

        });
        unit_tests.test_cases.push_back(move_search_test_case);
    }
}