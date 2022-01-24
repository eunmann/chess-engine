#include "Tests.hpp"
#include "UCIUtils.hpp"
#include "CachedMoves.hpp"
#include "MagicBitBoards.hpp"
#include "Timer.hpp"


int main() {

  auto run_tests = true;
  if (run_tests) {
    Tests::run_tests();
  } else {
    UCIUtils::loop();
  }

  return 0;
}