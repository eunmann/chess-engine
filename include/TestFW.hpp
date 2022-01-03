#pragma once

#include <functional>
#include <string>
#include <format>

#define TFW_ASSERT_EQ(expected, actual)                                                           \
  {                                                                                               \
    if ((expected) != (actual)) {                                                                 \
      std::string file_name = std::string(__FILE__);                                              \
      file_name = file_name.substr(file_name.find_last_of('\\') + 1);                             \
      std::string failure_message = std::format("[ERROR: {}:{}:{}] - ({} != {}) - ({} != {}))\n", \
      file_name,                                                                                  \
      __func__,                                                                                   \
      __LINE__,                                                                                   \
      #expected,                                                                                  \
      #actual,                                                                                    \
      std::to_string(expected),                                                                   \
      std::to_string(actual));                                                                    \
      throw failure_message;                                                                      \
    }                                                                                             \
}

namespace TestFW {

  class Test {
    public:
    Test(const std::string& description, const std::function<void()> test) noexcept;

    auto run() noexcept -> void;

    std::string description;
    std::function<void()> test;
  };

  class TestCase {
    public:
    TestCase(const std::string& description) noexcept;

    auto run() noexcept -> void;

    std::string description;
    std::vector<Test> tests;
  };

  class UnitTest {
    public:
    UnitTest(const std::string& description) noexcept;

    auto run() noexcept -> void;

    std::string description;
    std::vector<TestCase> test_cases;
  };
}  // namespace TestFW