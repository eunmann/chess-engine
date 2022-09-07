
#pragma once

#include <cinttypes>
#include "BitBoard.hpp"
#include "Definitions.hpp"

class Square {

public:

    constexpr explicit Square(const int32_t value) noexcept: m_value(value) {
    }

    [[nodiscard]] constexpr auto value() const noexcept -> int { return this->m_value; }

    auto operator=(const int32_t value) noexcept -> Square & {
        this->m_value = value;
        return *this;
    }

    constexpr auto operator==(const Square square) const noexcept -> bool {
        return this->m_value == square.m_value;
    }

    constexpr auto operator<=(const Square square) const noexcept -> bool {
        return this->m_value <= square.m_value;
    }

    constexpr auto operator<(const Square square) const noexcept -> bool {
        return this->m_value < square.m_value;
    }

    constexpr auto operator++(const int32_t value) noexcept -> Square {
        this->m_value++;
        return *this;
    }

    constexpr auto operator-(const int32_t value) const noexcept -> Square {
        return Square(this->m_value - value);
    }

    [[nodiscard]] constexpr auto to_bit_board() const noexcept -> BitBoard {
        return BitBoard(0b1ULL << this->m_value);
    }

    static auto from_bit_board(const BitBoard bit_board) noexcept -> Square {
        if (bit_board == BitBoards::EMPTY) {
            return Square(-1); //Squares::INVALID
        }

        unsigned long index = 0;
        _BitScanForward64(&index, bit_board.value());
        return Square(index);
    }

    [[nodiscard]] constexpr auto col_index_of() const noexcept -> int32_t {
        return (this->m_value % BOARD_DIM);
    }

    [[nodiscard]] constexpr auto col_of() const noexcept -> BitBoard {
        return BitBoards::COL_A << this->col_index_of();
    }

    [[nodiscard]] constexpr auto row_index_of() const noexcept -> int32_t {
        return (this->m_value / BOARD_DIM);
    }

    [[nodiscard]] constexpr auto row_of() const noexcept -> BitBoard {
        return BitBoards::ROW_1 << (this->row_index_of() * BOARD_DIM);
    }

private:
    int32_t m_value{};
};

constexpr auto operator-(const int32_t value, const Square square) noexcept -> Square {
    return Square(value - square.value());
}

namespace Squares {

    static constexpr Square A1{0}, A2{8}, A3{16}, A4{24}, A5{32}, A6{40}, A7{48}, A8{56},
            B1{1}, B2{9}, B3{17}, B4{25}, B5{33}, B6{41}, B7{49}, B8{57},
            C1{2}, C2{10}, C3{18}, C4{26}, C5{34}, C6{42}, C7{50}, C8{58},
            D1{3}, D2{11}, D3{19}, D4{27}, D5{35}, D6{43}, D7{51}, D8{59},
            E1{4}, E2{13}, E3{20}, E4{28}, E5{36}, E6{44}, E7{52}, E8{60},
            F1{5}, F2{14}, F3{21}, F4{29}, F5{37}, F6{45}, F7{53}, F8{61},
            G1{6}, G2{15}, G3{22}, G4{30}, G5{38}, G6{46}, G7{54}, G8{62},
            H1{7}, H2{16}, H3{23}, H4{31}, H5{39}, H6{47}, H7{55}, H8{63},
            NUM{64},
            INVALID{-1};

    constexpr Square WHITE_KING_START = E1;
    constexpr Square BLACK_KING_START = E8;

    constexpr Square WHITE_KING_KING_CASTLE_END = G1;
    constexpr Square WHITE_KING_QUEEN_CASTLE_END = C1;

    constexpr Square BLACK_KING_KING_CASTLE_END = G8;
    constexpr Square BLACK_KING_QUEEN_CASTLE_END = C8;
}

