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

    BitBoard get_piece_bit_board(PieceCode piece_code) const;
    void set_piece_bit_board(PieceCode piece_code, BitBoard bit_board);
    BitBoard get_piece_color_bit_board(PieceCode piece_code, Color color) const;

    BitBoard get_color_bit_board(Color color) const;

    BitBoard get_occupied_bit_board() const;
    BitBoard get_empty_bit_board() const;

    BitBoard get_white_bit_board() const;
    BitBoard get_black_bit_board() const;

    BitBoard get_white_threaten() const;
    BitBoard get_black_threaten() const;

   private:
    std::array<BitBoard, PieceCodes::NUM> piece_positions;
    std::array<BitBoard, Colors::NUM> color_positions;
    std::array<BitBoard, Colors::NUM> threaten_positions;
};