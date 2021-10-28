#include "Position.hpp"

#include "GameUtils.hpp"

Position::Position() {
    this->clear();
}

void Position::init() {
    // Pawns
    for (int i = 0; i < 8; ++i) {
        this->piece_positions[PieceCodes::PAWN] |= GameUtils::move(0x1ULL, 1, i);
        this->piece_positions[PieceCodes::PAWN] |= GameUtils::move(0x1ULL, 6, i);
    }

    // Knights
    this->piece_positions[PieceCodes::KNIGHT] |= GameUtils::move(0x1ULL, 0, 1);
    this->piece_positions[PieceCodes::KNIGHT] |= GameUtils::move(0x1ULL, 0, 6);
    this->piece_positions[PieceCodes::KNIGHT] |= GameUtils::move(0x1ULL, 7, 1);
    this->piece_positions[PieceCodes::KNIGHT] |= GameUtils::move(0x1ULL, 7, 6);

    // Bishops
    this->piece_positions[PieceCodes::BISHOP] |= GameUtils::move(0x1ULL, 0, 2);
    this->piece_positions[PieceCodes::BISHOP] |= GameUtils::move(0x1ULL, 0, 5);
    this->piece_positions[PieceCodes::BISHOP] |= GameUtils::move(0x1ULL, 7, 2);
    this->piece_positions[PieceCodes::BISHOP] |= GameUtils::move(0x1ULL, 7, 5);

    // Rooks
    this->piece_positions[PieceCodes::ROOK] |= GameUtils::move(0x1ULL, 0, 0);
    this->piece_positions[PieceCodes::ROOK] |= GameUtils::move(0x1ULL, 0, 7);
    this->piece_positions[PieceCodes::ROOK] |= GameUtils::move(0x1ULL, 7, 0);
    this->piece_positions[PieceCodes::ROOK] |= GameUtils::move(0x1ULL, 7, 7);

    // Queens
    this->piece_positions[PieceCodes::QUEEN] |= GameUtils::move(0x1ULL, 0, 3);
    this->piece_positions[PieceCodes::QUEEN] |= GameUtils::move(0x1ULL, 7, 3);

    // Kings
    this->piece_positions[PieceCodes::KING] |= GameUtils::move(0x1ULL, 0, 4);
    this->piece_positions[PieceCodes::KING] |= GameUtils::move(0x1ULL, 7, 4);

    // Color Position
    this->color_positions[Colors::WHITE] = BitBoards::ROW_1 | BitBoards::ROW_2;
    this->color_positions[Colors::BLACK] = BitBoards::ROW_7 | BitBoards::ROW_8;

    // TODO(EMU): When done refactoring, fix this
    //game_state.position.get_white_threaten() = GameUtils::get_capture_positions(game_state, true);
    //game_state.position.get_black_threaten() = GameUtils::get_capture_positions(game_state, false);
}

void Position::clear() {
    for (int i = 0; i < PieceCodes::NUM; ++i) {
        this->piece_positions[i] = 0ULL;
    }

    for (int i = 0; i < Colors::NUM; ++i) {
        this->color_positions[i] = 0ULL;
        this->threaten_positions[i] = 0ULL;
    }
}

BitBoard Position::get_piece_bit_board(PieceCode piece_code) const {
    return this->piece_positions[piece_code];
}

void Position::set_piece_bit_board(PieceCode piece_code, BitBoard bit_board) {
    this->piece_positions[piece_code] = bit_board;
}

BitBoard Position::get_color_bit_board(Color color) const {
    return this->color_positions[color];
}

BitBoard Position::get_piece_color_bit_board(PieceCode piece_code, Color color) const {
    return this->get_piece_bit_board(piece_code) & this->get_color_bit_board(color);
}

BitBoard Position::get_occupied_bit_board() const {
    return this->color_positions[Colors::WHITE] | this->color_positions[Colors::BLACK];
}

BitBoard Position::get_empty_bit_board() const {
    return ~this->get_occupied_bit_board();
}

BitBoard Position::get_white_bit_board() const {
    return this->get_color_bit_board(Colors::WHITE);
}
BitBoard Position::get_black_bit_board() const {
    return this->get_color_bit_board(Colors::BLACK);
}

BitBoard Position::get_white_threaten() const {
    return this->threaten_positions[Colors::WHITE];
}

BitBoard Position::get_black_threaten() const {
    return this->threaten_positions[Colors::BLACK];
}
