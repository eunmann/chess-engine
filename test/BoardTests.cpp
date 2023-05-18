#include "../src/BitBoardUtils.hpp"
#include "../src/CommonHeaders.hpp"
#include "../src/Position.hpp"
#include "../src/TestFW.hpp"

namespace Tests {
auto add_board_tests(TestFW::UnitTest &unit_tests) -> void {
    TestFW::TestCase board_test_case("Board");
    board_test_case.tests.emplace_back("Position::to_board()", []() {
        Position position;
        position.add(PieceCodes::BISHOP, Colors::WHITE, Squares::A1.to_bit_board());
        Board board = position.to_board();

        TFW_ASSERT_EQ(BoardValues::BISHOP, board.positions[Squares::A1.value()]);

        position.add(PieceCodes::BISHOP, Colors::WHITE, Squares::E4.to_bit_board());
        position.add(PieceCodes::PAWN, Colors::BLACK, Squares::D3.to_bit_board());
        board = position.to_board();
        TFW_ASSERT_EQ(BoardValues::BISHOP, board.positions[Squares::E4.value()]);
        TFW_ASSERT_EQ(-1 * BoardValues::PAWN, board.positions[Squares::D3.value()]);
    });
    unit_tests.test_cases.push_back(board_test_case);
}
}  // namespace Tests