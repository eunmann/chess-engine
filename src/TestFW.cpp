#include "TestFW.hpp"

#include <iostream>
#include <stdexcept>
#include <utility>

#include "Timer.hpp"

#include "color/color.hpp"

namespace TestFW {
    Test::Test(std::string description, std::function<void()> test) noexcept
            : description(std::move(description)), test(std::move(test)) {
    }

    auto Test::run() noexcept -> void {
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
        }
    }

    TestCase::TestCase(std::string description) noexcept
            : description(std::move(description)), tests() {
    }

    auto TestCase::run() noexcept -> void {
        printf("\t%s\n", this->description.c_str());
        for (auto &test: this->tests) {
            test.run();
        }
    }

    UnitTest::UnitTest(std::string description) noexcept
            : description(std::move(description)), test_cases() {
    }

    auto UnitTest::run() noexcept -> void {
        printf("TFW Unit Tests\n%s\n", this->description.c_str());
        for (auto &test_case: this->test_cases) {
            test_case.run();
        }
    }
}  // namespace TestFW