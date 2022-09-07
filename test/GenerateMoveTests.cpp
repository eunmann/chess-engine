#include "TestFW.hpp"

#include "CommonHeaders.hpp"
#include "BitBoardUtils.hpp"
#include "Moves.hpp"
#include "GameState.hpp"
#include <unordered_set>
#include "MoveGeneration.hpp"

namespace Tests {
    auto add_generate_move_tests(TestFW::UnitTest &unit_tests) -> void {
        TestFW::TestCase generate_moves_test_case("Generate Moves");
        generate_moves_test_case.tests.emplace_back(TestFW::Test("MoveGeneration::get_moves - White first move", []() {
            GameState game_state;
            game_state.init();

            Moves moves;
            MoveGeneration::get_moves<Colors::WHITE>(game_state, moves);

            std::vector<std::string> legal_moves_strs;
            GameUtils::for_each_legal_move<Colors::WHITE>(game_state, moves, [&legal_moves_strs](const Move &move) {
                legal_moves_strs.push_back(move.to_string());
            });

            std::unordered_set<std::string> expected_moves{"a2a3", "a2a4",
                                                           "b2b3", "b2b4",
                                                           "c2c3", "c2c4",
                                                           "d2d3", "d2d4",
                                                           "e2e3", "e2e4",
                                                           "f2f3", "f2f4",
                                                           "g2g3", "g2g4",
                                                           "h2h3", "h2h4",
                                                           "b1a3", "b1c3",
                                                           "g1f3", "g1h3"};

            std::vector<std::string> not_contained_moves;

            for (auto &move: legal_moves_strs) {
                if (!expected_moves.contains(move)) {
                    not_contained_moves.push_back(move);
                }
            }

            TFW_ASSERT_EQ(0, not_contained_moves.size());
            TFW_ASSERT_EQ(20, legal_moves_strs.size());
        }));

        generate_moves_test_case.tests.emplace_back(TestFW::Test("MoveGeneration::get_moves - Black first move", []() {
            GameState game_state;
            game_state.init();
            GameUtils::process_user_move(game_state, "e2e4");

            Moves moves;
            MoveGeneration::get_moves<Colors::BLACK>(game_state, moves);

            std::vector<std::string> legal_moves_strs;
            GameUtils::for_each_legal_move<Colors::BLACK>(game_state, moves, [&legal_moves_strs](const Move &move) {
                legal_moves_strs.push_back(move.to_string());
            });

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

            std::vector<std::string> not_contained_moves;

            for (auto &move: legal_moves_strs) {
                if (!expected_moves.contains(move)) {
                    not_contained_moves.push_back(move);
                    printf("*** %s ***\n", move.c_str());
                }
            }

            TFW_ASSERT_EQ(0, not_contained_moves.size());
            TFW_ASSERT_EQ(20, legal_moves_strs.size());
        }));

        unit_tests.test_cases.push_back(generate_moves_test_case);
    }
}