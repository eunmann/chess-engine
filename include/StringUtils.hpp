#pragma once

#include <string>
#include <vector>

namespace StringUtils {
    auto split(const std::string &str) noexcept -> std::vector<std::string>;
}