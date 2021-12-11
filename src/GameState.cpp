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

auto GameState::apply_move(const Move move) noexcept -> void {
  const BitBoard source_bit_board = move.get_source_bit_board();
  const BitBoard destination_bit_board = move.get_destination_bit_board();
  const bool is_white_to_move = this->is_white_to_move();
  const Color color = Colors::bool_to_color(is_white_to_move);

  // TODO(EMU): Does this check make sense here?
  if ((color == Colors::WHITE && !is_white_to_move) ||
    (color == Colors::BLACK && is_white_to_move)) {
    this->set_is_legal(false);
    return;
  }

  PieceCode piece_code = 0;
  const bool is_castle = move.is_castle();
  const Castle castle = move.get_castle();

  if (is_castle) {
    piece_code = PieceCodes::KING;

    if (this->has_king_moved(color) ||
      this->is_color_in_check(color)) {
      this->set_is_legal(false);
      return;
    }
    const Color opponent_color = color == Colors::WHITE ? Colors::BLACK : Colors::WHITE;

    constexpr auto can_queen_castle = [](const GameState& game_state, const Color color, const Color opponent_color) {
      const BitBoard queen_castle = color == Colors::WHITE ? BitBoards::WHITE_QUEEN_CASTLE : BitBoards::BLACK_QUEEN_CASTLE;
      return game_state.has_rook_A_moved(color) ||
        !game_state.position.is_empty(queen_castle) ||
        game_state.position.is_threaten(queen_castle, opponent_color);
    };

    constexpr auto can_king_castle = [](const GameState& game_state, const Color color, const Color opponent_color) {
      const BitBoard king_castle = color == Colors::WHITE ? BitBoards::WHITE_KING_CASTLE : BitBoards::BLACK_KING_CASTLE;
      return game_state.has_rook_H_moved(color) ||
        !game_state.position.is_empty(king_castle) ||
        game_state.position.is_threaten(king_castle, opponent_color);
    };

    if (color == Colors::WHITE) {
      if (castle == Castles::WHITE_KING) {
        if (!can_king_castle(*this, color, opponent_color)) {
          this->set_is_legal(false);
          return;
        }
        this->position.clear(BitBoards::WHITE_KING_START |
          BitBoards::WHITE_ROOK_H_START);
        this->position.add(PieceCodes::KING, color,
          BitBoards::WHITE_KING_KING_CASTLE);
        this->position.add(PieceCodes::ROOK, color,
          BitBoards::WHITE_ROOK_KING_CASTLE);
      } else {
        if (!can_queen_castle(*this, color, opponent_color)) {
          this->set_is_legal(false);
          return;
        }
        this->position.clear(BitBoards::WHITE_KING_START |
          BitBoards::WHITE_ROOK_A_START);
        this->position.add(PieceCodes::KING, color,
          BitBoards::WHITE_KING_QUEEN_CASTLE);
        this->position.add(PieceCodes::ROOK, color,
          BitBoards::WHITE_ROOK_QUEEN_CASTLE);
      }
    } else {
      if (castle == Castles::BLACK_KING) {
        if (!can_king_castle(*this, color, opponent_color)) {
          this->set_is_legal(false);
          return;
        }
        this->position.clear(BitBoards::BLACK_KING_START |
          BitBoards::BLACK_ROOK_H_START);
        this->position.add(PieceCodes::KING, color,
          BitBoards::BLACK_KING_KING_CASTLE);
        this->position.add(PieceCodes::ROOK, color,
          BitBoards::BLACK_ROOK_KING_CASTLE);
      } else {
        if (!can_queen_castle(*this, color, opponent_color)) {
          this->set_is_legal(false);
          return;
        }
        this->position.clear(BitBoards::BLACK_KING_START |
          BitBoards::BLACK_ROOK_A_START);
        this->position.add(PieceCodes::KING, color,
          BitBoards::BLACK_KING_QUEEN_CASTLE);
        this->position.add(PieceCodes::ROOK, color,
          BitBoards::BLACK_ROOK_QUEEN_CASTLE);
      }
    }
  } else {
    if ((color == Colors::WHITE &&
      this->position.is_white_occupied(destination_bit_board)) ||
      (color == Colors::BLACK &&
        this->position.is_black_occupied(destination_bit_board))) {
      this->set_is_legal(false);
      return;
    }

    this->set_en_passant(move.get_en_passant());

    if (move.is_promotion()) {
      piece_code = move.get_promotion();
    } else {
      piece_code = this->position.get_piece_type(source_bit_board);
    }

    this->position.clear(source_bit_board | destination_bit_board);
    this->position.add(piece_code, color, destination_bit_board);
  }

  this->position.recompute_threaten();

  if (color == Colors::WHITE) {
    this->set_is_legal(!this->is_white_in_check());
  } else {
    this->set_is_legal(!this->is_black_in_check());
  }

  this->set_white_to_move(!is_white_to_move);

  this->set_king_moved<Colors::WHITE>(color == Colors::WHITE && (piece_code == PieceCodes::KING || is_castle));
  this->set_rook_A_moved<Colors::WHITE>(color == Colors::WHITE && ((piece_code == PieceCodes::ROOK && source_bit_board == BitBoards::WHITE_ROOK_A_START) || castle == Castles::WHITE_QUEEN));
  this->set_rook_H_moved<Colors::WHITE>(color == Colors::WHITE && ((piece_code == PieceCodes::ROOK && source_bit_board == BitBoards::WHITE_ROOK_H_START) || castle == Castles::WHITE_KING));

  this->set_king_moved<Colors::BLACK>(color == Colors::BLACK && (piece_code == PieceCodes::KING || is_castle));
  this->set_rook_A_moved<Colors::BLACK>(color == Colors::BLACK && ((piece_code == PieceCodes::ROOK && source_bit_board == BitBoards::WHITE_ROOK_A_START) || castle == Castles::BLACK_QUEEN));
  this->set_rook_H_moved<Colors::BLACK>(color == Colors::BLACK && ((piece_code == PieceCodes::ROOK && source_bit_board == BitBoards::WHITE_ROOK_H_START) || castle == Castles::BLACK_KING));
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