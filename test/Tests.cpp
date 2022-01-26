#include "Tests.hpp"

#include <assert.h>
#include <vector>
#include <unordered_set>
#include <string>

#include "GameUtils.hpp"
#include "MoveSearch.hpp"
#include "TestFW.hpp"
#include "CachedCapturesGeneration.hpp"

namespace Tests {


  auto run_tests() noexcept -> void {

    TestFW::UnitTest unit_tests("Chess Engine");

    Tests::add_conversion_tests(unit_tests);
    Tests::add_shift_tests(unit_tests);
    Tests::add_board_tests(unit_tests);
    Tests::add_move_tests(unit_tests);
    Tests::add_process_move_tests(unit_tests);
    Tests::add_generate_move_tests(unit_tests);
    Tests::add_move_search_tests(unit_tests);
    Tests::add_magic_bit_boards_tests(unit_tests);

    unit_tests.run();
  }
}  // namespace Tests