#include "GameState.hpp"

GameState::GameState()
    : position(),
      white_to_move(true),
      white_king_moved(false),
      white_rook_1_moved(false),
      white_rook_2_moved(false),
      black_king_moved(false),
      black_rook_1_moved(false),
      black_rook_2_moved(false),
      black_king_in_check(false),
      pawn_ep(-128) {
}

auto GameState::apply_move(const Move move) -> void {
    BitBoard source_bit_board = move.get_source_bit_board();
    BitBoard destination_bit_board = move.get_destination_bit_board();
    PieceCode piece_code = this->position.get_piece_type(source_bit_board);
    Color color = this->position.get_color(source_bit_board);

    this->position.clear(source_bit_board);
    this->position.add(piece_code, color, destination_bit_board);
    this->position.recompute();

    // TODO(EMU): Update GameState flags
}

auto GameState::is_move_legal(const Move move) const -> bool {
    BitBoard source_bit_board = move.get_source_bit_board();
    BitBoard destination_bit_board = move.get_destination_bit_board();
    Color color = this->position.get_color(source_bit_board);

    if ((color == Colors::WHITE && ((this->position.get_white_bit_board() & destination_bit_board) != 0)) ||
        (color == Colors::BLACK && ((this->position.get_black_bit_board() & destination_bit_board) != 0))) {
        return false;
    }

    // TODO(EMU): I don't like this copy here
    GameState temp = *this;
    temp.apply_move(move);

    if (color == Colors::WHITE) {
        return (temp.position.get_piece_color_bit_board(PieceCodes::KING, Colors::WHITE) & temp.position.get_black_threaten()) == 0;
    } else {
        return (temp.position.get_piece_color_bit_board(PieceCodes::KING, Colors::BLACK) & temp.position.get_white_threaten()) == 0;
    }

    return true;
}