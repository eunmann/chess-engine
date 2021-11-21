#pragma once

#include <functional>
#include <string>

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