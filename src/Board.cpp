#include "Board.hpp"

#include <stdio.h>

#include <algorithm>
#include <string>

#include "Definitions.hpp"

Board::Board() {
    this->clear();
};

auto Board::init_standard() -> void {
    constexpr std::array<PieceCode, 8> init_rank{
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
    std::fill(this->positions.begin(), this->positions.end(), BoardValues::EMPTY);
}

auto Board::print() const -> void {
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
                case BoardValues::EMPTY:
                    out += ' ';
                    break;
                case BoardValues::PAWN:
                case -1 * BoardValues::PAWN:
                    out += 'p';
                    break;
                case BoardValues::KNIGHT:
                case -1 * BoardValues::KNIGHT:
                    out += 'N';
                    break;
                case BoardValues::BISHOP:
                case -1 * BoardValues::BISHOP:
                    out += 'B';
                    break;
                case BoardValues::ROOK:
                case -1 * BoardValues::ROOK:
                    out += 'R';
                    break;
                case BoardValues::QUEEN:
                case -1 * BoardValues::QUEEN:
                    out += 'Q';
                    break;
                case BoardValues::KING:
                case -1 * BoardValues::KING:
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