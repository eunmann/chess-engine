#include "Tests.hpp"

#include <assert.h>

#include "GameUtils.hpp"
#include "TestFW.hpp"

namespace Tests {
  auto run_tests() -> void {
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
      for(int i = 0; i < 64; i++) {
        Move move(i, 0);
        assert(i == move.get_source_square());
        assert(!move.is_castle());
        assert(!move.is_en_passantable());
        assert(!move.is_promotion());
      }
                                   }));
    move_test_case.tests.push_back(
      TestFW::Test("Move::get_destination_square", []() {
        for(int i = 0; i < 64; i++) {
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
          Move move(source, dest);
          assert(source == move.get_source_square());
          assert(dest == move.get_destination_square());
          assert(!move.is_castle());
          assert(!move.is_en_passantable());
          assert(!move.is_promotion());
        }
      }));
    move_test_case.tests.push_back(TestFW::Test("Move::get_en_passant", []() {
      Move move(1, 1);
      move.set_en_passant(3);
      TFW_ASSERT_EQ(true, move.is_en_passantable());
      TFW_ASSERT_EQ(3, move.get_en_passant());
                                   }));
    move_test_case.tests.push_back(TestFW::Test("Move::get_castle", []() {
      Move move(1, 1);
      move.set_castle(Castles::WHITE_QUEEN);
      TFW_ASSERT_EQ(true, move.is_castle());
      TFW_ASSERT_EQ(Castles::WHITE_QUEEN, move.get_castle());
                                   }));
    move_test_case.tests.push_back(TestFW::Test("Move::get_promotion", []() {
      Move move(1, 1);
      TFW_ASSERT_EQ(false, move.is_promotion());
      move.set_promotion(PieceCodes::BISHOP);
      TFW_ASSERT_EQ(true, move.is_promotion());
      TFW_ASSERT_EQ(PieceCodes::BISHOP, move.get_promotion());
                                   }));
    game_utils_unit_test.test_cases.push_back(move_test_case);

    game_utils_unit_test.run();
  }
}  // namespace Tests