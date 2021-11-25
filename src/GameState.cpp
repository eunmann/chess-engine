#include "GameState.hpp"

GameState::GameState()
  : position(),
  white_to_move(true),
  white_king_moved(false),
  white_rook_A_moved(false),
  white_rook_H_moved(false),
  black_king_moved(false),
  black_rook_A_moved(false),
  black_rook_H_moved(false),
  is_legal(true),
  pawn_ep(15) {}

auto GameState::init() -> void {
  this->position.init();

  this->white_to_move = true;
  this->white_king_moved = false;
  this->white_rook_A_moved = false;
  this->white_rook_H_moved = false;
  this->black_king_moved = false;
  this->black_rook_A_moved = false;
  this->black_rook_H_moved = false;

  this->is_legal = true;

  this->pawn_ep = 15;
}

auto GameState::apply_move(const Move move) -> void {
  const BitBoard source_bit_board = move.get_source_bit_board();
  const BitBoard destination_bit_board = move.get_destination_bit_board();
  const Color color = this->position.get_color(source_bit_board);

  // TODO(EMU): Does this check make sense here?
  if((color == Colors::WHITE && !this->white_to_move) ||
     (color == Colors::BLACK && this->white_to_move)) {
    this->is_legal = false;
    return;
  }

  PieceCode piece_code = 0;
  const bool is_castle = move.is_castle();
  const Castle castle = move.get_castle();

  if(is_castle) {
    piece_code = PieceCodes::KING;

    if(color == Colors::WHITE) {
      if(castle == Castles::WHITE_KING) {
        this->position.clear(BitBoards::WHITE_KING_START |
                             BitBoards::WHITE_ROOK_H_START);
        this->position.add(PieceCodes::KING, color,
                           BitBoards::WHITE_KING_KING_CASTLE);
        this->position.add(PieceCodes::ROOK, color,
                           BitBoards::WHITE_ROOK_KING_CASTLE);
      } else {
        this->position.clear(BitBoards::WHITE_KING_START |
                             BitBoards::WHITE_ROOK_A_START);
        this->position.add(PieceCodes::KING, color,
                           BitBoards::WHITE_KING_QUEEN_CASTLE);
        this->position.add(PieceCodes::ROOK, color,
                           BitBoards::WHITE_ROOK_QUEEN_CASTLE);
      }
    } else {
      if(castle == Castles::BLACK_KING) {
        this->position.clear(BitBoards::BLACK_KING_START |
                             BitBoards::BLACK_ROOK_H_START);
        this->position.add(PieceCodes::KING, color,
                           BitBoards::BLACK_KING_KING_CASTLE);
        this->position.add(PieceCodes::ROOK, color,
                           BitBoards::BLACK_ROOK_KING_CASTLE);
      } else {
        this->position.clear(BitBoards::BLACK_KING_START |
                             BitBoards::BLACK_ROOK_A_START);
        this->position.add(PieceCodes::KING, color,
                           BitBoards::BLACK_KING_QUEEN_CASTLE);
        this->position.add(PieceCodes::ROOK, color,
                           BitBoards::BLACK_ROOK_QUEEN_CASTLE);
      }
    }
  } else {
    if((color == Colors::WHITE &&
       this->position.is_white_occupied(destination_bit_board)) ||
       (color == Colors::BLACK &&
       this->position.is_black_occupied(destination_bit_board))) {
      this->is_legal = false;
      return;
    }

    this->pawn_ep = move.get_en_passant();

    if(move.is_promotion()) {
      piece_code = move.get_promotion();
    } else {
      piece_code = this->position.get_piece_type(source_bit_board);
    }

    this->position.clear(source_bit_board | destination_bit_board);
    this->position.add(piece_code, color, destination_bit_board);
  }

  this->position.recompute_threaten();

  if(color == Colors::WHITE) {
    this->is_legal = !this->is_white_in_check();
  } else {
    this->is_legal = !this->is_black_in_check();
  }

  this->white_to_move = !this->white_to_move;
  this->white_king_moved =
    color == Colors::WHITE && (piece_code == PieceCodes::KING || is_castle);
  this->white_rook_A_moved =
    color == Colors::WHITE &&
    ((piece_code == PieceCodes::ROOK &&
     source_bit_board == BitBoards::WHITE_ROOK_A_START) ||
     castle == Castles::WHITE_QUEEN);
  this->white_rook_H_moved =
    color == Colors::WHITE &&
    ((piece_code == PieceCodes::ROOK &&
     source_bit_board == BitBoards::WHITE_ROOK_H_START) ||
     castle == Castles::WHITE_KING);

  this->black_king_moved =
    color == Colors::BLACK && (piece_code == PieceCodes::KING || is_castle);
  this->black_rook_A_moved =
    color == Colors::BLACK &&
    ((piece_code == PieceCodes::ROOK &&
     source_bit_board == BitBoards::WHITE_ROOK_A_START) ||
     castle == Castles::BLACK_QUEEN);
  this->black_rook_H_moved =
    color == Colors::BLACK &&
    ((piece_code == PieceCodes::ROOK &&
     source_bit_board == BitBoards::WHITE_ROOK_H_START) ||
     castle == Castles::BLACK_KING);
}

auto GameState::is_white_in_check() const -> bool {
  return this->is_color_in_check<Colors::WHITE>();
}
auto GameState::is_black_in_check() const -> bool {
  return this->is_color_in_check<Colors::BLACK>();
}