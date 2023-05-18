#include "../src/BitBoardUtils.hpp"
#include "../src/CommonHeaders.hpp"
#include "../src/TestFW.hpp"

namespace Tests {
auto add_conversion_tests(TestFW::UnitTest &unit_tests) -> void {
    TestFW::TestCase conversion_test_case("Conversions");
    conversion_test_case.tests.emplace_back(
        TestFW::Test("BitBoardUtils::bit_board_to_square", []() {
            auto bit_board = BitBoard(0b101ULL);
            int32_t counter = 0;
            do {
                auto expected_square = Square(counter);
                Square actual_square = BitBoardUtils::bit_board_to_square(bit_board);

                TFW_ASSERT_EQ(expected_square.value(), actual_square.value());

                counter++;
                bit_board <<= 1;
            } while (bit_board != BitBoards::EMPTY);
        }));
    conversion_test_case.tests.emplace_back(
        TestFW::Test("BitBoardUtils::square_to_bit_board", []() {
            auto square = Squares::A1;
            do {
                BitBoard expected_bit_board = BitBoard(0b1ULL) << square.value();
                BitBoard actual_bit_board = square.to_bit_board();

                TFW_ASSERT_EQ(expected_bit_board.value(), actual_bit_board.value());

                square++;
            } while (square < Squares::NUM);
        }));
    unit_tests.test_cases.push_back(conversion_test_case);
}
}  // namespace Tests