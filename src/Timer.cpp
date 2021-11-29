#include "Timer.hpp"

#include <cstdio>
#include "FormatUtils.hpp"

Timer::Timer(std::string message) noexcept : message(message),
s(std::chrono::steady_clock::now()),
e(s) {
}

Timer::Timer() noexcept : message(""),
s(std::chrono::steady_clock::now()),
e(s) {
}

auto Timer::start() noexcept -> void {
  this->s = std::chrono::steady_clock::now();
}

auto Timer::end() noexcept -> void {
  this->e = std::chrono::steady_clock::now();
}

auto Timer::get_time_elapsed() const noexcept -> int64_t {
  return std::chrono::duration_cast<std::chrono::nanoseconds>(this->e - this->s).count();
}

auto Timer::print() const noexcept -> void {
  printf("[ %s | %s ]\n", this->message.c_str(), this->time_string().c_str());
}

auto Timer::print(const std::string& message) const noexcept -> void {
  printf("[ %s | %s ]\n", message.c_str(), this->time_string().c_str());
}

auto Timer::time_string() const noexcept -> std::string {
  return FormatUtils::format_time(this->get_time_elapsed());
}