#include "TestFW.hpp"

#include "CommonHeaders.hpp"
#include "BitBoardUtils.hpp"
#include <cassert>

namespace Tests {
    auto add_shift_tests(TestFW::UnitTest &unit_tests) -> void {

        TestFW::TestCase shifts_test_case("Shifts");
        shifts_test_case.tests.emplace_back(
                TestFW::Test("BitBoardUtils::shift_bit_board", []() {
                    const BitBoard start = BitBoard(0b10'0000'0000);

                    const BitBoard expected_left = BitBoard(0b100'0000'0000);
                    const BitBoard expected_right = BitBoard(0b001'0000'0000);
                    const BitBoard expected_up = BitBoard(0b10'0000'0000'0000'0000);
                    const BitBoard expected_down = BitBoard(0b10);
                    const BitBoard expected_up_left = BitBoard(0b100'0000'0000'0000'0000);
                    const BitBoard expected_up_right = BitBoard(0b001'0000'0000'0000'0000);
                    const BitBoard expected_down_left = BitBoard(0b100);
                    const BitBoard expected_down_right = BitBoard(0b001);

                    assert((start.shift<1, 0>() == expected_up));
                    assert((start.shift(1, 0) == expected_up));

                    assert((start.shift<-1, 0>() == expected_down));
                    assert((start.shift(-1, 0) == expected_down));

                    assert((start.shift<0, 1>() == expected_left));
                    assert((start.shift(0, 1) == expected_left));

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
        unit_tests.test_cases.push_back(shifts_test_case);
    }
}