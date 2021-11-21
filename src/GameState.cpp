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
      black_king_in_check(false),
      pawn_ep(-128) {
}

auto GameState::apply_move(const Move move) -> void {
    BitBoard source_bit_board = move.get_source_bit_board();
    BitBoard destination_bit_board = move.get_destination_bit_board();
    Color color = this->position.get_color(source_bit_board);

    if ((color == Colors::WHITE && this->position.is_white_occupied(destination_bit_board)) ||
        (color == Colors::BLACK && this->position.is_black_occupied(destination_bit_board))) {
        this->is_legal = false;
        return;
    }

    this->pawn_ep = move.get_en_passant();

    PieceCode piece_code = 0;
    if (move.is_promotion()) {
        piece_code = move.get_promotion();
    } else {
        piece_code = this->position.get_piece_type(source_bit_board);
    }

    bool is_castle = move.is_castle();
    Castle castle = move.get_castle();

    if (is_castle) {
        if (color == Colors::WHITE) {
            if (castle == Castles::WHITE_KING) {
                this->position.clear(BitBoards::WHITE_KING_START | BitBoards::WHITE_ROOK_H_START);
                this->position.add(PieceCodes::KING, color, BitBoards::WHITE_KING_KING_CASTLE);
                this->position.add(PieceCodes::ROOK, color, BitBoards::WHITE_ROOK_KING_CASTLE);
            } else {
                this->position.clear(BitBoards::WHITE_KING_START | BitBoards::WHITE_ROOK_A_START);
                this->position.add(PieceCodes::KING, color, BitBoards::WHITE_KING_QUEEN_CASTLE);
                this->position.add(PieceCodes::ROOK, color, BitBoards::WHITE_ROOK_QUEEN_CASTLE);
            }
        } else {
            if (castle == Castles::BLACK_KING) {
                this->position.clear(BitBoards::BLACK_KING_START | BitBoards::BLACK_ROOK_H_START);
                this->position.add(PieceCodes::KING, color, BitBoards::BLACK_KING_KING_CASTLE);
                this->position.add(PieceCodes::ROOK, color, BitBoards::BLACK_ROOK_KING_CASTLE);
            } else {
                this->position.clear(BitBoards::BLACK_KING_START | BitBoards::BLACK_ROOK_A_START);
                this->position.add(PieceCodes::KING, color, BitBoards::BLACK_KING_QUEEN_CASTLE);
                this->position.add(PieceCodes::ROOK, color, BitBoards::BLACK_ROOK_QUEEN_CASTLE);
            }
        }
    } else {
        this->position.clear(source_bit_board);
        this->position.add(piece_code, color, destination_bit_board);
    }

    this->position.recompute_threaten();

    if (color == Colors::WHITE) {
        BitBoard king_bit_board = this->position.get_piece_color_bit_board(PieceCodes::KING, Colors::WHITE);
        this->is_legal = this->position.is_black_threaten(king_bit_board);
    } else {
        BitBoard king_bit_board = this->position.get_piece_color_bit_board(PieceCodes::KING, Colors::BLACK);
        this->is_legal = this->position.is_white_threaten(king_bit_board);
    }

    this->white_to_move = !this->white_to_move;
    this->white_king_moved = color == Colors::WHITE && (piece_code == PieceCodes::KING || is_castle);
    this->white_rook_A_moved = color == Colors::WHITE && ((piece_code == PieceCodes::ROOK && source_bit_board == BitBoards::WHITE_ROOK_A_START) || castle == Castles::WHITE_QUEEN);
    this->white_rook_H_moved = color == Colors::WHITE && ((piece_code == PieceCodes::ROOK && source_bit_board == BitBoards::WHITE_ROOK_H_START) || castle == Castles::WHITE_KING);
    BitBoard king_bit_board = this->position.get_piece_color_bit_board(PieceCodes::KING, Colors::WHITE);
    this->white_king_in_check = this->position.is_black_threaten(king_bit_board);

    this->black_king_moved = color == Colors::BLACK && (piece_code == PieceCodes::KING || is_castle);
    this->black_rook_A_moved = color == Colors::BLACK && ((piece_code == PieceCodes::ROOK && source_bit_board == BitBoards::WHITE_ROOK_A_START) || castle == Castles::BLACK_QUEEN);
    this->black_rook_H_moved = color == Colors::BLACK && ((piece_code == PieceCodes::ROOK && source_bit_board == BitBoards::WHITE_ROOK_H_START) || castle == Castles::BLACK_KING);
    king_bit_board = this->position.get_piece_color_bit_board(PieceCodes::KING, Colors::BLACK);
    this->black_king_in_check = this->position.is_white_threaten(king_bit_board);

    if (color == Colors::WHITE && this->white_king_in_check) {
        this->is_legal = false;
    } else if (color == Colors::BLACK && this->black_king_in_check) {
        this->is_legal = false;
    }
}