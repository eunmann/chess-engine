#pragma once

#include <inttypes.h>

#include <array>
#include <vector>

#include "Definitions.hpp"

class Position {
   public:
    Position();
    void init();
    void clear();

    auto get_piece_bit_board(PieceCode piece_code) const -> BitBoard;
    auto get_color_bit_board(Color color) const -> BitBoard;
    auto get_piece_color_bit_board(PieceCode piece_code, Color color) const -> BitBoard;

    auto get_color(const BitBoard bit_board) const -> Color;
    auto get_piece_type(const BitBoard bit_board) const -> PieceCode;

    auto get_occupied_bit_board() const -> BitBoard;
    auto get_empty_bit_board() const -> BitBoard;

    auto get_white_bit_board() const -> BitBoard;
    auto get_black_bit_board() const -> BitBoard;

    auto get_white_threaten() const -> BitBoard;
    auto get_black_threaten() const -> BitBoard;

    auto clear(const BitBoard bit_board) -> void;
    auto add(const PieceCode piece_code, const Color color, const BitBoard bit_board) -> void;

    auto recompute() -> void;

    auto is_empty(const BitBoard bit_board) const -> bool;
    auto is_occupied(const BitBoard bit_board) const -> bool;

   private:
    std::array<BitBoard, PieceCodes::NUM> piece_positions;
    std::array<BitBoard, Colors::NUM> color_positions;
    std::array<BitBoard, Colors::NUM> threaten_positions;
};