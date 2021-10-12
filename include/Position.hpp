#pragma once

#include <inttypes.h>

#include <vector>

#include "BitBoard.hpp"
#include "Definitions.hpp"

class Position {
   public:
    Position();
    void init();
    void clear();

    BitBoard get_piece_bit_board(PieceCode piece_code) const;
    BitBoard get_color_bit_board(Color color) const;
    BitBoard get_bit_board(PieceCode piece_code, Color color) const;
    BitBoard get_occupied_bit_board() const;
    BitBoard get_empty_bit_board() const;
    BitBoard get_white_bit_board() const;
    BitBoard get_black_bit_board() const;
    BitBoard get_white_threaten() const;
    BitBoard get_black_threaten() const;

   private:
    BitBoard piece_positions[PieceCodes::NUM];
    BitBoard color_positions[Colors::NUM];
    BitBoard threaten_positions[Colors::NUM];
};

inline BitBoard Position::get_piece_bit_board(PieceCode piece_code) const {
    return this->piece_positions[piece_code];
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
