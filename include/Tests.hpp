#pragma once

#include "TestFW.hpp"

namespace Tests {

  auto run_tests() noexcept -> void;

  auto add_conversion_tests(TestFW::UnitTest& unit_tests) -> void;
  auto add_shift_tests(TestFW::UnitTest& unit_tests) -> void;
  auto add_board_tests(TestFW::UnitTest& unit_tests) -> void;
  auto add_move_tests(TestFW::UnitTest& unit_tests) -> void;
  auto add_process_move_tests(TestFW::UnitTest& unit_tests) -> void;
  auto add_generate_move_tests(TestFW::UnitTest& unit_tests) -> void;
  auto add_move_search_tests(TestFW::UnitTest& unit_tests) -> void;
  auto add_magic_bit_boards_tests(TestFW::UnitTest& unit_tests) -> void;
  auto add_game_state_tests(TestFW::UnitTest& unit_tests) -> void;
}