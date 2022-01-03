#include "Tests.hpp"
#include "UCIUtils.hpp"
#include "CachedMoves.hpp"
#include "MagicBitBoards.hpp"


int main() {

  CachedMoves::init();
  MagicBitBoards::init();

  auto run_tests = false;
  if (run_tests) {
    Tests::run_tests();
  } else {
    UCIUtils::loop();
  }

  return 0;
}