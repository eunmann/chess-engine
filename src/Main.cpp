#include "Tests.hpp"
#include "UCIUtils.hpp"
#include "CachedMoves.hpp"
#include "MagicBitBoards.hpp"
#include "Timer.hpp"


int main() {

  MagicBitBoards::init();

  auto run_tests = true;
  if (run_tests) {
    Tests::run_tests();
  } else {
    UCIUtils::loop();
  }

  return 0;
}