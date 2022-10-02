#include "TestFW.hpp"

#include <iostream>
#include <stdexcept>
#include <utility>

#include "Timer.hpp"

#include "color/color.hpp"
#include "FormatUtils.hpp"

namespace TestFW {
    Test::Test(std::string description, std::function<void()> test) noexcept
            : description(std::move(description)), test(std::move(test)) {
    }

    auto Test::run() noexcept -> TestRunResult {
        printf("\t\t%s", this->description.c_str());
        Timer timer;
        try {
            this->test();
            timer.end();
            std::cout << " - " << dye::green("PASSED ") << timer.time_string() << "\n";
        } catch (std::runtime_error &error) {
            timer.end();
            std::cout << " - " << dye::red("FAILED ") << timer.time_string() << "\n";
            printf("\t\t\t%s\n", error.what());
            return {0, 1, timer.get_time_elapsed()};
        }

        return {1, 1, timer.get_time_elapsed()};
    }

    TestCase::TestCase(std::string description) noexcept
            : description(std::move(description)), tests() {
    }

    auto TestCase::run() noexcept -> TestRunResult {
        printf("\t%s - %llu Tests\n", this->description.c_str(), this->tests.size());
        auto test_case_results = TestRunResult();
        for (auto &test: this->tests) {
            test_case_results += test.run();
        }
        return test_case_results;
    }

    UnitTest::UnitTest(std::string description) noexcept
            : description(std::move(description)), test_cases() {
    }

    auto UnitTest::run() noexcept -> void {
        printf("TFW Unit Tests\n%s - %llu Testcases\n", this->description.c_str(), this->test_cases.size());
        auto unit_test_results = TestRunResult();
        for (auto &test_case: this->test_cases) {
            unit_test_results += test_case.run();
        }

        auto time_string = FormatUtils::format_time(unit_test_results.time_elapsed);
        printf("Results - %llu/%llu - %s\n", unit_test_results.succeeded_count, unit_test_results.total_count,
               time_string.c_str());
    }
}  // namespace TestFW