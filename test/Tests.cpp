#include "../src/Tests.hpp"

#include <cassert>
#include <string>
#include <unordered_set>
#include <vector>

#include "../src/CachedCapturesGeneration.hpp"
#include "../src/GameUtils.hpp"
#include "../src/MoveSearch.hpp"
#include "../src/TestFW.hpp"

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
    Tests::add_game_state_tests(unit_tests);

    unit_tests.run();
}
}  // namespace Tests