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
    //game_state.position.white_threaten = GameUtils::get_capture_positions(game_state, true);
    //game_state.position.black_threaten = GameUtils::get_capture_positions(game_state, false);
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