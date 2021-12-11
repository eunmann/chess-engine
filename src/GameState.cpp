#include "GameState.hpp"

GameState::GameState() noexcept
  : position() {

  this->init();
}

auto GameState::init() noexcept -> void {
  this->position.init();

  this->set_white_to_move(true);
  this->set_king_moved<Colors::WHITE>(false);
  this->set_king_moved<Colors::BLACK>(false);
  this->set_rook_A_moved<Colors::WHITE>(false);
  this->set_rook_A_moved<Colors::BLACK>(false);
  this->set_rook_H_moved<Colors::WHITE>(false);
  this->set_rook_H_moved<Colors::BLACK>(false);
  this->set_is_legal(true);
  this->set_en_passant(GameState::MASK_4_BIT);
}

auto GameState::has_king_moved(const Color color) const noexcept -> bool {
  if (color == Colors::WHITE) {
    return this->m_flags.get_bits< GameState::MASK_1_BIT, GameState::WHITE_KING_MOVED_OFFSET>();
  } else {
    return this->m_flags.get_bits<GameState::MASK_1_BIT, GameState::BLACK_KING_MOVED_OFFSET>();
  }
}

auto GameState::is_color_in_check(const Color color) const noexcept -> bool {
  if (color == Colors::WHITE) {
    return this->is_color_in_check<Colors::WHITE>();
  } else {
    return this->is_color_in_check<Colors::BLACK>();
  }
}

auto GameState::has_rook_A_moved(const Color color) const noexcept -> bool {
  if (color == Colors::WHITE) {
    return this->m_flags.get_bits<GameState::MASK_1_BIT, GameState::WHITE_ROOK_A_OFFSET>();;
  } else {
    return this->m_flags.get_bits<GameState::MASK_1_BIT, GameState::BLACK_ROOK_A_OFFSET>();
  }
}
auto GameState::has_rook_H_moved(const Color color) const noexcept -> bool {
  if (color == Colors::WHITE) {
    return this->m_flags.get_bits<GameState::MASK_1_BIT, GameState::WHITE_ROOK_H_OFFSET>();;
  } else {
    return this->m_flags.get_bits<GameState::MASK_1_BIT, GameState::BLACK_ROOK_H_OFFSET>();
  }
}

auto GameState::is_white_in_check() const noexcept -> bool {
  return this->is_color_in_check<Colors::WHITE>();
}
auto GameState::is_black_in_check() const noexcept -> bool {
  return this->is_color_in_check<Colors::BLACK>();
}

auto GameState::set_white_to_move(const bool white_move) noexcept -> void {
  this->m_flags.set_bits<GameState::MASK_1_BIT, GameState::WHITE_TO_MOVE_OFFSET>(white_move);
}

auto GameState::is_white_to_move() const noexcept ->bool {
  return this->m_flags.get_bits<GameState::MASK_1_BIT, GameState::WHITE_TO_MOVE_OFFSET>() == 1;
}

auto GameState::set_is_legal(const bool is_legal) noexcept -> void {
  this->m_flags.set_bits<GameState::MASK_1_BIT, GameState::IS_LEGAL_OFFSET>(is_legal);
}

auto GameState::is_legal() const noexcept -> bool {
  return this->m_flags.get_bits<GameState::MASK_1_BIT, GameState::IS_LEGAL_OFFSET>() == 1;
}

auto GameState::set_en_passant(const int32_t pawn_ep) noexcept -> void {
  this->m_flags.set_bits<GameState::MASK_4_BIT, GameState::PAWN_EN_OFFSET>(pawn_ep);
}

auto GameState::get_en_passant() const noexcept -> int32_t {
  return this->m_flags.get_bits<GameState::MASK_4_BIT, GameState::PAWN_EN_OFFSET>();
}