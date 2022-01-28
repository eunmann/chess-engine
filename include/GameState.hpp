#pragma once

#include <inttypes.h>

#include "Move.hpp"
#include "Position.hpp"
#include "BitMaskValue.hpp"
#include "BitBoardUtils.hpp"
#include "Assert.hpp"


class GameState {
  public:
  GameState() noexcept;

  auto init() noexcept -> void;

  template<const Color color>
  auto apply_move(const Move move) noexcept -> void {

    const BitBoard source_bit_board = move.get_source_bit_board();
    const BitBoard destination_bit_board = move.get_destination_bit_board();

    if (color != this->position.get_color(source_bit_board)) {
      this->set_is_legal(false);
      return;
    }

    const bool is_castle = move.is_castle();
    PieceCode piece_code = 0;

    if (is_castle) {

      piece_code = PieceCodes::KING;

      if (this->has_king_moved<color>() ||
        this->is_color_in_check<color>()) {
        this->set_is_legal(false);
        return;
      }
      constexpr Color opponent_color = color == Colors::WHITE ? Colors::BLACK : Colors::WHITE;

      constexpr auto can_queen_castle = [](const GameState& game_state) {
        constexpr BitBoard queen_castle = color == Colors::WHITE ? BitBoards::WHITE_QUEEN_CASTLE : BitBoards::BLACK_QUEEN_CASTLE;
        return !(game_state.has_rook_A_moved<color>() ||
          !game_state.position.is_empty(queen_castle) ||
          game_state.position.is_threaten<opponent_color>(queen_castle));
      };

      constexpr auto can_king_castle = [](const GameState& game_state) {
        constexpr BitBoard king_castle = color == Colors::WHITE ? BitBoards::WHITE_KING_CASTLE : BitBoards::BLACK_KING_CASTLE;
        return !(game_state.has_rook_H_moved<color>() ||
          !game_state.position.is_empty(king_castle) ||
          game_state.position.is_threaten<opponent_color>(king_castle));
      };

      const Castle castle = move.get_castle();
      if constexpr (color == Colors::WHITE) {
        if (castle == Castles::WHITE_KING) {
          if (!can_king_castle(*this)) {
            this->set_is_legal(false);
            return;
          }
          this->position.clear(BitBoards::WHITE_KING_START | BitBoards::WHITE_ROOK_H_START);
          this->position.add(PieceCodes::KING, color, BitBoards::WHITE_KING_KING_CASTLE);
          this->position.add(PieceCodes::ROOK, color, BitBoards::WHITE_ROOK_KING_CASTLE);
        } else {
          if (!can_queen_castle(*this)) {
            this->set_is_legal(false);
            return;
          }
          this->position.clear(BitBoards::WHITE_KING_START | BitBoards::WHITE_ROOK_A_START);
          this->position.add(PieceCodes::KING, color, BitBoards::WHITE_KING_QUEEN_CASTLE);
          this->position.add(PieceCodes::ROOK, color, BitBoards::WHITE_ROOK_QUEEN_CASTLE);
        }
      } else if constexpr (color == Colors::BLACK) {
        if (castle == Castles::BLACK_KING) {
          if (!can_king_castle(*this)) {
            this->set_is_legal(false);
            return;
          }
          this->position.clear(BitBoards::BLACK_KING_START | BitBoards::BLACK_ROOK_H_START);
          this->position.add(PieceCodes::KING, color, BitBoards::BLACK_KING_KING_CASTLE);
          this->position.add(PieceCodes::ROOK, color, BitBoards::BLACK_ROOK_KING_CASTLE);
        } else {
          if (!can_queen_castle(*this)) {
            this->set_is_legal(false);
            return;
          }
          this->position.clear(BitBoards::BLACK_KING_START | BitBoards::BLACK_ROOK_A_START);
          this->position.add(PieceCodes::KING, color, BitBoards::BLACK_KING_QUEEN_CASTLE);
          this->position.add(PieceCodes::ROOK, color, BitBoards::BLACK_ROOK_QUEEN_CASTLE);
        }
      }
    } else if (this->position.is_color_occupied<color>(destination_bit_board)) {
      this->set_is_legal(false);
      return;
    } else {
      if (move.is_promotion()) {
        piece_code = move.get_promotion();
      } else {
        piece_code = this->position.get_piece_type(source_bit_board);
      }

      constexpr int32_t starting_row = color == Colors::WHITE ? 1 : 6;
      constexpr int32_t forward_2_row = color == Colors::WHITE ? 3 : 4;

      if (piece_code == PieceCodes::PAWN &&
        BitBoardUtils::is_piece_in_row(source_bit_board, starting_row) &&
        BitBoardUtils::is_piece_in_row(destination_bit_board, forward_2_row)) {
        this->set_en_passant(BitBoardUtils::get_col(source_bit_board));
      }

      this->position.clear(source_bit_board | destination_bit_board);
      this->position.add(piece_code, color, destination_bit_board);
    }

    this->position.recompute_threaten();

    this->set_is_legal(!this->is_color_in_check<color>());

    this->set_white_to_move(color != Colors::WHITE);

    this->set_king_moved<color>(piece_code == PieceCodes::KING || is_castle);

    constexpr BitBoard rook_a_start = color == Colors::WHITE ? BitBoards::WHITE_ROOK_A_START : BitBoards::BLACK_ROOK_A_START;
    this->set_rook_A_moved<color>(piece_code == PieceCodes::ROOK && source_bit_board == rook_a_start);

    constexpr BitBoard rook_h_start = color == Colors::WHITE ? BitBoards::WHITE_ROOK_H_START : BitBoards::BLACK_ROOK_H_START;
    this->set_rook_H_moved<color>(piece_code == PieceCodes::ROOK && source_bit_board == rook_h_start);
  }

  auto has_king_moved(const Color color) const noexcept -> bool;
  auto is_color_in_check(const Color color) const noexcept -> bool;
  auto has_rook_A_moved(const Color color) const noexcept -> bool;
  auto has_rook_H_moved(const Color color) const noexcept -> bool;

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