#include "TestFW.hpp"

#include "Definitions.hpp"
#include "BitBoardUtils.hpp"
#include "Move.hpp"
#include <assert.h>

namespace Tests {
  auto add_move_tests(TestFW::UnitTest& unit_tests) -> void {
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

    move_test_case.tests.push_back(TestFW::Test("Move - Black Castle", []() {
      Move move(Squares::BLACK_KING_START, Squares::BLACK_KING_KING_CASTLE_END);
      move.set_castle(Castles::BLACK_KING);
      TFW_ASSERT_EQ(true, move.is_castle());
      TFW_ASSERT_EQ(Squares::BLACK_KING_START, BitBoardUtils::bit_board_to_square(move.get_source_bit_board()));
      TFW_ASSERT_EQ(Squares::BLACK_KING_KING_CASTLE_END, BitBoardUtils::bit_board_to_square(move.get_destination_bit_board()));

      move.set_destination_square(Squares::BLACK_KING_QUEEN_CASTLE_END);
      move.set_castle(Castles::BLACK_QUEEN);
      TFW_ASSERT_EQ(true, move.is_castle());
      TFW_ASSERT_EQ(Squares::BLACK_KING_START, BitBoardUtils::bit_board_to_square(move.get_source_bit_board()));
      TFW_ASSERT_EQ(Squares::BLACK_KING_QUEEN_CASTLE_END, BitBoardUtils::bit_board_to_square(move.get_destination_bit_board()));
      }));
    unit_tests.test_cases.push_back(move_test_case);
  }
}