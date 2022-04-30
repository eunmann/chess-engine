#pragma once

#include <array>
#include <numeric>
#include <algorithm>

#include "Definitions.hpp"
#include "BitBoardUtils.hpp"
#include "CapturesGeneration.hpp"

namespace CachedMoves {

    auto get_knight_moves(Square square) noexcept -> BitBoard;

    auto get_king_moves(Square square) noexcept -> BitBoard;

    constexpr auto generate_knight_moves() noexcept -> std::array<BitBoard, Squares::NUM> {
        std::array<BitBoard, Squares::NUM> moves{0};
        std::iota(moves.begin(), moves.end(), Squares::A1);
        std::transform(moves.begin(), moves.end(), moves.begin(), [](auto square) {
            const BitBoard bit_board = BitBoardUtils::square_to_bit_board(square);
            return CapturesGeneration::get_knight_capture_positions(bit_board);
        });
        return moves;
    }

    constexpr auto generate_king_moves() noexcept -> std::array<BitBoard, Squares::NUM> {
        std::array<BitBoard, Squares::NUM> moves{0};
        std::iota(moves.begin(), moves.end(), Squares::A1);
        std::transform(moves.begin(), moves.end(), moves.begin(), [](auto square) {
            const BitBoard bit_board = BitBoardUtils::square_to_bit_board(square);
            return CapturesGeneration::get_king_capture_positions(bit_board);
        });
        return moves;
    }

    constexpr auto generate_pawn_moves() noexcept -> std::array<BitBoard, Squares::NUM * Colors::NUM> {

        std::array<BitBoard, Squares::NUM * Colors::NUM> moves{0};

        auto begin = moves.begin();
        auto end = moves.begin() + Squares::NUM;
        std::iota(begin, end, Squares::A1);
        std::transform(begin, end, begin, [](auto square) {
            const BitBoard bit_board = BitBoardUtils::square_to_bit_board(square);
            return CapturesGeneration::get_pawn_move_positions<Colors::WHITE>(bit_board);
        });

        begin = moves.begin() + Squares::NUM;
        end = moves.end();
        std::iota(begin, end, Squares::A1);
        std::transform(begin, end, begin, [](auto square) {
            const BitBoard bit_board = BitBoardUtils::square_to_bit_board(square);
            return CapturesGeneration::get_pawn_move_positions<Colors::BLACK>(bit_board);
        });

        return moves;
    }

    constexpr auto generate_pawn_captures() noexcept -> std::array<BitBoard, Squares::NUM * Colors::NUM> {

        std::array<BitBoard, Squares::NUM * Colors::NUM> moves{0};

        auto begin = moves.begin();
        auto end = moves.begin() + Squares::NUM;
        std::iota(begin, end, Squares::A1);
        std::transform(begin, end, begin, [](auto square) {
            const BitBoard bit_board = BitBoardUtils::square_to_bit_board(square);
            return CapturesGeneration::get_pawn_capture_positions<Colors::WHITE>(bit_board);
        });

        begin = moves.begin() + Squares::NUM;
        end = moves.end();
        std::iota(begin, end, Squares::A1);
        std::transform(begin, end, begin, [](auto square) {
            const BitBoard bit_board = BitBoardUtils::square_to_bit_board(square);
            return CapturesGeneration::get_pawn_capture_positions<Colors::BLACK>(bit_board);
        });

        return moves;
    }

    extern const std::array<BitBoard, Squares::NUM> KNIGHT;
    extern const std::array<BitBoard, Squares::NUM> KING;
    extern const std::array<BitBoard, Squares::NUM * Colors::NUM> PAWN_CAPTURES;
    extern const std::array<BitBoard, Squares::NUM * Colors::NUM> PAWN;

    template<const Color color>
    auto get_pawn_capture_moves(const Square square) noexcept -> BitBoard {
        return PAWN_CAPTURES[Squares::NUM * color + square];
    }

    template<const Color color>
    auto get_pawn_moves(const Square square) noexcept -> BitBoard {
        return PAWN[Squares::NUM * color + square];
    }
}