#include "Tests.hpp"
#include "UCIUtils.hpp"

int main() {

  // Do not buffer output or else SearchThread output does not print out instantly
  setbuf(stdout, NULL);
  setbuf(stdin, NULL);

  auto run_tests = false;
  if (run_tests) {
    Tests::run_tests();
  } else {
    UCIUtils::loop();
  }

  return 0;
}