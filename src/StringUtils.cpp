#include "StringUtils.hpp"

auto StringUtils::split(const std::string &str) noexcept -> std::vector<std::string> {
    std::vector<std::string> split;
    std::string temp;
    for (char c: str) {
        if (c == ' ') {
            split.push_back(temp);
            temp = "";
        } else {
            temp.push_back(c);
        }
    }
    split.push_back(temp);
    return split;
}