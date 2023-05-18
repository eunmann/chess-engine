#pragma once

#include <bit>
#include <string>

#include "CommonHeaders.hpp"

namespace BitBoardUtils {
auto print_position(BitBoard bit_board) noexcept -> void;

auto shift_bit_board(BitBoard bit_board, int32_t vertical, int32_t horizontal) noexcept -> BitBoard;

auto get_row(BitBoard bit_board) noexcept -> int32_t;

auto get_col(BitBoard bit_board) noexcept -> int32_t;

auto get_row_col(BitBoard bit_board, int32_t &row, int32_t &col) noexcept -> void;

auto get_tile_name(BitBoard bit_board) noexcept -> std::string;

constexpr auto get_count(const BitBoard bit_board) noexcept -> int32_t {
    return std::popcount(bit_board.value());
}

// Conversions
auto bit_board_to_square(BitBoard bit_board) noexcept -> Square;

// Checks

auto is_piece_in_col(BitBoard bit_board, int32_t col) noexcept -> bool;

constexpr auto is_piece_in_top_row(const BitBoard bit_board) noexcept -> bool {
    return BitBoards::ROW_8.overlaps(bit_board);
}

constexpr auto is_piece_in_bottom_row(const BitBoard bit_board) noexcept -> bool {
    return BitBoards::ROW_1.overlaps(bit_board);
}

constexpr auto is_piece_in_left_col(const BitBoard bit_board) noexcept -> bool {
    return BitBoards::COL_A.overlaps(bit_board);
}

constexpr auto is_piece_in_right_col(const BitBoard bit_board) noexcept -> bool {
    return BitBoards::COL_H.overlaps(bit_board);
}

auto for_each_set_square(const BitBoard bit_board, const auto &func) noexcept -> void {
    BitBoard temp_bit_board = bit_board;
    while (true) {
        const Square square = bit_board_to_square(temp_bit_board);
        if (square == Squares::INVALID) {
            break;
        }
        func(square);
        temp_bit_board &= square.to_bit_board().invert();
    };
}

constexpr auto for_each_bit_board(const BitBoard bit_board, const auto &func) noexcept -> void {
    BitBoard temp_bit_board = bit_board;
    while (true) {
        const Square square = BitBoardUtils::bit_board_to_square(temp_bit_board);
        if (square == Squares::INVALID) {
            break;
        }
        const BitBoard single_bit_board = square.to_bit_board();
        func(single_bit_board);
        temp_bit_board &= single_bit_board.invert();
    };
}

template <const int V, const int H>
constexpr auto shift_bit_board(const BitBoard bit_board) noexcept -> BitBoard {
    constexpr int shift = V * 8 + H;
    if constexpr (shift >= 0) {
        return bit_board << shift;
    } else {
        return bit_board >> shift * -1;
    }
}

constexpr auto row_from_index(const int32_t index) noexcept -> BitBoard {
    return BitBoards::ROW_1 << index * BOARD_DIM;
}

constexpr auto col_from_index(const int32_t index) noexcept -> BitBoard {
    return BitBoards::COL_A << index;
}

auto get_col(int32_t col_index) noexcept -> BitBoard;
}  // namespace BitBoardUtils