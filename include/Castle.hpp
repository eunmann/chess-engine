
#pragma once

#include <array>
#include <cinttypes>

class Castle {
public:
    constexpr explicit Castle(const int32_t value) noexcept: m_value(value) {}

    [[nodiscard]] constexpr auto value() const noexcept -> int32_t { return this->m_value; }

    constexpr auto operator==(const Castle castle) const noexcept -> bool {
        return this->m_value == castle.m_value;
    }

private:
    int32_t m_value;
};

namespace Castles {
    constexpr Castle NONE{0b000};
    constexpr Castle WHITE_KING{0b100};
    constexpr Castle WHITE_QUEEN{0b101};
    constexpr Castle BLACK_KING{0b110};
    constexpr Castle BLACK_QUEEN{0b111};
    constexpr std::array<Castle, 4> ALL{WHITE_KING, WHITE_QUEEN, BLACK_KING, BLACK_QUEEN};
}  // namespace Castles


