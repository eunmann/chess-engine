
#pragma once

#include <cinttypes>

class Color {
public:
    constexpr explicit Color(const int32_t value) noexcept: value(value) {}

    constexpr auto operator==(const Color color) const noexcept -> bool {
        return this->value == color.value;
    }

    int32_t value;
};

namespace Colors {
    constexpr Color WHITE{0},
            BLACK{1},
            NUM{2};

    template<const Color color>
    constexpr auto opponent_color() noexcept -> Color {
        return color == Colors::WHITE ? Colors::BLACK : Colors::WHITE;
    }

    auto bool_to_color(bool white_to_move) noexcept -> Color;
};  // namespace Colors

