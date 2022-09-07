
#pragma once

#include <cinttypes>

class PieceValue {
public:
    constexpr explicit PieceValue(const int32_t value) noexcept: m_value(value) {}

    [[nodiscard]] constexpr auto value() const noexcept -> int32_t { return this->m_value; }

private:
    int32_t m_value;
};

namespace PieceValues {
    constexpr PieceValue PAWN{100},
            KNIGHT{300},
            BISHOP{300},
            ROOK{500},
            QUEEN{900},
            KING{300000},
            POS_INFINITY{2 * KING.value()},
            NEG_INFINITY{-1 * POS_INFINITY.value()};
};


