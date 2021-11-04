#include "Position.hpp"

#include "GameUtils.hpp"

Position::Position() {
    this->clear();
}

auto Position::init() -> void {
    // Pawns
    for (int i = 0; i < 8; ++i) {
        this->piece_positions[PieceCodes::PAWN] |= GameUtils::shift_bit_board(0b1, 1, i);
        this->piece_positions[PieceCodes::PAWN] |= GameUtils::shift_bit_board(0b1, 6, i);
    }

    // Knights
    this->piece_positions[PieceCodes::KNIGHT] |= GameUtils::shift_bit_board<0, 1>(0b1);
    this->piece_positions[PieceCodes::KNIGHT] |= GameUtils::shift_bit_board<0, 6>(0b1);
    this->piece_positions[PieceCodes::KNIGHT] |= GameUtils::shift_bit_board<7, 1>(0b1);
    this->piece_positions[PieceCodes::KNIGHT] |= GameUtils::shift_bit_board<7, 6>(0b1);

    // Bishops
    this->piece_positions[PieceCodes::BISHOP] |= GameUtils::shift_bit_board<0, 2>(0b1);
    this->piece_positions[PieceCodes::BISHOP] |= GameUtils::shift_bit_board<0, 5>(0b1);
    this->piece_positions[PieceCodes::BISHOP] |= GameUtils::shift_bit_board<7, 2>(0b1);
    this->piece_positions[PieceCodes::BISHOP] |= GameUtils::shift_bit_board<7, 5>(0b1);

    // Rooks
    this->piece_positions[PieceCodes::ROOK] |= GameUtils::shift_bit_board<0, 0>(0b1);
    this->piece_positions[PieceCodes::ROOK] |= GameUtils::shift_bit_board<0, 7>(0b1);
    this->piece_positions[PieceCodes::ROOK] |= GameUtils::shift_bit_board<7, 0>(0b1);
    this->piece_positions[PieceCodes::ROOK] |= GameUtils::shift_bit_board<7, 7>(0b1);

    // Queens
    this->piece_positions[PieceCodes::QUEEN] |= GameUtils::shift_bit_board<0, 3>(0b1);
    this->piece_positions[PieceCodes::QUEEN] |= GameUtils::shift_bit_board<7, 3>(0b1);

    // Kings
    this->piece_positions[PieceCodes::KING] |= GameUtils::shift_bit_board<0, 4>(0b1);
    this->piece_positions[PieceCodes::KING] |= GameUtils::shift_bit_board<7, 4>(0b1);

    // Color Position
    this->color_positions[Colors::WHITE] = BitBoards::ROW_1 | BitBoards::ROW_2;
    this->color_positions[Colors::BLACK] = BitBoards::ROW_7 | BitBoards::ROW_8;

    // TODO(EMU): When done refactoring, fix this
    //game_state.position.get_white_threaten() = GameUtils::get_capture_positions(game_state, true);
    //game_state.position.get_black_threaten() = GameUtils::get_capture_positions(game_state, false);
}

auto Position::clear() -> void {
    for (int i = 0; i < PieceCodes::NUM; ++i) {
        this->piece_positions[i] = 0ULL;
    }

    for (int i = 0; i < Colors::NUM; ++i) {
        this->color_positions[i] = 0ULL;
        this->threaten_positions[i] = 0ULL;
    }
}

auto Position::get_piece_bit_board(PieceCode piece_code) const -> BitBoard {
    return this->piece_positions[piece_code];
}

auto Position::set_piece_bit_board(PieceCode piece_code, BitBoard bit_board) -> void {
    this->piece_positions[piece_code] = bit_board;
}

auto Position::get_color_bit_board(Color color) const -> BitBoard {
    return this->color_positions[color];
}

auto Position::get_piece_color_bit_board(PieceCode piece_code, Color color) const -> BitBoard {
    return this->get_piece_bit_board(piece_code) & this->get_color_bit_board(color);
}

auto Position::get_occupied_bit_board() const -> BitBoard {
    return this->color_positions[Colors::WHITE] | this->color_positions[Colors::BLACK];
}

auto Position::get_empty_bit_board() const -> BitBoard {
    return ~this->get_occupied_bit_board();
}

auto Position::get_white_bit_board() const -> BitBoard {
    return this->get_color_bit_board(Colors::WHITE);
}
auto Position::get_black_bit_board() const -> BitBoard {
    return this->get_color_bit_board(Colors::BLACK);
}

auto Position::get_white_threaten() const -> BitBoard {
    return this->threaten_positions[Colors::WHITE];
}

auto Position::get_black_threaten() const -> BitBoard {
    return this->threaten_positions[Colors::BLACK];
}
