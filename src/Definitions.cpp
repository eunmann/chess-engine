#include "Definitions.hpp"

char PieceCodes::to_char(int32_t piece_code) {
    switch (piece_code) {
        case PieceCodes::PAWN: {
            return ' ';
        }
        case PieceCodes::KNIGHT: {
            return 'N';
        }
        case PieceCodes::BISHOP: {
            return 'B';
        }
        case PieceCodes::ROOK: {
            return 'R';
        }
        case PieceCodes::QUEEN: {
            return 'Q';
        }
        case PieceCodes::KING: {
            return 'K';
        }
        default: {
            return '-';
        }
    }
}

Color Colors::bool_to_color(bool white_to_move) {
    return white_to_move ? Colors::WHITE : Colors::BLACK;
}