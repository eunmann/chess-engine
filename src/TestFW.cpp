#include "TestFW.hpp"

#include <iostream>

#include "color/color.hpp"

namespace TestFW {
  Test::Test(const std::string& description, const std::function<void()> test) noexcept
    : description(description), test(test) {}

  auto Test::run() noexcept -> void {
    printf("\t\t%s", this->description.c_str());
    try {
      this->test();
    } catch(std::string failure_message) {
      std::cout << " - " << dye::red("FAILED") << "\n";
      printf("\t\t\t%s\n", failure_message.c_str());
      return;
    }

    std::cout << " - " << dye::green("PASSED") << "\n";
  }

  TestCase::TestCase(const std::string& description) noexcept
    : description(description), tests() {}

  auto TestCase::run() noexcept -> void {
    printf("\t%s\n", this->description.c_str());
    for(auto& test : this->tests) {
      test.run();
    }
  }

  UnitTest::UnitTest(const std::string& description) noexcept
    : description(description), test_cases() {}

  auto UnitTest::run() noexcept -> void {
    printf("TFW Unit Tests\n%s\n", this->description.c_str());
    for(auto& test_case : this->test_cases) {
      test_case.run();
    }
  }
}  // namespace TestFW