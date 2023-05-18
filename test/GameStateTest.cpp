#include "../src/GameState.hpp"
#include "../src/Tests.hpp"

auto Tests::add_game_state_tests(TestFW::UnitTest &unit_tests) -> void {
    TestFW::TestCase move_test_case("GameState");

    move_test_case.tests.emplace_back(TestFW::Test("GameState::set_rook_*_moved", []() {
        GameState game_state;

        TFW_ASSERT_EQ(false, game_state.has_rook_A_moved<Colors::WHITE>());
        TFW_ASSERT_EQ(false, game_state.has_rook_H_moved<Colors::WHITE>());
        TFW_ASSERT_EQ(false, game_state.has_rook_A_moved<Colors::BLACK>());
        TFW_ASSERT_EQ(false, game_state.has_rook_H_moved<Colors::BLACK>());

        game_state.set_rook_A_moved<Colors::WHITE>(true);
        TFW_ASSERT_EQ(true, game_state.has_rook_A_moved<Colors::WHITE>());

        game_state.set_rook_H_moved<Colors::WHITE>(true);
        TFW_ASSERT_EQ(true, game_state.has_rook_H_moved<Colors::WHITE>());

        game_state.set_rook_A_moved<Colors::BLACK>(true);
        TFW_ASSERT_EQ(true, game_state.has_rook_A_moved<Colors::BLACK>());

        game_state.set_rook_H_moved<Colors::BLACK>(true);
        TFW_ASSERT_EQ(true, game_state.has_rook_H_moved<Colors::BLACK>());
    }));

    unit_tests.test_cases.push_back(move_test_case);
}