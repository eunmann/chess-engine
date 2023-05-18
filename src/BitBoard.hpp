
#pragma once

#include <cinttypes>
#include <intrin.h>
#include "Color.hpp"

class BitBoard {

public:

    constexpr BitBoard() noexcept: BitBoard(0) {}

    constexpr explicit BitBoard(const int32_t value) noexcept: m_value(value) {}

    constexpr explicit BitBoard(const uint64_t value) noexcept: m_value(value) {}

    [[nodiscard]] constexpr auto invert() const noexcept -> BitBoard {
        return BitBoard(~this->m_value);
    }

    constexpr auto operator<<(const int32_t shift_amount) const noexcept -> BitBoard {
        return BitBoard(this->m_value << shift_amount);
    }

    constexpr auto operator>>(const int32_t shift_amount) const noexcept -> BitBoard {
        return BitBoard(this->m_value >> shift_amount);
    }

    constexpr auto operator&(const BitBoard bit_board) const noexcept -> BitBoard {
        return BitBoard(this->m_value & bit_board.m_value);
    }

    constexpr auto operator|(const BitBoard bit_board) const noexcept -> BitBoard {
        return BitBoard(this->m_value | bit_board.m_value);
    }

    [[nodiscard]] constexpr auto value() const noexcept -> uint64_t { return this->m_value; }

    [[nodiscard]] constexpr auto overlaps(const BitBoard bit_board) const noexcept -> bool {
        return (this->m_value & bit_board.m_value) != 0;
    }

    constexpr auto operator&=(const BitBoard bit_board) noexcept -> void {
        this->m_value &= bit_board.m_value;
    }

    constexpr auto operator|=(const BitBoard bit_board) noexcept -> void {
        this->m_value |= bit_board.m_value;
    }

    constexpr auto operator==(const BitBoard bit_board) const noexcept -> bool {
        return this->m_value == bit_board.m_value;
    }

    constexpr auto operator!=(const BitBoard bit_board) const noexcept -> bool {
        return !(*this == bit_board);
    }

    constexpr auto operator<<=(const int32_t shift_amount) noexcept -> void {
        this->m_value <<= shift_amount;
    }

    constexpr auto operator*(const bool value) const noexcept -> BitBoard {
        return BitBoard(this->m_value * value);
    }

    template<class T>
    constexpr auto operator*(T) const noexcept -> BitBoard = delete;

    constexpr auto operator*(const uint64_t value) const noexcept -> BitBoard {
        return BitBoard(this->m_value * value);
    }

    constexpr auto operator=(const int32_t value) noexcept -> BitBoard & {
        this->m_value = value;
        return *this;
    }

    constexpr auto operator=(const uint64_t value) noexcept -> BitBoard & {
        this->m_value = value;
        return *this;
    }

    template<const int V, const int H>
    [[nodiscard]] constexpr auto shift() const noexcept -> BitBoard {
        constexpr int shift = V * 8 + H;
        if constexpr (shift >= 0) {
            return *this << shift;
        } else {
            return *this >> (shift * -1);
        }
    }

    [[nodiscard]] constexpr auto shift(const int V, const int H) const noexcept -> BitBoard {
        int shift = V * 8 + H;
        if (shift >= 0) {
            return *this << shift;
        } else {
            return *this >> (shift * -1);
        }
    }

private:
    uint64_t m_value{};
};

constexpr auto operator*(const bool value, const BitBoard bit_board) noexcept -> BitBoard {
    return bit_board * value;
}

constexpr auto operator*(const uint64_t value, const BitBoard bit_board) noexcept -> BitBoard {
    return bit_board * value;
}

template<class T>
constexpr auto operator*(T, BitBoard) noexcept -> BitBoard = delete;

namespace BitBoards {

    // Entire Boards
    constexpr BitBoard EMPTY{0ULL};
    constexpr BitBoard ALL_SQUARES = EMPTY.invert();

    // Rows
    constexpr BitBoard ROW_1{0xFFULL};
    constexpr BitBoard ROW_2 = ROW_1 << 8;
    constexpr BitBoard ROW_3 = ROW_2 << 8;
    constexpr BitBoard ROW_4 = ROW_3 << 8;
    constexpr BitBoard ROW_5 = ROW_4 << 8;
    constexpr BitBoard ROW_6 = ROW_5 << 8;
    constexpr BitBoard ROW_7 = ROW_6 << 8;
    constexpr BitBoard ROW_8 = ROW_7 << 8;

    // Cols
    constexpr BitBoard COL_A{0x0101010101010101ULL};
    constexpr BitBoard COL_B = COL_A << 1;
    constexpr BitBoard COL_C = COL_B << 1;
    constexpr BitBoard COL_D = COL_C << 1;
    constexpr BitBoard COL_E = COL_D << 1;
    constexpr BitBoard COL_F = COL_E << 1;
    constexpr BitBoard COL_G = COL_F << 1;
    constexpr BitBoard COL_H = COL_G << 1;

    // Pawn
    constexpr BitBoard WHITE_PAWN_START_ROW = ROW_2;
    constexpr BitBoard WHITE_PAWN_FORWARD_TWO_ROW = ROW_4;
    constexpr BitBoard WHITE_PAWN_PROMOTION_ROW = ROW_8;
    constexpr BitBoard WHITE_PAWN_EN_PASSANT_ROW = ROW_6;
    constexpr BitBoard BLACK_PAWN_START_ROW = ROW_7;
    constexpr BitBoard BLACK_PAWN_FORWARD_TWO_ROW = ROW_5;
    constexpr BitBoard BLACK_PAWN_PROMOTION_ROW = ROW_1;
    constexpr BitBoard BLACK_PAWN_EN_PASSANT_ROW = ROW_3;

    template<class Color color>
    constexpr auto pawn_start_row() noexcept -> BitBoard {
        return color == Colors::WHITE ? WHITE_PAWN_START_ROW : BLACK_PAWN_START_ROW;
    }

    template<class Color color>
    constexpr auto pawn_forward_two_row() noexcept -> BitBoard {
        return color == Colors::WHITE ? WHITE_PAWN_FORWARD_TWO_ROW : BLACK_PAWN_FORWARD_TWO_ROW;
    }

    template<class Color color>
    constexpr auto pawn_promotion_row() noexcept -> BitBoard {
        return color == Colors::WHITE ? WHITE_PAWN_PROMOTION_ROW : BLACK_PAWN_PROMOTION_ROW;
    }

    template<class Color color>
    constexpr auto pawn_en_passant_row() noexcept -> BitBoard {
        return color == Colors::WHITE ? WHITE_PAWN_EN_PASSANT_ROW : BLACK_PAWN_EN_PASSANT_ROW;
    }

    template<class Color color>
    constexpr auto pawn_direction() noexcept -> int32_t {
        return color == Colors::WHITE ? 1 : -1;
    }

    // King Starting
    constexpr BitBoard WHITE_KING_START = ROW_1 & COL_E;
    constexpr BitBoard BLACK_KING_START = ROW_8 & COL_E;

    // Rook Starting
    constexpr BitBoard WHITE_ROOK_A_START = ROW_1 & COL_A;
    constexpr BitBoard WHITE_ROOK_H_START = ROW_1 & COL_H;
    constexpr BitBoard BLACK_ROOK_A_START = ROW_8 & COL_A;
    constexpr BitBoard BLACK_ROOK_H_START = ROW_8 & COL_H;

    // Castle Empty
    constexpr BitBoard WHITE_KING_CASTLE = ROW_1 & (COL_F | COL_G);
    constexpr BitBoard WHITE_QUEEN_CASTLE = ROW_1 & (COL_B | COL_C | COL_D);

    constexpr BitBoard BLACK_KING_CASTLE = ROW_8 & (COL_F | COL_G);
    constexpr BitBoard BLACK_QUEEN_CASTLE = ROW_8 & (COL_B | COL_C | COL_D);

    // King Castle End Positions
    constexpr BitBoard WHITE_KING_KING_CASTLE = ROW_1 & COL_G;
    constexpr BitBoard WHITE_ROOK_KING_CASTLE = ROW_1 & COL_F;
    constexpr BitBoard WHITE_KING_QUEEN_CASTLE = ROW_1 & COL_C;
    constexpr BitBoard WHITE_ROOK_QUEEN_CASTLE = ROW_1 & COL_D;

    constexpr BitBoard BLACK_KING_KING_CASTLE = ROW_8 & COL_G;
    constexpr BitBoard BLACK_ROOK_KING_CASTLE = ROW_8 & COL_F;
    constexpr BitBoard BLACK_KING_QUEEN_CASTLE = ROW_8 & COL_C;
    constexpr BitBoard BLACK_ROOK_QUEEN_CASTLE = ROW_8 & COL_D;

    constexpr BitBoard CENTER_4_SQUARES = (ROW_4 | ROW_5) & (COL_D | COL_E);
    constexpr BitBoard CENTER_16_SQUARES = (ROW_3 | ROW_4 | ROW_5 | ROW_6) & (COL_C | COL_D | COL_E | COL_F);

    constexpr BitBoard EDGES = ROW_1 | ROW_8 | COL_A | COL_H;
}  // namespace BitBoards



