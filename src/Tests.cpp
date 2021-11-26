#include "Tests.hpp"

#include <assert.h>

#include "GameUtils.hpp"
#include "TestFW.hpp"

namespace Tests {
  auto run_tests() noexcept -> void {
    TestFW::UnitTest game_utils_unit_test("GameUtils");
    TestFW::TestCase conversion_test_case("Conversions");
    conversion_test_case.tests.push_back(
      TestFW::Test("GameUtils::bit_board_to_square", []() {
        BitBoard bit_board = 0b101ULL;
        int32_t counter = 0;
        do {
          Square expected_square = counter;
          Square actual_square = GameUtils::bit_board_to_square(bit_board);

          TFW_ASSERT_EQ(expected_square, actual_square);

          counter++;
          bit_board <<= 1;
        } while(bit_board != 0);
      }));
    conversion_test_case.tests.push_back(
      TestFW::Test("GameUtils::square_to_bit_board", []() {
        Square square = 0;
        do {
          BitBoard expected_bit_board = 0b1ULL << square;
          BitBoard actual_bit_board = GameUtils::square_to_bit_board(square);

          TFW_ASSERT_EQ(expected_bit_board, actual_bit_board);

          square++;
        } while(square < 64);
      }));
    game_utils_unit_test.test_cases.push_back(conversion_test_case);

    TestFW::TestCase shifts_test_case("Shifts");
    shifts_test_case.tests.push_back(
      TestFW::Test("GameUtils::shift_bit_board", []() {
        const BitBoard start = 0b10'0000'0000;

        const BitBoard expected_left = 0b100'0000'0000;
        const BitBoard expected_right = 0b001'0000'0000;
        const BitBoard expected_up = 0b10'0000'0000'0000'0000;
        const BitBoard expected_down = 0b10;
        const BitBoard expected_up_left = 0b100'0000'0000'0000'0000;
        const BitBoard expected_up_right = 0b001'0000'0000'0000'0000;
        const BitBoard expected_down_left = 0b100;
        const BitBoard expected_down_right = 0b001;

        assert((GameUtils::shift_bit_board<1, 0>(start) == expected_up));
        assert((GameUtils::shift_bit_board(start, 1, 0) == expected_up));

        assert((GameUtils::shift_bit_board<-1, 0>(start) == expected_down));
        assert((GameUtils::shift_bit_board(start, -1, 0) == expected_down));

        assert((GameUtils::shift_bit_board<0, 1>(start) == expected_left));
        assert((GameUtils::shift_bit_board(start, 0, 1) == expected_left));

        assert((GameUtils::shift_bit_board<0, -1>(start) == expected_right));
        assert((GameUtils::shift_bit_board(start, 0, -1) == expected_right));

        assert((GameUtils::shift_bit_board<1, 1>(start) == expected_up_left));
        assert((GameUtils::shift_bit_board(start, 1, 1) == expected_up_left));

        assert((GameUtils::shift_bit_board<1, -1>(start) == expected_up_right));
        assert((GameUtils::shift_bit_board(start, 1, -1) == expected_up_right));

        assert(
          (GameUtils::shift_bit_board<-1, 1>(start) == expected_down_left));
        assert(
          (GameUtils::shift_bit_board(start, -1, 1) == expected_down_left));

        assert(
          (GameUtils::shift_bit_board<-1, -1>(start) == expected_down_right));
        assert(
          (GameUtils::shift_bit_board(start, -1, -1) == expected_down_right));
      }));
    game_utils_unit_test.test_cases.push_back(shifts_test_case);

    TestFW::TestCase move_test_case("Move");
    move_test_case.tests.push_back(TestFW::Test("Move::get_source_square", []() {
      for(int i = 1; i < 64; i++) {
        Move move(i, 0);
        assert(i == move.get_source_square());
        assert(!move.is_castle());
        assert(!move.is_en_passantable());
        assert(!move.is_promotion());
      }
                                   }));
    move_test_case.tests.push_back(
      TestFW::Test("Move::get_destination_square", []() {
        for(int i = 1; i < 64; i++) {
          Move move(0, i);
          assert(i == move.get_destination_square());
          assert(!move.is_castle());
          assert(!move.is_en_passantable());
          assert(!move.is_promotion());
        }
      }));
    move_test_case.tests.push_back(TestFW::Test(
      "Move::get_source_square & Move::get_destination_square", []() {
        for(int i = 0; i < 64; i++) {
          int source = 63 - i;
          int dest = i;
          if(source == dest) {
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

    TestFW::TestCase process_move_test_case("Process Move");
    process_move_test_case.tests.push_back(TestFW::Test("GameUtils::process_user_move", []() {

      GameState game_state;
      game_state.init();
      std::vector<std::string> moves{"e2e4", "d7d6", "d2d4", "g8f6", "b1c3", "g7g6", "c1e3", "f8g7", "d1d2", "c7c6", "f2f3", "b7b5", "g1e2", "b8d7", "e3h6", "g7h6", "d2h6", "c8b7", "a2a3", "e7e5", "e1c1", "d8e7", "c1b1", "a7a6", "e2c1", "e8c8", "c1b3", "e5d4", "d1d4", "c6c5", "d4d1", "d7b6", "g2g3", "c8b8", "b3a5", "b7a8", "f1h3", "d6d5", "h6f4", "b8a7", "h1e1", "d5d4", "c3d5", "b6d5", "e4d5", "e7d6", "d1d4", "c5d4", "e1e7", "a7b6", "f4d4", "b6a5", "b2b4", "a5a4", "d4c3", "d6d5", "e7a7", "a8b7", "a7b7", "d5c4", "c3f6", "a4a3", "f6a6", "a3b4", "c2c3", "b4c3", "a6a1", "c3d2", "a1b2", "d2d1", "h3f1", "d8d2", "b7d7", "d2d7", "f1c4", "b5c4", "b2h8", "d7d3", "h8a8", "c4c3", "a8a4", "d1e1", "f3f4", "f7f5", "b1c1", "d3d2", "a4a7"};
      for(auto& move : moves) {
        GameUtils::process_user_move(game_state, move);
        TFW_ASSERT_EQ(true, game_state.is_legal);
      }
                                           }));
    game_utils_unit_test.test_cases.push_back(process_move_test_case);

    game_utils_unit_test.run();
  }
}  // namespace Tests