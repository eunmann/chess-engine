#pragma once

#include <inttypes.h>

#include "Move.hpp"
#include "Position.hpp"
#include "BitMaskValue.hpp"

class GameState {
  public:
  GameState() noexcept;

  auto init() noexcept -> void;
  auto apply_move(const Move move) noexcept -> void;

  auto is_white_in_check() const noexcept -> bool;
  auto is_black_in_check() const noexcept -> bool;

  template <const Color color>
  auto is_color_in_check() const noexcept -> bool {
    constexpr Color opponent_color = color == Colors::WHITE ? Colors::BLACK : Colors::WHITE;
    const BitBoard king_bit_board = this->position.get_piece_color_bit_board<PieceCodes::KING, color>();
    return this->position.is_threaten<opponent_color>(king_bit_board);
  }

  auto is_white_to_move() const noexcept ->bool;
  auto set_white_to_move(const bool white_move) noexcept -> void;

  template <const Color color>
  auto has_king_moved() const noexcept -> bool {
    if constexpr (color == Colors::WHITE) {
      return this->m_flags.get_bits< GameState::MASK_1_BIT, GameState::WHITE_KING_MOVED_OFFSET>();
    } else if constexpr (color == Colors::BLACK) {
      return this->m_flags.get_bits<GameState::MASK_1_BIT, GameState::BLACK_KING_MOVED_OFFSET>();
    }
  }

  template <const Color color>
  auto set_king_moved(const bool did_move) noexcept -> void {
    if constexpr (color == Colors::WHITE) {
      this->m_flags.set_bits<GameState::MASK_1_BIT, GameState::WHITE_KING_MOVED_OFFSET>(did_move);
    } else if constexpr (color == Colors::BLACK) {
      this->m_flags.set_bits<GameState::MASK_1_BIT, GameState::BLACK_KING_MOVED_OFFSET>(did_move);
    }
  }

  template <const Color color>
  auto has_rook_A_moved() const noexcept -> bool {
    if constexpr (color == Colors::WHITE) {
      return this->m_flags.get_bits<GameState::MASK_1_BIT, GameState::WHITE_ROOK_A_OFFSET>();;
    } else if constexpr (color == Colors::BLACK) {
      return this->m_flags.get_bits<GameState::MASK_1_BIT, GameState::BLACK_ROOK_A_OFFSET>();
    }
  }

  template <const Color color>
  auto set_rook_A_moved(const bool did_move) noexcept -> void {
    if constexpr (color == Colors::WHITE) {
      this->m_flags.set_bits<GameState::MASK_1_BIT, GameState::WHITE_KING_MOVED_OFFSET>(did_move);
    } else if constexpr (color == Colors::BLACK) {
      this->m_flags.set_bits<GameState::MASK_1_BIT, GameState::BLACK_KING_MOVED_OFFSET>(did_move);
    }
  }

  template <const Color color>
  auto has_rook_H_moved() const noexcept -> bool {
    if constexpr (color == Colors::WHITE) {
      return this->m_flags.get_bits<GameState::MASK_1_BIT, GameState::WHITE_ROOK_H_OFFSET>();;
    } else if constexpr (color == Colors::BLACK) {
      return this->m_flags.get_bits<GameState::MASK_1_BIT, GameState::BLACK_ROOK_H_OFFSET>();
    }
  }

  template <const Color color>
  auto set_rook_H_moved(const bool did_move) noexcept -> void {
    if constexpr (color == Colors::WHITE) {
      this->m_flags.set_bits<GameState::MASK_1_BIT, GameState::WHITE_ROOK_H_OFFSET>(did_move);
    } else if constexpr (color == Colors::BLACK) {
      this->m_flags.set_bits<GameState::MASK_1_BIT, GameState::BLACK_ROOK_H_OFFSET>(did_move);
    }
  }

  auto set_is_legal(const bool is_legal) noexcept -> void;
  auto is_legal() const noexcept -> bool;

  auto set_en_passant(const int32_t pawn_ep) noexcept -> void;
  auto get_en_passant() const noexcept -> int32_t;

  Position position;
  private:
  BitMaskValue m_flags;
  static constexpr int32_t MASK_1_BIT = 0b1;
  static constexpr int32_t WHITE_TO_MOVE_OFFSET = 0;
  static constexpr int32_t WHITE_KING_MOVED_OFFSET = WHITE_TO_MOVE_OFFSET + 1;
  static constexpr int32_t WHITE_ROOK_A_OFFSET = WHITE_KING_MOVED_OFFSET + 1;
  static constexpr int32_t WHITE_ROOK_H_OFFSET = WHITE_ROOK_A_OFFSET + 1;
  static constexpr int32_t BLACK_KING_MOVED_OFFSET = WHITE_ROOK_H_OFFSET + 1;
  static constexpr int32_t BLACK_ROOK_A_OFFSET = BLACK_KING_MOVED_OFFSET + 1;
  static constexpr int32_t BLACK_ROOK_H_OFFSET = BLACK_ROOK_A_OFFSET + 1;
  static constexpr int32_t IS_LEGAL_OFFSET = BLACK_ROOK_H_OFFSET + 1;
  static constexpr int32_t MASK_4_BIT = 0b1111;
  static constexpr int32_t PAWN_EN_OFFSET = IS_LEGAL_OFFSET + 1;
};