#include "TestFW.hpp"

namespace TestFW {
Test::Test(const std::string& description, const std::function<void()> test)
    : description(description), test(test) {}

auto Test::run() -> void {
  printf("\t\tTest: %s\n", this->description.c_str());
  this->test();
}

TestCase::TestCase(const std::string& description)
    : description(description), tests() {}

auto TestCase::run() -> void {
  printf("\tTest Case: %s\n", this->description.c_str());
  for (auto& test : this->tests) {
    test.run();
  }
}

UnitTest::UnitTest(const std::string& description)
    : description(description), test_cases() {}

auto UnitTest::run() -> void {
  printf("Unit Test: %s\n", this->description.c_str());
  for (auto& test_case : this->test_cases) {
    test_case.run();
  }
}
}  // namespace TestFW