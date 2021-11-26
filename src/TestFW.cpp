#include "TestFW.hpp"

#include <iostream>

#include "Timer.hpp"

#include "color/color.hpp"

namespace TestFW {
  Test::Test(const std::string& description, const std::function<void()> test) noexcept
    : description(description), test(test) {}

  auto Test::run() noexcept -> void {
    printf("\t\t%s", this->description.c_str());
    Timer timer;
    try {
      this->test();
      timer.end();
    } catch(std::string failure_message) {
      timer.end();
      std::cout << " - " << dye::red("FAILED ") << timer.time_string() << "\n";
      printf("\t\t\t%s\n", failure_message.c_str());
      return;
    }

    std::cout << " - " << dye::green("PASSED ") << timer.time_string() << "\n";
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