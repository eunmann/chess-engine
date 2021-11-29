#pragma once

#include <array>
#include <string>

#include "Definitions.hpp"
#include "BitMaskValue.hpp"

class Move {
  public:
  Move() noexcept;
  Move(const Square source, const Square dest) noexcept;
  Move(const int& move) noexcept;

  auto set_source_square(const Square square) noexcept -> void;
  auto set_destination_square(const Square square) noexcept -> void;

  auto get_source_square() const noexcept ->Square;
  auto get_destination_square() const noexcept ->Square;

  auto get_source_bit_board() const noexcept ->BitBoard;
  auto get_destination_bit_board() const noexcept ->BitBoard;

<<<<<<< HEAD
    Move& back_ref();
    int32_t size();
    Move& operator[](int32_t index);
    void add_moves(BitBoard bit_board);
=======
  auto get_promotion() const noexcept ->PieceCode;
  auto get_en_passant() const noexcept ->int32_t;
  auto get_castle() const noexcept ->Castle;
>>>>>>> good

  auto set_promotion(const PieceCode piece_code) noexcept -> void;
  auto set_en_passant(const int32_t column_index) noexcept -> void;
  auto set_castle(const Castle castle) noexcept -> void;

  auto is_promotion() const noexcept -> bool;
  auto is_en_passantable() const noexcept -> bool;
  auto is_castle() const noexcept -> bool;

  auto to_string() const noexcept ->std::string;

  auto operator==(const Move& move) const noexcept -> bool;

  private:
  BitMaskValue m_move;
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
