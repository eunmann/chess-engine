#include "Move.hpp"

#include <assert.h>

#include "GameUtils.hpp"

Move::Move() noexcept : m_move(0) {}

Move::Move(const Square source, const Square dest) noexcept : m_move(0) {
  assert(source != dest);
  this->set_source_square(source);
  this->set_destination_square(dest);
  this->set_promotion(Move::MASK_3_BITS);
  this->set_en_passant(Move::MASK_4_BITS);
  this->set_castle(Castles::NONE);
}

Move::Move(const int& move) noexcept : m_move(move) {}

auto Move::set_source_square(const Square square) noexcept -> void {
  this->m_move |= (square & Move::MASK_6_BITS) << Move::SOURCE_OFFSET;
}

auto Move::set_destination_square(const Square square) noexcept -> void {
  this->m_move |= (square & Move::MASK_6_BITS) << Move::DEST_OFFSET;
}

auto Move::get_source_square() const noexcept -> Square {
  return static_cast<Square>(this->m_move & Move::MASK_6_BITS);
}

auto Move::get_destination_square() const noexcept -> Square {
  return static_cast<Square>((this->m_move >> DEST_OFFSET) & Move::MASK_6_BITS);
}

auto Move::get_source_bit_board() const noexcept -> BitBoard {
  return GameUtils::square_to_bit_board(this->get_source_square());
}

auto Move::get_destination_bit_board() const noexcept -> BitBoard {
  return GameUtils::square_to_bit_board(this->get_destination_square());
}

auto Move::get_promotion() const noexcept -> PieceCode {
  return (this->m_move >> Move::PROMO_OFFSET) & Move::MASK_3_BITS;
}

auto Move::get_en_passant() const noexcept -> int32_t {
  return (this->m_move >> Move::EN_OFFSET) & Move::MASK_4_BITS;
}

auto Move::get_castle() const noexcept -> Castle {
  return (this->m_move >> Move::CASTLE_OFFSET) & Move::MASK_3_BITS;
}

auto Move::set_promotion(const PieceCode piece_code) noexcept -> void {
  this->m_move = (this->m_move & ~(Move::MASK_3_BITS << Move::PROMO_OFFSET)) |
    (piece_code & Move::MASK_3_BITS) << Move::PROMO_OFFSET;
}

auto Move::set_en_passant(const int32_t column_index) noexcept -> void {
  this->m_move = (this->m_move & ~(Move::MASK_4_BITS << Move::EN_OFFSET)) |
    (column_index & Move::MASK_4_BITS) << Move::EN_OFFSET;
}

auto Move::set_castle(const Castle castle) noexcept -> void {
  this->m_move = (this->m_move & ~(Move::MASK_3_BITS << Move::CASTLE_OFFSET)) |
    (castle & Move::MASK_3_BITS) << Move::CASTLE_OFFSET;
}

auto Move::is_promotion() const noexcept -> bool {
  return ((this->m_move >> Move::PROMO_OFFSET) & Move::MASK_3_BITS) !=
    Move::MASK_3_BITS;
}

auto Move::is_en_passantable() const noexcept -> bool {
  return ((this->m_move >> Move::EN_OFFSET) & Move::MASK_4_BITS) !=
    Move::MASK_4_BITS;
}

auto Move::is_castle() const noexcept -> bool {
  return ((this->m_move >> Move::CASTLE_OFFSET) & Move::MASK_3_BITS) != 0;
}

auto Move::to_string() const noexcept -> std::string {
  std::string move_str = "";

  auto is_castle = this->is_castle();
  if(is_castle) {
    auto castle = this->get_castle();
    switch(castle) {
      case Castles::WHITE_KING:
      {
        move_str = "e1g1";
        break;
      }
      case Castles::WHITE_QUEEN:
      {
        move_str = "e1c1";
        break;
      }
      case Castles::BLACK_KING:
      {
        move_str = "e8g8";
        break;
      }
      case Castles::BLACK_QUEEN:
      {
        move_str = "e8c8";
        break;
      }
      default:
      {
        assert(false);
      }
    }
  } else {
    move_str += GameUtils::get_tile_name(this->get_source_bit_board());
    move_str += GameUtils::get_tile_name(this->get_destination_bit_board());
    if(this->is_promotion()) {
      switch(this->get_promotion()) {
        case PieceCodes::BISHOP:
        {
          move_str += 'b';
          break;
        }
        case PieceCodes::KNIGHT:
        {
          move_str += 'k';
          break;
        }
        case PieceCodes::ROOK:
        {
          move_str += 'r';
          break;
        }
        case PieceCodes::QUEEN:
        {
          move_str += 'q';
          break;
        }
        default:
        {
          assert(false);
        }
      }
    }
  }
  return move_str;
}

auto Move::operator==(const Move& move) const noexcept -> bool {
  return this->m_move == move.m_move;
}
