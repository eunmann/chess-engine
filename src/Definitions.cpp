#include "CommonHeaders.hpp"

auto PieceCodes::to_char(PieceCode piece_code) noexcept -> char {
    switch (piece_code.value) {
        case PieceCodes::PAWN.value: {
            return ' ';
        }
        case PieceCodes::KNIGHT.value: {
            return 'N';
        }
        case PieceCodes::BISHOP.value: {
            return 'B';
        }
        case PieceCodes::ROOK.value: {
            return 'R';
        }
        case PieceCodes::QUEEN.value: {
            return 'Q';
        }
        case PieceCodes::KING.value: {
            return 'K';
        }
        default: {
            return '-';
        }
    }
}

auto Colors::bool_to_color(bool white_to_move) noexcept -> Color {
    return white_to_move ? Colors::WHITE : Colors::BLACK;
}