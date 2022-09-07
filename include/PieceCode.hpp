
#pragma once

#include <cinttypes>
#include <array>

class PieceCode {
public:
    PieceCode() : PieceCode(0) {}

    constexpr explicit PieceCode(const int32_t value) noexcept: value(value) {}

    constexpr auto operator==(const PieceCode piece_code) const noexcept -> bool {
        return this->value == piece_code.value;
    }

    constexpr auto operator++(const int32_t value) noexcept -> PieceCode {
        this->value++;
        return *this;
    }

    constexpr auto operator<(const PieceCode piece_code) const noexcept -> bool {
        return this->value < piece_code.value;
    }

    constexpr auto operator<=(const PieceCode piece_code) const noexcept -> bool {
        return this->value <= piece_code.value;
    }

    int32_t value{};
};

namespace PieceCodes {
    constexpr PieceCode PAWN{0},
            KNIGHT{1},
            BISHOP{2},
            ROOK{3},
            QUEEN{4},
            KING{5},
            NUM{6};

    constexpr std::array<PieceCode, 6> ALL{PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING};

    auto to_char(PieceCode piece_code) noexcept -> char;

}  // namespace PieceCodes



