#include "Timer.hpp"

#include <cstdio>
#include <utility>
#include "FormatUtils.hpp"

Timer::Timer(std::string message) noexcept: m_message(std::move(message)),
                                            m_start_timestamp(std::chrono::high_resolution_clock::now()),
                                            m_end_timestamp(m_start_timestamp) {
}

Timer::Timer() noexcept: m_message(),
                         m_start_timestamp(std::chrono::steady_clock::now()),
                         m_end_timestamp(m_start_timestamp) {
}

auto Timer::start() noexcept -> void {
    this->m_start_timestamp = std::chrono::high_resolution_clock::now();
}

auto Timer::end() noexcept -> void {
    this->m_end_timestamp = std::chrono::high_resolution_clock::now();
}

auto Timer::get_time_elapsed() const noexcept -> int64_t {
    return std::chrono::duration_cast<std::chrono::nanoseconds>(
            this->m_end_timestamp - this->m_start_timestamp).count();
}

auto Timer::print() const noexcept -> void {
    printf("[ %s | %s ]\n", this->m_message.c_str(), this->time_string().c_str());
}

auto Timer::print(const std::string &message) const noexcept -> void {
    printf("[ %s | %s ]\n", message.c_str(), this->time_string().c_str());
}

auto Timer::time_string() const noexcept -> std::string {
    return FormatUtils::format_time(this->get_time_elapsed());
}
