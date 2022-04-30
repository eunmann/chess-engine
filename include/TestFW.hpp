#pragma once

#include <functional>
#include <string>
#include <format>
#include <stdexcept>

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
      throw std::runtime_error(failure_message);                                                  \
    }                                                                                             \
}

namespace TestFW {

    class Test {
    public:
        Test(std::string description, std::function<void()> test) noexcept;

        auto run() noexcept -> void;

        std::string description;
        std::function<void()> test;
    };

    class TestCase {
    public:
        explicit TestCase(std::string description) noexcept;

        auto run() noexcept -> void;

        std::string description;
        std::vector<Test> tests;
    };

    class UnitTest {
    public:
        explicit UnitTest(std::string description) noexcept;

        auto run() noexcept -> void;

        std::string description;
        std::vector<TestCase> test_cases;
    };
}  // namespace TestFW