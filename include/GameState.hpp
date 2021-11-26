#pragma once

#include <inttypes.h>

#include "Move.hpp"
#include "Position.hpp"

class GameState {
public:
  GameState() noexcept;

  auto init() noexcept -> void;
  auto apply_move(const Move move) noexcept -> void;

  auto is_white_in_check() const noexcept -> bool;
  auto is_black_in_check() const noexcept -> bool;
  template <const Color color>
  auto is_color_in_check() const noexcept -> bool {
    constexpr Color opponent_color =
      color == Colors::WHITE ? Colors::BLACK : Colors::WHITE;
    const BitBoard king_bit_board =
      this->position.get_piece_color_bit_board<PieceCodes::KING, color>();
    return this->position.is_threaten<opponent_color>(king_bit_board);
  }

  template <const Color color>
  auto has_king_moved() const noexcept -> bool {
    if constexpr(color == Colors::WHITE) {
      return this->white_king_moved;
    } else {
      return this->black_king_moved;
    }
  }

  template <const Color color>
  auto has_rook_A_moved() const noexcept -> bool {
    if constexpr(color == Colors::WHITE) {
      return this->white_rook_A_moved;
    } else {
      return this->black_rook_A_moved;
    }
  }

  template <const Color color>
  auto has_rook_H_moved() const noexcept -> bool {
    if constexpr(color == Colors::WHITE) {
      return this->white_rook_H_moved;
    } else {
      return this->black_rook_H_moved;
    }
  }

  Position position;
  // TODO(EMU): Mask this a bit mask instead
  bool white_to_move;
  bool white_king_moved;
  bool white_rook_A_moved;
  bool white_rook_H_moved;
  bool black_king_moved;
  bool black_rook_A_moved;
  bool black_rook_H_moved;
  bool is_legal;

  int32_t pawn_ep;
};