#include "TestFW.hpp"

#include "Definitions.hpp"
#include "BitBoardUtils.hpp"

namespace Tests {
    auto add_conversion_tests(TestFW::UnitTest &unit_tests) -> void {
        TestFW::TestCase conversion_test_case("Conversions");
        conversion_test_case.tests.emplace_back(
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
        conversion_test_case.tests.emplace_back(
                TestFW::Test("BitBoardUtils::square_to_bit_board", []() {
                    Square square = 0;
                    do {
                        BitBoard expected_bit_board = 0b1ULL << square;
                        BitBoard actual_bit_board = BitBoardUtils::square_to_bit_board(square);

                        TFW_ASSERT_EQ(expected_bit_board, actual_bit_board);

                        square++;
                    } while (square < 64);
                }));
        unit_tests.test_cases.push_back(conversion_test_case);
    }
}