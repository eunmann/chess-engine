#pragma once

#include <string>
#include <stdexcept>

namespace FormatUtils {
    auto format_number(double num) noexcept -> std::string;

    template<typename ... Args>
    auto format(const std::string &format, Args ... args) noexcept -> std::string {
        int size = snprintf(nullptr, 0, format.c_str(), args ...) + 1;
        if (size <= 0) {
            throw std::runtime_error("String format error. size <= 0");
        }
        char buff[size];
        snprintf(buff, size, format.c_str(), args ...);
        return std::string(buff, buff + size - 1);
    }

    std::string format_time(int64_t time_ns);
}