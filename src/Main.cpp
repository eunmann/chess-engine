#include "Tests.hpp"
#include "UCIUtils.hpp"

int main() {

  setbuf(stdout, NULL);
  setbuf(stdin, NULL);

  auto run_tests = true;
  if (run_tests) {
    Tests::run_tests();
  } else {
    UCIUtils::loop();
  }

  return 0;
}