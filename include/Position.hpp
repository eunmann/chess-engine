#pragma once

#include <cinttypes>

#include <array>
#include <vector>

#include "Board.hpp"
#include "CommonHeaders.hpp"

class Position {
public:
    Position() noexcept;

    auto init() noexcept -> void;

    auto clear() noexcept -> void;

    [[nodiscard]] auto get_piece_bit_board(PieceCode piece_code) const noexcept -> BitBoard;

    template<const PieceCode piece_code>
    [[nodiscard]] auto get_piece_bit_board() const noexcept -> BitBoard {
        return this->piece_positions[piece_code.value];
    }

    [[nodiscard]] auto get_color_bit_board(Color color) const noexcept -> BitBoard;

    template<const Color color>
    [[nodiscard]] auto get_color_bit_board() const noexcept -> BitBoard {
        return this->color_positions[color.value];
    }

    [[nodiscard]] auto get_piece_color_bit_board(PieceCode piece_code, Color color) const noexcept -> BitBoard;

    template<const PieceCode piece_code, const Color color>
    [[nodiscard]] auto get_piece_color_bit_board() const noexcept -> BitBoard {
        return this->get_piece_bit_board<piece_code>() &
               this->get_color_bit_board<color>();
    }

    [[nodiscard]] auto get_color(BitBoard bit_board) const noexcept -> Color;

    [[nodiscard]] auto get_piece_type(BitBoard bit_board) const noexcept -> PieceCode;

    [[nodiscard]] auto get_occupied_bit_board() const noexcept -> BitBoard;

    [[nodiscard]] auto get_empty_bit_board() const noexcept -> BitBoard;

    [[nodiscard]] auto get_white_bit_board() const noexcept -> BitBoard;

    [[nodiscard]] auto get_black_bit_board() const noexcept -> BitBoard;

    template<const Color color>
    [[nodiscard]] auto get_bit_board() const noexcept -> BitBoard {
        if constexpr (color == Colors::WHITE) {
            return this->get_white_bit_board();
        } else {
            return this->get_black_bit_board();
        }
    }

    [[nodiscard]] auto get_white_threaten() const noexcept -> BitBoard;

    [[nodiscard]] auto get_black_threaten() const noexcept -> BitBoard;

    template<const Color color>
    [[nodiscard]] auto get_threaten() const noexcept -> BitBoard {
        return this->threaten_positions[color.value];
    }

    auto clear(BitBoard bit_board) noexcept -> void;

    auto add(PieceCode piece_code, Color color, BitBoard bit_board) noexcept -> void;

    auto recompute_threaten() noexcept -> void;

    [[nodiscard]] auto is_empty(BitBoard bit_board) const noexcept -> bool;

    [[nodiscard]] auto is_occupied(BitBoard bit_board) const noexcept -> bool;

    [[nodiscard]] auto is_white_occupied(BitBoard bit_board) const noexcept -> bool;

    [[nodiscard]] auto is_black_occupied(BitBoard bit_board) const noexcept -> bool;

    template<const Color color>
    [[nodiscard]] auto is_color_occupied(const BitBoard bit_board) const noexcept -> bool {
        return this->get_color_bit_board<color>().overlaps(bit_board);
    }

    [[nodiscard]] auto is_white_threaten(BitBoard bit_board) const noexcept -> bool;

    [[nodiscard]] auto is_black_threaten(BitBoard bit_board) const noexcept -> bool;

    template<const Color color>
    [[nodiscard]] auto is_threaten(const BitBoard bit_board) const noexcept -> bool {
        return this->threaten_positions[color.value].overlaps(bit_board);
    }

    [[nodiscard]] auto is_threaten(BitBoard bit_board, Color color) const noexcept -> bool;

    [[nodiscard]] auto to_board() const noexcept -> Board;

private:
    std::array<BitBoard, PieceCodes::NUM.value> piece_positions{};
    std::array<BitBoard, Colors::NUM.value> color_positions{};
    std::array<BitBoard, Colors::NUM.value> threaten_positions{};
};