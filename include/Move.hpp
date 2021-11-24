#pragma once

#include <array>
#include <string>

#include "Definitions.hpp"

class Move {
 public:
  Move();
  Move(const Square source, const Square dest);
  Move(const int& move);

  auto set_source_square(const Square square) -> void;
  auto set_destination_square(const Square square) -> void;

  auto get_source_square() const -> Square;
  auto get_destination_square() const -> Square;

  auto get_source_bit_board() const -> BitBoard;
  auto get_destination_bit_board() const -> BitBoard;

  auto get_promotion() const -> PieceCode;
  auto get_en_passant() const -> int32_t;
  auto get_castle() const -> Castle;

  auto set_promotion(const PieceCode piece_code) -> void;
  auto set_en_passant(const int32_t column_index) -> void;
  auto set_castle(const Castle castle) -> void;

  auto is_promotion() const -> bool;
  auto is_en_passantable() const -> bool;
  auto is_castle() const -> bool;

  auto to_string() const -> std::string;

  auto operator==(const Move& move) const -> bool;

 private:
  uint32_t m_move = 0;
  static constexpr uint32_t SOURCE_OFFSET = 0;
  static constexpr uint32_t SOURCE_BIT_NUM = 6;

  static constexpr uint32_t DEST_OFFSET = SOURCE_OFFSET + SOURCE_BIT_NUM;
  static constexpr uint32_t DEST_BIT_NUM = 6;

  static constexpr uint32_t PROMO_OFFSET = DEST_OFFSET + DEST_BIT_NUM;
  static constexpr uint32_t PROMO_BIT_NUM = 3;

  static constexpr uint32_t EN_OFFSET = PROMO_OFFSET + PROMO_BIT_NUM;
  static constexpr uint32_t EN_BIT_NUM = 4;

  static constexpr uint32_t CASTLE_OFFSET = EN_OFFSET + EN_BIT_NUM;
  static constexpr uint32_t CASTLE_BIT_NUM = 3;

  static constexpr uint32_t MASK_6_BITS = 0b0011'1111;
  static constexpr uint32_t MASK_4_BITS = 0b0000'1111;
  static constexpr uint32_t MASK_3_BITS = 0b0000'0111;
};
