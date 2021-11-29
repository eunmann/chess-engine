#include "StringUtils.hpp"

std::vector<std::string> StringUtils::split(const std::string& str) {
    std::vector<std::string> split;
    std::string temp;
    for (size_t i = 0; i < str.size(); ++i) {
        char c = str[i];

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