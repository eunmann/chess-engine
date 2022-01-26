#include "TestFW.hpp"

#include "Definitions.hpp"
#include "BitBoardUtils.hpp"
#include "Position.hpp"

namespace Tests {
  auto add_board_tests(TestFW::UnitTest& unit_tests) -> void {
    TestFW::TestCase board_test_case("Board");
    board_test_case.tests.push_back(TestFW::Test("Position::to_board()", []() {
      Position position;
      position.add(PieceCodes::BISHOP, Colors::WHITE, BitBoardUtils::square_to_bit_board(Squares::A1));
      Board board = position.to_board();

      TFW_ASSERT_EQ(BoardValues::BISHOP, board.positions[Squares::A1]);

      position.add(PieceCodes::BISHOP, Colors::WHITE, BitBoardUtils::square_to_bit_board(Squares::E4));
      position.add(PieceCodes::PAWN, Colors::BLACK, BitBoardUtils::square_to_bit_board(Squares::D3));
      board = position.to_board();
      TFW_ASSERT_EQ(BoardValues::BISHOP, board.positions[Squares::E4]);
      TFW_ASSERT_EQ(-1 * BoardValues::PAWN, board.positions[Squares::D3]);
      }));
    unit_tests.test_cases.push_back(board_test_case);
  }
}