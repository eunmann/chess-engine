#include "Position.hpp"

#include "GameUtils.hpp"
#include "MoveGeneration.hpp"

Position::Position() noexcept {
  this->clear();
}

auto Position::init() noexcept -> void {

  // Pawns
  this->piece_positions[PieceCodes::PAWN] = BitBoards::ROW_2;
  this->piece_positions[PieceCodes::PAWN] |= BitBoards::ROW_7;

  // Knights
  this->piece_positions[PieceCodes::KNIGHT] = GameUtils::shift_bit_board<0, 1>(0b1ULL);
  this->piece_positions[PieceCodes::KNIGHT] |= GameUtils::shift_bit_board<0, 6>(0b1ULL);
  this->piece_positions[PieceCodes::KNIGHT] |= GameUtils::shift_bit_board<7, 1>(0b1ULL);
  this->piece_positions[PieceCodes::KNIGHT] |= GameUtils::shift_bit_board<7, 6>(0b1ULL);

  // Bishops
  this->piece_positions[PieceCodes::BISHOP] = GameUtils::shift_bit_board<0, 2>(0b1ULL);
  this->piece_positions[PieceCodes::BISHOP] |= GameUtils::shift_bit_board<0, 5>(0b1ULL);
  this->piece_positions[PieceCodes::BISHOP] |= GameUtils::shift_bit_board<7, 2>(0b1ULL);
  this->piece_positions[PieceCodes::BISHOP] |= GameUtils::shift_bit_board<7, 5>(0b1ULL);

  // Rooks
  this->piece_positions[PieceCodes::ROOK] = GameUtils::shift_bit_board<0, 0>(0b1ULL);
  this->piece_positions[PieceCodes::ROOK] |= GameUtils::shift_bit_board<0, 7>(0b1ULL);
  this->piece_positions[PieceCodes::ROOK] |= GameUtils::shift_bit_board<7, 0>(0b1ULL);
  this->piece_positions[PieceCodes::ROOK] |= GameUtils::shift_bit_board<7, 7>(0b1ULL);

  // Queens
  this->piece_positions[PieceCodes::QUEEN] = GameUtils::shift_bit_board<0, 3>(0b1ULL);
  this->piece_positions[PieceCodes::QUEEN] |= GameUtils::shift_bit_board<7, 3>(0b1ULL);

  // Kings
  this->piece_positions[PieceCodes::KING] = GameUtils::shift_bit_board<0, 4>(0b1ULL);
  this->piece_positions[PieceCodes::KING] |= GameUtils::shift_bit_board<7, 4>(0b1ULL);

  // Color Position
  this->color_positions[Colors::WHITE] = BitBoards::ROW_1 | BitBoards::ROW_2;
  this->color_positions[Colors::BLACK] = BitBoards::ROW_7 | BitBoards::ROW_8;

  this->recompute_threaten();
}

auto Position::clear() noexcept -> void {
  for (int i = 0; i < PieceCodes::NUM; ++i) {
    this->piece_positions[i] = 0ULL;
  }

  for (int i = 0; i < Colors::NUM; ++i) {
    this->color_positions[i] = 0ULL;
    this->threaten_positions[i] = 0ULL;
  }
}

auto Position::get_piece_bit_board(const PieceCode piece_code) const noexcept-> BitBoard {
  return this->piece_positions[piece_code];
}

auto Position::get_color_bit_board(const Color color) const noexcept -> BitBoard {
  return this->color_positions[color];
}

auto Position::get_piece_color_bit_board(const PieceCode piece_code,
  const Color color) const noexcept -> BitBoard {
  return this->get_piece_bit_board(piece_code) &
    this->get_color_bit_board(color);
}

auto Position::get_occupied_bit_board() const noexcept -> BitBoard {
  return this->get_color_bit_board<Colors::WHITE>() |
    this->get_color_bit_board<Colors::BLACK>();
}

auto Position::get_empty_bit_board() const noexcept -> BitBoard {
  return ~this->get_occupied_bit_board();
}

auto Position::get_white_bit_board() const noexcept -> BitBoard {
  return this->get_color_bit_board<Colors::WHITE>();
}
auto Position::get_black_bit_board() const noexcept -> BitBoard {
  return this->get_color_bit_board<Colors::BLACK>();
}

auto Position::get_white_threaten() const noexcept -> BitBoard {
  return this->get_threaten<Colors::WHITE>();
}

auto Position::get_black_threaten() const noexcept -> BitBoard {
  return this->get_threaten<Colors::BLACK>();
}

auto Position::get_color(const BitBoard bit_board) const noexcept -> Color {
  if (this->is_white_occupied(bit_board)) {
    return Colors::WHITE;
  } else if (this->is_black_occupied(bit_board)) {
    return Colors::BLACK;
  } else {
    return Colors::NUM;
  }
}

auto Position::get_piece_type(const BitBoard bit_board) const noexcept -> PieceCode {
  for (PieceCode p = PieceCodes::PAWN; p < PieceCodes::NUM; p++) {
    if ((this->piece_positions[p] & bit_board) != 0) {
      return p;
    }
  }

  return PieceCodes::NUM;
}

auto Position::clear(const BitBoard bit_board) noexcept -> void {
  const BitBoard negated_bit_board = ~bit_board;

  for (PieceCode pc = 0; pc < PieceCodes::NUM; pc++) {
    this->piece_positions[pc] &= negated_bit_board;
  }

  this->color_positions[Colors::WHITE] &= negated_bit_board;
  this->color_positions[Colors::BLACK] &= negated_bit_board;
}

auto Position::add(const PieceCode piece_code, const Color color,
  const BitBoard bit_board) noexcept -> void {
  this->piece_positions[piece_code] |= bit_board;
  this->color_positions[color] |= bit_board;
}

auto Position::recompute_threaten() noexcept -> void {
  this->threaten_positions[Colors::WHITE] = MoveGeneration::get_capture_positions<Colors::WHITE>(*this);
  this->threaten_positions[Colors::BLACK] = MoveGeneration::get_capture_positions<Colors::BLACK>(*this);
}

auto Position::is_empty(const BitBoard bit_board) const noexcept -> bool {
  return !this->is_occupied(bit_board);
}

auto Position::is_occupied(const BitBoard bit_board) const noexcept -> bool {
  return (this->get_occupied_bit_board() & bit_board) != 0;
}

auto Position::is_white_occupied(const BitBoard bit_board) const noexcept -> bool {
  return this->is_color_occupied<Colors::WHITE>(bit_board);
}

auto Position::is_black_occupied(const BitBoard bit_board) const noexcept -> bool {
  return this->is_color_occupied<Colors::BLACK>(bit_board);
}

auto Position::is_white_threaten(const BitBoard bit_board) const noexcept -> bool {
  return this->is_threaten<Colors::WHITE>(bit_board);
}

auto Position::is_black_threaten(const BitBoard bit_board) const noexcept -> bool {
  return this->is_threaten<Colors::BLACK>(bit_board);
}

auto Position::to_board() const noexcept -> Board {
  Board board;

  for (int i = 0; i < PieceCodes::NUM; i++) {
    const PieceCode piece_code = PieceCodes::ALL[i];
    const int32_t board_value = BoardValues::ALL[i];

    const BitBoard piece_bit_board = this->get_piece_bit_board(piece_code);

    GameUtils::for_each_set_square(
      piece_bit_board, [this, board_value, &board](auto square) {
        BitBoard bit_board = GameUtils::square_to_bit_board(square);
        if (this->is_white_occupied(bit_board)) {
          board.positions[square] = board_value;
        } else {
          board.positions[square] = -1 * board_value;
        }
      });
  }

  return board;
}

auto Position::is_threaten(const BitBoard bit_board, const Color color) const noexcept -> bool {
  return (this->threaten_positions[color] & bit_board) != 0;
}