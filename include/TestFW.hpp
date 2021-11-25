#pragma once

#include <functional>
#include <string>

#define TFW_ASSERT_EQ(expected, actual)                               \
  {                                                                   \
    if (expected != actual) {                                         \
      std::string failure_message = "[ERROR:";                        \
      std::string file_name = std::string(__FILE__);                  \
      file_name = file_name.substr(file_name.find_last_of('\\') + 1); \
      failure_message += file_name;                                   \
      failure_message += ":";                                         \
      failure_message += __func__;                                    \
      failure_message += ":";                                         \
      failure_message += __LINE__;                                    \
      failure_message += "] - Expected[";                             \
      failure_message += #expected;                                   \
      failure_message += "]='";                                       \
      failure_message += std::to_string(expected);                    \
      failure_message += "' Actual[";                                 \
      failure_message += #actual;                                     \
      failure_message += "]='";                                       \
      failure_message += std::to_string(actual);                      \
      failure_message += "'";                                         \
      throw failure_message;                                          \
    }                                                                 \
  }

namespace TestFW {

  class Test {
  public:
    Test(const std::string& description, const std::function<void()> test);

    auto run() -> void;

    std::string description;
    std::function<void()> test;
  };

  class TestCase {
  public:
    TestCase(const std::string& description);

    auto run() -> void;

    std::string description;
    std::vector<Test> tests;
  };

  class UnitTest {
  public:
    UnitTest(const std::string& description);

    auto run() -> void;

    std::string description;
    std::vector<TestCase> test_cases;
  };
}  // namespace TestFW