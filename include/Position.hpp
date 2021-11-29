#pragma once

#include <inttypes.h>

#include <array>
#include <vector>

#include "BitBoard.hpp"
#include "Definitions.hpp"

class Position {
   public:
    Position();
    void init();
    void clear();

    BitBoard get_piece_bit_board(PieceCode piece_code) const;
    void set_piece_bit_board(PieceCode piece_code, BitBoard bit_board);
    BitBoard get_color_bit_board(Color color) const;
    BitBoard get_bit_board(PieceCode piece_code, Color color) const;
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

inline BitBoard Position::get_piece_bit_board(PieceCode piece_code) const {
    return this->piece_positions[piece_code];
}

inline void Position::set_piece_bit_board(PieceCode piece_code, BitBoard bit_board) {
    this->piece_positions[piece_code] = bit_board;
}

inline BitBoard Position::get_color_bit_board(Color color) const {
    return this->color_positions[color];
}

inline BitBoard Position::get_bit_board(PieceCode piece_code, Color color) const {
    return this->get_piece_bit_board(piece_code) & this->get_color_bit_board(color);
}

inline BitBoard Position::get_occupied_bit_board() const {
    return this->color_positions[Colors::WHITE] | this->color_positions[Colors::BLACK];
}

inline BitBoard Position::get_empty_bit_board() const {
    return ~this->get_occupied_bit_board();
}

inline BitBoard Position::get_white_bit_board() const {
    return this->get_color_bit_board(Colors::WHITE);
}
inline BitBoard Position::get_black_bit_board() const {
    return this->get_color_bit_board(Colors::BLACK);
}

inline BitBoard Position::get_white_threaten() const {
    return this->threaten_positions[Colors::WHITE];
}

inline BitBoard Position::get_black_threaten() const {
    return this->threaten_positions[Colors::BLACK];
}
