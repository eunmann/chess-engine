#include "TestFW.hpp"

#include "Definitions.hpp"
#include "BitBoardUtils.hpp"
#include <assert.h>

namespace Tests {
  auto add_shift_tests(TestFW::UnitTest& unit_tests) -> void {

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
    unit_tests.test_cases.push_back(shifts_test_case);
  }
}