#include "Tests.hpp"

#include <assert.h>
#include <vector>
#include <unordered_set>
#include <string>

#include "GameUtils.hpp"
#include "MoveSearch.hpp"
#include "TestFW.hpp"

namespace Tests {
  auto run_tests() noexcept -> void {
    TestFW::UnitTest game_utils_unit_test("GameUtils");
    TestFW::TestCase conversion_test_case("Conversions");
    conversion_test_case.tests.push_back(
      TestFW::Test("BitBoardUtils::bit_board_to_square", []() {
        BitBoard bit_board = 0b101ULL;
        int32_t counter = 0;
        do {
          Square expected_square = counter;
          Square actual_square = BitBoardUtils::bit_board_to_square(bit_board);

          TFW_ASSERT_EQ(expected_square, actual_square);

          counter++;
          bit_board <<= 1;
        } while (bit_board != 0);
        }));
    conversion_test_case.tests.push_back(
      TestFW::Test("BitBoardUtils::square_to_bit_board", []() {
        Square square = 0;
        do {
          BitBoard expected_bit_board = 0b1ULL << square;
          BitBoard actual_bit_board = BitBoardUtils::square_to_bit_board(square);

          TFW_ASSERT_EQ(expected_bit_board, actual_bit_board);

          square++;
        } while (square < 64);
        }));
    game_utils_unit_test.test_cases.push_back(conversion_test_case);

    TestFW::TestCase shifts_test_case("Shifts");
    shifts_test_case.tests.push_back(
      TestFW::Test("BitBoardUtils::shift_bit_board", []() {
        const BitBoard start = 0b10'0000'0000;

        const BitBoard expected_left = 0b100'0000'0000;
        const BitBoard expected_right = 0b001'0000'0000;
        const BitBoard expected_up = 0b10'0000'0000'0000'0000;
        const BitBoard expected_down = 0b10;
        const BitBoard expected_up_left = 0b100'0000'0000'0000'0000;
        const BitBoard expected_up_right = 0b001'0000'0000'0000'0000;
        const BitBoard expected_down_left = 0b100;
        const BitBoard expected_down_right = 0b001;

        assert((BitBoardUtils::shift_bit_board<1, 0>(start) == expected_up));
        assert((BitBoardUtils::shift_bit_board(start, 1, 0) == expected_up));

        assert((BitBoardUtils::shift_bit_board<-1, 0>(start) == expected_down));
        assert((BitBoardUtils::shift_bit_board(start, -1, 0) == expected_down));

        assert((BitBoardUtils::shift_bit_board<0, 1>(start) == expected_left));
        assert((BitBoardUtils::shift_bit_board(start, 0, 1) == expected_left));

        assert((BitBoardUtils::shift_bit_board<0, -1>(start) == expected_right));
        assert((BitBoardUtils::shift_bit_board(start, 0, -1) == expected_right));

        assert((BitBoardUtils::shift_bit_board<1, 1>(start) == expected_up_left));
        assert((BitBoardUtils::shift_bit_board(start, 1, 1) == expected_up_left));

        assert((BitBoardUtils::shift_bit_board<1, -1>(start) == expected_up_right));
        assert((BitBoardUtils::shift_bit_board(start, 1, -1) == expected_up_right));

        assert(
          (BitBoardUtils::shift_bit_board<-1, 1>(start) == expected_down_left));
        assert(
          (BitBoardUtils::shift_bit_board(start, -1, 1) == expected_down_left));

        assert(
          (BitBoardUtils::shift_bit_board<-1, -1>(start) == expected_down_right));
        assert(
          (BitBoardUtils::shift_bit_board(start, -1, -1) == expected_down_right));
        }));
    game_utils_unit_test.test_cases.push_back(shifts_test_case);

    TestFW::TestCase move_test_case("Move");
    move_test_case.tests.push_back(TestFW::Test("Move::get_source_square", []() {
      for (int i = 1; i < 64; i++) {
        Move move(i, 0);
        assert(i == move.get_source_square());
        assert(!move.is_castle());
        assert(!move.is_en_passantable());
        assert(!move.is_promotion());
      }
      }));
    move_test_case.tests.push_back(
      TestFW::Test("Move::get_destination_square", []() {
        for (int i = 1; i < 64; i++) {
          Move move(0, i);
          assert(i == move.get_destination_square());
          assert(!move.is_castle());
          assert(!move.is_en_passantable());
          assert(!move.is_promotion());
        }
        }));
    move_test_case.tests.push_back(TestFW::Test(
      "Move::get_source_square & Move::get_destination_square", []() {
        for (int i = 0; i < 64; i++) {
          int source = 63 - i;
          int dest = i;
          if (source == dest) {
            continue;
          }
          Move move(source, dest);
          assert(source == move.get_source_square());
          assert(dest == move.get_destination_square());
          assert(!move.is_castle());
          assert(!move.is_en_passantable());
          assert(!move.is_promotion());
        }
      }));
    move_test_case.tests.push_back(TestFW::Test("Move::get_en_passant", []() {
      Move move(1, 2);
      move.set_en_passant(3);
      TFW_ASSERT_EQ(true, move.is_en_passantable());
      TFW_ASSERT_EQ(3, move.get_en_passant());
      }));
    move_test_case.tests.push_back(TestFW::Test("Move::get_castle", []() {
      Move move(1, 3);
      move.set_castle(Castles::WHITE_QUEEN);
      TFW_ASSERT_EQ(true, move.is_castle());
      TFW_ASSERT_EQ(Castles::WHITE_QUEEN, move.get_castle());
      }));
    move_test_case.tests.push_back(TestFW::Test("Move::get_promotion", []() {
      Move move(1, 4);
      TFW_ASSERT_EQ(false, move.is_promotion());
      move.set_promotion(PieceCodes::BISHOP);
      TFW_ASSERT_EQ(true, move.is_promotion());
      TFW_ASSERT_EQ(PieceCodes::BISHOP, move.get_promotion());
      }));
    game_utils_unit_test.test_cases.push_back(move_test_case);

    TestFW::TestCase generate_moves_test_case("Generate Moves");
    generate_moves_test_case.tests.push_back(TestFW::Test("MoveGeneration::get_moves - White first move", []() {
      GameState game_state;
      game_state.init();

      Moves moves;
      MoveGeneration::get_moves<Colors::WHITE>(game_state, moves);

      std::vector<std::string> legal_moves_strs;
      GameUtils::for_each_legal_move<Colors::WHITE>(game_state, moves, [&legal_moves_strs](const Move& move) {
        legal_moves_strs.push_back(move.to_string());
        });

      TFW_ASSERT_EQ(20, legal_moves_strs.size());

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

      for (auto& move : legal_moves_strs) {
        if (!expected_moves.contains(move)) {
          not_contained_moves.push_back(move);
        }
      }

      TFW_ASSERT_EQ(0, not_contained_moves.size());
      }));

    generate_moves_test_case.tests.push_back(TestFW::Test("MoveGeneration::get_moves - Black first move", []() {
      GameState game_state;
      game_state.init();
      GameUtils::process_user_move(game_state, "e2e4");

      Moves moves;
      MoveGeneration::get_moves<Colors::BLACK>(game_state, moves);

      std::vector<std::string> legal_moves_strs;
      GameUtils::for_each_legal_move<Colors::BLACK>(game_state, moves, [&legal_moves_strs](const Move& move) {
        legal_moves_strs.push_back(move.to_string());
        });

      TFW_ASSERT_EQ(20, legal_moves_strs.size());

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

      for (auto& move : legal_moves_strs) {
        if (!expected_moves.contains(move)) {
          not_contained_moves.push_back(move);
        }
      }

      TFW_ASSERT_EQ(0, not_contained_moves.size());
      }));

    game_utils_unit_test.test_cases.push_back(generate_moves_test_case);

    TestFW::TestCase process_move_test_case("Process Move");
    process_move_test_case.tests.push_back(TestFW::Test("GameUtils::process_user_move - Kasparov vs. Topalov, Wijk aan Zee 1999", []() {

      GameState game_state;
      game_state.init();
      std::vector<std::string> moves{"e2e4", "d7d6", "d2d4", "g8f6", "b1c3", "g7g6", "c1e3", "f8g7", "d1d2", "c7c6", "f2f3", "b7b5", "g1e2", "b8d7", "e3h6", "g7h6", "d2h6", "c8b7", "a2a3", "e7e5", "e1c1", "d8e7", "c1b1", "a7a6", "e2c1", "e8c8", "c1b3", "e5d4", "d1d4", "c6c5", "d4d1", "d7b6", "g2g3", "c8b8", "b3a5", "b7a8", "f1h3", "d6d5", "h6f4", "b8a7", "h1e1", "d5d4", "c3d5", "b6d5", "e4d5", "e7d6", "d1d4", "c5d4", "e1e7", "a7b6", "f4d4", "b6a5", "b2b4", "a5a4", "d4c3", "d6d5", "e7a7", "a8b7", "a7b7", "d5c4", "c3f6", "a4a3", "f6a6", "a3b4", "c2c3", "b4c3", "a6a1", "c3d2", "a1b2", "d2d1", "h3f1", "d8d2", "b7d7", "d2d7", "f1c4", "b5c4", "b2h8", "d7d3", "h8a8", "c4c3", "a8a4", "d1e1", "f3f4", "f7f5", "b1c1", "d3d2", "a4a7"};

      for (auto& move : moves) {
        GameUtils::process_user_move(game_state, move);
        TFW_ASSERT_EQ(true, game_state.is_legal());
      }
      }));
    process_move_test_case.tests.push_back(TestFW::Test("GameUtils::process_user_move - Morphy vs. Allies, Paris Opera 1858", []() {

      GameState game_state;
      game_state.init();
      std::vector<std::string> moves{"e2e4", "e7e5", "g1f3", "d7d6", "d2d4", "c8g4", "d4e5", "g4f3", "d1f3", "d6e5", "f1c4", "g8f6", "f3b3", "d8e7", "b1c3", "c7c6", "c1g5", "b7b5", "c3b5", "c6b5", "c4b5", "b8d7", "e1c1", "a8d8", "d1d7", "d8d7", "h1d1", "e7e6", "b5d7", "f6d7", "b3b8", "d7b8", "d1d8"};

      for (auto& move : moves) {
        GameUtils::process_user_move(game_state, move);
        TFW_ASSERT_EQ(true, game_state.is_legal());
      }
      }));
    process_move_test_case.tests.push_back(TestFW::Test("GameUtils::process_user_move - Aronian vs. Anand, Wijk aan Zee 2013", []() {

      GameState game_state;
      game_state.init();
      std::vector<std::string> moves{"d2d4", "d7d5", "c2c4", "c7c6", "g1f3", "g8f6", "b1c3", "e7e6", "e2e3", "b8d7", "f1d3", "d5c4", "d3c4", "b7b5", "c4d3", "f8d6", "e1g1", "e8g8", "d1c2", "c8b7", "a2a3", "a8c8", "f3g5", "c6c5", "g5h7", "f6g4", "f2f4", "c5d4", "e3d4", "d6c5", "d3e2", "d7e5", "e2g4", "c5d4", "g1h1", "e5g4", "h7f8", "f7f5", "f8g6", "d8f6", "h2h3", "f6g6", "c2e2", "g6h5", "e2d3", "d4e3"};

      for (auto& move : moves) {
        GameUtils::process_user_move(game_state, move);
        TFW_ASSERT_EQ(true, game_state.is_legal());
      }
      }));
    process_move_test_case.tests.push_back(TestFW::Test("GameUtils::process_user_move - Karpov vs. Kasparov, World Championship 1985, game 16", []() {

      GameState game_state;
      game_state.init();
      std::vector<std::string> moves{"e2e4", "c7c5", "g1f3", "e7e6", "d2d4", "c5d4", "f3d4", "b8c6", "d4b5", "d7d6", "c2c4", "g8f6", "b1c3", "a7a6", "b5a3", "d6d5", "c4d5", "e6d5", "e4d5", "c6b4", "f1e2", "f8c5", "e1g1", "e8g8", "e2f3", "c8f5", "c1g5", "f8e8", "d1d2", "b7b5", "a1d1", "b4d3", "a3b1", "h7h6", "g5h4", "b5b4", "c3a4", "c5d6", "h4g3", "a8c8", "b2b3", "g7g5", "g3d6", "d8d6", "g2g3", "f6d7", "f3g2", "d6f6", "a2a3", "a6a5", "a3b4", "a5b4", "d2a2", "f5g6", "d5d6", "g5g4", "a2d2", "g8g7", "f2f3", "f6d6", "f3g4", "d6d4", "g1h1", "d7f6", "f1f4", "f6e4", "d2d3", "e4f2", "f4f2", "g6d3", "f2d2", "d4e3", "d2d3", "c8c1", "a4b2", "e3f2", "b1d2", "c1d1", "b2d1", "e8e1",};

      for (auto& move : moves) {
        GameUtils::process_user_move(game_state, move);
        TFW_ASSERT_EQ(true, game_state.is_legal());
      }
      }));
    process_move_test_case.tests.push_back(TestFW::Test("GameUtils::process_user_move - Byrne vs. Fischer, New York 1956", []() {

      GameState game_state;
      game_state.init();
      std::vector<std::string> moves{"g1f3", "g8f6", "c2c4", "g7g6", "b1c3", "f8g7", "d2d4", "e8g8", "c1f4", "d7d5", "d1b3", "d5c4", "b3c4", "c7c6", "e2e4", "b8d7", "a1d1", "d7b6", "c4c5", "c8g4", "f4g5", "b6a4", "c5a3", "a4c3", "b2c3", "f6e4", "g5e7", "d8b6", "f1c4", "e4c3", "e7c5", "f8e8", "e1f1", "g4e6", "c5b6", "e6c4", "f1g1", "c3e2", "g1f1", "e2d4", "f1g1", "d4e2", "g1f1", "e2c3", "f1g1", "a7b6", "a3b4", "a8a4", "b4b6", "c3d1", "h2h3", "a4a2", "g1h2", "d1f2", "h1e1", "e8e1", "b6d8", "g7f8", "f3e1", "c4d5", "e1f3", "f2e4", "d8b8", "b7b5", "h3h4", "h7h5", "f3e5", "g8g7", "h2g1", "f8c5", "g1f1", "e4g3", "f1e1", "c5b4", "e1d1", "d5b3", "d1c1", "g3e2", "c1b1", "e2c3", "b1c1", "a2c2"};

      for (auto& move : moves) {
        GameUtils::process_user_move(game_state, move);
        TFW_ASSERT_EQ(true, game_state.is_legal());
      }
      }));
    process_move_test_case.tests.push_back(TestFW::Test("GameUtils::process_user_move - Ivanchuk vs. Yusupov, Brussels 1991", []() {

      GameState game_state;
      game_state.init();
      std::vector<std::string> moves{"c2c4", "e7e5", "g2g3", "d7d6", "f1g2", "g7g6", "d2d4", "b8d7", "b1c3", "f8g7", "g1f3", "g8f6", "e1g1", "e8g8", "d1c2", "f8e8", "f1d1", "c7c6", "b2b3", "d8e7", "c1a3", "e5e4", "f3g5", "e4e3", "f2f4", "d7f8", "b3b4", "c8f5", "c2b3", "h7h6", "g5f3", "f6g4", "b4b5", "g6g5", "b5c6", "b7c6", "f3e5", "g5f4", "e5c6", "e7g5", "a3d6", "f8g6", "c3d5", "g5h5", "h2h4", "g6h4", "g3h4", "h5h4", "d5e7", "g8h8", "e7f5", "h4h2", "g1f1", "e8e6", "b3b7", "e6g6", "b7a8", "h8h7", "a8g8", "h7g8", "c6e7", "g8h7", "e7g6", "f7g6", "f5g7", "g4f2", "d6f4", "h2f4", "g7e6", "f4h2", "d1b1", "f2h3", "b1b7", "h7h8", "b7b8", "h2b8", "g2h3", "b8g3"};

      for (auto& move : moves) {
        GameUtils::process_user_move(game_state, move);
        TFW_ASSERT_EQ(true, game_state.is_legal());
      }
      }));
    process_move_test_case.tests.push_back(TestFW::Test("GameUtils::process_user_move - Short vs. Timman, Tilburg 1991", []() {

      GameState game_state;
      game_state.init();
      std::vector<std::string> moves{"e2e4", "g8f6", "e4e5", "f6d5", "d2d4", "d7d6", "g1f3", "g7g6", "f1c4", "d5b6", "c4b3", "f8g7", "d1e2", "b8c6", "e1g1", "e8g8", "h2h3", "a7a5", "a2a4", "d6e5", "d4e5", "c6d4", "f3d4", "d8d4", "f1e1", "e7e6", "b1d2", "b6d5", "d2f3", "d4c5", "e2e4", "c5b4", "b3c4", "d5b6", "b2b3", "b6c4", "b3c4", "f8e8", "e1d1", "b4c5", "e4h4", "b7b6", "c1e3", "c5c6", "e3h6", "g7h8", "d1d8", "c8b7", "a1d1", "h8g7", "d8d7", "e8f8", "h6g7", "g8g7", "d1d4", "a8e8", "h4f6", "g7g8", "h3h4", "h7h5", "g1h2", "e8c8", "h2g3", "c8e8", "g3f4", "b7c8", "f4g5"};

      for (auto& move : moves) {
        GameUtils::process_user_move(game_state, move);
        TFW_ASSERT_EQ(true, game_state.is_legal());
      }
      }));
    process_move_test_case.tests.push_back(TestFW::Test("GameUtils::process_user_move - Bai Jinshi vs. Ding Liren, Chinese League 2017", []() {

      GameState game_state;
      game_state.init();
      std::vector<std::string> moves{"d2d4", "g8f6", "c2c4", "e7e6", "b1c3", "f8b4", "g1f3", "e8g8", "c1g5", "c7c5", "e2e3", "c5d4", "d1d4", "b8c6", "d4d3", "h7h6", "g5h4", "d7d5", "a1d1", "g7g5", "h4g3", "f6e4", "f3d2", "e4c5", "d3c2", "d5d4", "d2f3", "e6e5", "f3e5", "d4c3", "d1d8", "c3b2", "e1e2", "f8d8", "c2b2", "c5a4", "b2c2", "a4c3", "e2f3", "d8d4", "h2h3", "h6h5", "g3h2", "g5g4", "f3g3", "d4d2", "c2b3", "c3e4", "g3h4", "b4e7", "h4h5", "g8g7", "h2f4", "c8f5", "f4h6", "g7h7", "b3b7", "d2f2", "h6g5", "a8h8", "e5f7", "f5g6", "h5g4", "c6e5"};

      for (auto& move : moves) {
        GameUtils::process_user_move(game_state, move);
        TFW_ASSERT_EQ(true, game_state.is_legal());
      }
      }));
    process_move_test_case.tests.push_back(TestFW::Test("GameUtils::process_user_move - Rotlewi vs. Rubinstein, Lodz 1907", []() {

      GameState game_state;
      game_state.init();
      std::vector<std::string> moves{"d2d4", "d7d5", "g1f3", "e7e6", "e2e3", "c7c5", "c2c4", "b8c6", "b1c3", "g8f6", "d4c5", "f8c5", "a2a3", "a7a6", "b2b4", "c5d6", "c1b2", "e8g8", "d1d2", "d8e7", "f1d3", "d5c4", "d3c4", "b7b5", "c4d3", "f8d8", "d2e2", "c8b7", "e1g1", "c6e5", "f3e5", "d6e5", "f2f4", "e5c7", "e3e4", "a8c8", "e4e5", "c7b6", "g1h1", "f6g4", "d3e4", "e7h4", "g2g3", "c8c3", "g3h4", "d8d2", "e2d2", "b7e4", "d2g2", "c3h3"};

      for (auto& move : moves) {
        GameUtils::process_user_move(game_state, move);
        TFW_ASSERT_EQ(true, game_state.is_legal());
      }
      }));
    process_move_test_case.tests.push_back(TestFW::Test("GameUtils::process_user_move - Geller vs. Euwe, Zurich 1953", []() {

      GameState game_state;
      game_state.init();
      std::vector<std::string> moves{"d2d4", "g8f6", "c2c4", "e7e6", "b1c3", "f8b4", "e2e3", "c7c5", "a2a3", "b4c3", "b2c3", "b7b6", "f1d3", "c8b7", "f2f3", "b8c6", "g1e2", "e8g8", "e1g1", "c6a5", "e3e4", "f6e8", "e2g3", "c5d4", "c3d4", "a8c8", "f3f4", "a5c4", "f4f5", "f7f6", "f1f4", "b6b5", "f4h4", "d8b6", "e4e5", "c4e5", "f5e6", "e5d3", "d1d3", "b6e6", "d3h7", "g8f7", "c1h6", "f8h8", "h7h8", "c8c2", "a1c1", "c2g2", "g1f1", "e6b3", "f1e1", "b3f3"};

      for (auto& move : moves) {
        GameUtils::process_user_move(game_state, move);
        TFW_ASSERT_EQ(true, game_state.is_legal());
      }
      }));
    process_move_test_case.tests.push_back(TestFW::Test("GameUtils::process_user_move - Test Game 1", []() {

      GameState game_state;
      game_state.init();
      std::vector<std::string> moves{"e2e4", " b8a6", "d1h5", "a6b4", "f1c4", "b4c2", "e1d1", "c2e3", "d2e3", "d7d5", "c4d5", "c8e6", "c1d2", "d8d7", "d5e6", "e8d8", "e6d7"};

      for (auto& move : moves) {
        GameUtils::process_user_move(game_state, move);
        TFW_ASSERT_EQ(true, game_state.is_legal());
      }
      }));

    game_utils_unit_test.test_cases.push_back(process_move_test_case);

    TestFW::TestCase move_search_test_case("Move Search");
    move_search_test_case.tests.push_back(TestFW::Test("MoveSearch::get_best_move", []() {
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

      }));
    game_utils_unit_test.test_cases.push_back(move_search_test_case);

    game_utils_unit_test.run();
  }
}  // namespace Tests