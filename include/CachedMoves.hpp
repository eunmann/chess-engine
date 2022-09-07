#pragma once

#include <array>
#include <numeric>
#include <algorithm>

#include "CommonHeaders.hpp"
#include "BitBoardUtils.hpp"
#include "CapturesGeneration.hpp"

namespace CachedMoves {

    constexpr auto generate_knight_moves() noexcept -> std::array<BitBoard, Squares::NUM.value()> {
        std::array<BitBoard, Squares::NUM.value()> moves{};

        for (auto square = Squares::A1; square <= Squares::H8; square++) {
            const BitBoard bit_board = square.to_bit_board();
            moves[square.value()] = CapturesGeneration::get_knight_capture_positions(bit_board);
        }

        return moves;
    }

    constexpr auto generate_king_moves() noexcept -> std::array<BitBoard, Squares::NUM.value()> {
        std::array<BitBoard, Squares::NUM.value()> moves{};

        for (auto square = Squares::A1; square <= Squares::H8; square++) {
            const BitBoard bit_board = square.to_bit_board();
            moves[square.value()] = CapturesGeneration::get_king_capture_positions(bit_board);
        }

        return moves;
    }

    constexpr auto generate_pawn_moves() noexcept -> std::array<BitBoard, Squares::NUM.value() * Colors::NUM.value> {

        std::array<BitBoard, Squares::NUM.value() * Colors::NUM.value> moves{};

        for (auto square = Squares::A1; square <= Squares::H8; square++) {
            const BitBoard bit_board = square.to_bit_board();
            moves[square.value()] = CapturesGeneration::get_pawn_move_positions<Colors::WHITE>(bit_board);
            moves[square.value() + Squares::NUM.value()] = CapturesGeneration::get_pawn_move_positions<Colors::BLACK>(
                    bit_board);
        }

        return moves;
    }

    constexpr auto generate_pawn_captures() noexcept -> std::array<BitBoard, Squares::NUM.value() * Colors::NUM.value> {

        std::array<BitBoard, Squares::NUM.value() * Colors::NUM.value> moves{};

        for (auto square = Squares::A1; square <= Squares::H8; square++) {
            const BitBoard bit_board = square.to_bit_board();
            moves[square.value()] = CapturesGeneration::get_pawn_capture_positions<Colors::WHITE>(bit_board);
            moves[square.value() +
                  Squares::NUM.value()] = CapturesGeneration::get_pawn_capture_positions<Colors::BLACK>(
                    bit_board);
        }

        return moves;
    }

    extern const std::array<BitBoard, Squares::NUM.value()> KNIGHT;
    extern const std::array<BitBoard, Squares::NUM.value()> KING;
    extern const std::array<BitBoard, Squares::NUM.value() * Colors::NUM.value> PAWN_CAPTURES;
    extern const std::array<BitBoard, Squares::NUM.value() * Colors::NUM.value> PAWN;

    template<const Color color>
    constexpr auto get_pawn_capture_moves(const Square square) noexcept -> BitBoard {
        return PAWN_CAPTURES[Squares::NUM.value() * color.value + square.value()];
    }

    template<const Color color>
    constexpr auto get_pawn_moves(const Square square) noexcept -> BitBoard {
        return PAWN[Squares::NUM.value() * color.value + square.value()];
    }

    constexpr auto get_knight_moves(const Square square) noexcept -> BitBoard {
        return CachedMoves::KNIGHT[square.value()];
    }

    constexpr auto get_king_moves(const Square square) noexcept -> BitBoard {
        return CachedMoves::KING[square.value()];
    }
}