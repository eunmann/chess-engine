#include "Board.hpp"

#include <stdio.h>

#include <algorithm>
#include <string>

#include "Definitions.hpp"

Board::Board() {
    this->clear();
};

auto Board::init_standard() -> void {
    const std::array<PieceCode, 8> init_rank{
        PieceCodes::ROOK,
        PieceCodes::KNIGHT,
        PieceCodes::BISHOP,
        PieceCodes::QUEEN,
        PieceCodes::KING,
        PieceCodes::BISHOP,
        PieceCodes::KNIGHT,
        PieceCodes::ROOK};

    const int bottom_start = 8 * 6;
    for (int i = 0; i < 8; ++i) {
        this->positions[i] = init_rank[i] * -1;
        this->positions[i + 8] = 1 * -1;
        this->positions[bottom_start + 8 + i] = init_rank[i];
        this->positions[bottom_start + i] = 1;
    }

    for (int i = 8 * 2; i < bottom_start; i++) {
        this->positions[i] = 0;
    }
}

auto Board::clear() -> void {
    std::fill(this->positions.begin(), this->positions.end(), 0);
}

auto Board::print() -> void {
    std::string out;
    out.reserve(256);

    out += '\n';

    for (int i = BOARD_DIM - 1; i >= 0; --i) {
        out += '1' + i;
        out += " |";
        for (uint64_t j = 0; j < BOARD_DIM; ++j) {
            int8_t value = this->positions[i * BOARD_DIM + j];

            if (value < 0) {
                out += "\033[1;34m";
            }
            switch (value) {
                case PieceCodes::NUM:
                    out += ' ';
                    break;
                case PieceCodes::PAWN:
                    //case -1 * PieceCodes::PAWN:
                    out += 'p';
                    break;
                case PieceCodes::KNIGHT:
                case -1 * PieceCodes::KNIGHT:
                    out += 'N';
                    break;
                case PieceCodes::BISHOP:
                case -1 * PieceCodes::BISHOP:
                    out += 'B';
                    break;
                case PieceCodes::ROOK:
                case -1 * PieceCodes::ROOK:
                    out += 'R';
                    break;
                case PieceCodes::QUEEN:
                case -1 * PieceCodes::QUEEN:
                    out += 'Q';
                    break;
                case PieceCodes::KING:
                case -1 * PieceCodes::KING:
                    out += 'K';
                    break;
                default:
                    out += ' ';
                    break;
            }

            if (value < 0) {
                out += "\033[0m";
            }

            out += '|';
        }
        out += '\n';
    }

    out += "\n  ";

    for (int i = 0; i < 8; ++i) {
        out += ' ';
        out += 'a' + i;
    }

    out += '\n';

    printf("%s", out.c_str());
}