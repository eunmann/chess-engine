#include "TestFW.hpp"

#include "Definitions.hpp"
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
                }
            }

            TFW_ASSERT_EQ(0, not_contained_moves.size());
            TFW_ASSERT_EQ(20, legal_moves_strs.size());
        }));

        generate_moves_test_case.tests.emplace_back(
                TestFW::Test("MoveGeneration::get_moves - Pawn 2 Forward Blocked by Piece", []() {

                    GameState game_state;
                    game_state.init();

                    std::vector<std::string> moves{"e2e4", "d7d5", "e4e5", "b8c6", "d2d4", "e7e6", "g1f3", "a7a6",
                                                   "c1e3", "f7f6", "e5f6", "g8f6", "c2c3", "f6g4", "h2h3", "g4e3",
                                                   "f2e3", "b7b5", "f1d3", "d8d6", "e1g1", "d6g3", "b1d2"};
                    for (std::size_t i = 0; i < moves.size(); i++) {
                        game_state.init();
                        for (std::size_t j = 0; j <= i; j++) {
                            auto move = moves[j];
                            TFW_ASSERT_EQ(true, GameUtils::process_user_move(game_state, move));
                            TFW_ASSERT_EQ(true, game_state.is_legal());
                        }
                    }

                    Moves gen_moves;
                    MoveGeneration::get_moves<Colors::BLACK>(game_state, gen_moves);

                    std::vector<std::string> legal_moves_strs;
                    GameUtils::for_each_legal_move<Colors::BLACK>(game_state, gen_moves,
                                                                  [&legal_moves_strs](const Move &move) {
                                                                      legal_moves_strs.push_back(move.to_string());
                                                                      printf("Move: %s\n", move.to_string().c_str());
                                                                  });

                    std::unordered_set<std::string> not_expected_moves{"c7c5"};
                    std::vector<std::string> illegal_moves_generated;

                    for (auto &move: legal_moves_strs) {
                        if (not_expected_moves.contains(move)) {
                            illegal_moves_generated.push_back(move);
                        }
                    }

                    TFW_ASSERT_EQ(0, illegal_moves_generated.size());
                }));

        unit_tests.test_cases.push_back(generate_moves_test_case);
    }
}