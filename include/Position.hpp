#pragma once

#include <inttypes.h>

#include <array>
#include <vector>

#include "Board.hpp"
#include "Definitions.hpp"

class Position {
public:
  Position();
  void init();
  void clear();

  auto get_piece_bit_board(const PieceCode piece_code) const->BitBoard;
  template <const PieceCode piece_code>
  auto get_piece_bit_board() const -> BitBoard {
    return this->piece_positions[piece_code];
  }

  auto get_color_bit_board(const Color color) const->BitBoard;
  template <const Color color>
  auto get_color_bit_board() const -> BitBoard {
    return this->color_positions[color];
  }
  auto get_piece_color_bit_board(const PieceCode piece_code,
                                 const Color color) const->BitBoard;
  template <const PieceCode piece_code, const Color color>
  auto get_piece_color_bit_board() const-> BitBoard {
    return this->get_piece_bit_board<piece_code>() &
      this->get_color_bit_board<color>();
  }

  auto get_color(const BitBoard bit_board) const->Color;
  auto get_piece_type(const BitBoard bit_board) const->PieceCode;

  auto get_occupied_bit_board() const->BitBoard;
  auto get_empty_bit_board() const->BitBoard;

  auto get_white_bit_board() const->BitBoard;
  auto get_black_bit_board() const->BitBoard;
  template <const Color color>
  auto get_bit_board() const -> BitBoard {
    if constexpr(color == Colors::WHITE) {
      return this->get_white_bit_board();
    } else {
      return this->get_black_bit_board();
    }
  }

  auto get_white_threaten() const->BitBoard;
  auto get_black_threaten() const->BitBoard;
  template <const Color color>
  auto get_threaten() const -> BitBoard {
    return this->threaten_positions[color];
  }

  auto clear(const BitBoard bit_board) -> void;
  auto add(const PieceCode piece_code, const Color color,
           const BitBoard bit_board) -> void;

  auto recompute_threaten() -> void;

  auto is_empty(const BitBoard bit_board) const -> bool;
  auto is_occupied(const BitBoard bit_board) const -> bool;

  auto is_white_occupied(const BitBoard bit_board) const -> bool;
  auto is_black_occupied(const BitBoard bit_board) const -> bool;
  template <const Color color>
  auto is_color_occupied(const BitBoard bit_board) const -> bool {
    return (this->get_color_bit_board<color>() & bit_board) != 0;
  }

  auto is_white_threaten(const BitBoard bit_board) const -> bool;
  auto is_black_threaten(const BitBoard bit_board) const -> bool;
  template <const Color color>
  auto is_threaten(const BitBoard bit_board) const -> bool {
    return (this->threaten_positions[color] & bit_board) != 0;
  }

  auto to_board() const->Board;

private:
  std::array<BitBoard, PieceCodes::NUM> piece_positions;
  std::array<BitBoard, Colors::NUM> color_positions;
  std::array<BitBoard, Colors::NUM> threaten_positions;
};