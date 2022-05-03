#include "FormatUtils.hpp"

namespace FormatUtils {
    auto format_number(double num) noexcept -> std::string {
        char unit[][2] = {"p", "n", "u", "m", "", "K", "M", "G", "T"};

        int unit_index = 4;
        double abs_num = std::abs(num);
        if (abs_num < 1.0) {
            for (; unit_index > 0; unit_index--) {
                if (abs_num <= 1e-3) {
                    abs_num *= 1e3;
                } else {
                    break;
                }
            }
        } else {
            for (; unit_index < 8; unit_index++) {
                if (abs_num >= 1e3) {
                    abs_num *= 1e-3;
                } else {
                    break;
                }
            }
        }

        char buff[32];
        if (num < 0.0) {
            abs_num *= -1.0;
        }

        snprintf(buff, sizeof(buff), "%7.3f%s", abs_num, unit[unit_index]);
        std::string str(buff);

        if (unit_index == 4) {
            str.resize(str.size() - 1);
        }

        return str;
    }

    std::string format_time(int64_t time_ns) {
        char unit[][4] = {"ns", "us", "ms", "s", "min"};
        double ratio[] = {1000, 1000, 1000, 60, 60};

        double time_elapsed = time_ns;

        int i;
        for (i = 0; i < 4; i++) {
            if (time_elapsed >= ratio[i]) {
                time_elapsed /= ratio[i];
            } else {
                break;
            }
        }

        char buff[32];
        snprintf(buff, sizeof(buff), "%.3f %s", time_elapsed, unit[i]);
        return {buff};
    }
}