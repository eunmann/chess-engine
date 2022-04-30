#include "Board.hpp"

#include <cstdio>

#include <algorithm>
#include <iostream>
#include <string>

#include "Definitions.hpp"
#include "color/color.hpp"

Board::Board() noexcept {
    this->clear();
};

auto Board::init_standard() noexcept -> void {
    constexpr std::array<PieceCode, 8> init_rank{
            PieceCodes::ROOK, PieceCodes::KNIGHT, PieceCodes::BISHOP,
            PieceCodes::QUEEN, PieceCodes::KING, PieceCodes::BISHOP,
            PieceCodes::KNIGHT, PieceCodes::ROOK};

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

auto Board::clear() noexcept -> void {
    std::fill(this->positions.begin(), this->positions.end(), BoardValues::EMPTY);
}

auto Board::print() const noexcept -> void {
#ifdef __linux__
    std::string out;
    out.reserve(512);
#elif _WIN32
    auto out = dye::vanilla("");
#endif

    out += "\n";

    for (char i = BOARD_DIM - 1; i >= 0; --i) {
        out += std::string(1, '1' + i);
        out += " |";
        for (uint64_t j = 0; j < BOARD_DIM; ++j) {
            int8_t value = this->positions[i * BOARD_DIM + j];

#ifdef __linux__
            if (value < 0) {
              out += "\033[1;34m";
            }
#endif
            std::string c;
            switch (value) {
                case BoardValues::EMPTY:
                    c = ' ';
                    break;
                case BoardValues::PAWN:
                case -1 * BoardValues::PAWN:
                    c = 'p';
                    break;
                case BoardValues::KNIGHT:
                case -1 * BoardValues::KNIGHT:
                    c = 'N';
                    break;
                case BoardValues::BISHOP:
                case -1 * BoardValues::BISHOP:
                    c = 'B';
                    break;
                case BoardValues::ROOK:
                case -1 * BoardValues::ROOK:
                    c = 'R';
                    break;
                case BoardValues::QUEEN:
                case -1 * BoardValues::QUEEN:
                    c = 'Q';
                    break;
                case BoardValues::KING:
                case -1 * BoardValues::KING:
                    c = 'K';
                    break;
                default:
                    c = ' ';
                    break;
            }

#ifdef __linux__
            if (value < 0) {
              out += c;
              out += "\033[0m";
            }
#elif _WIN32
            if (value < 0) {
                out += dye::purple(c);
            } else {
                out += c;
            }
#endif

            out += "|";
        }
        out += "\n";
    }

    out += "\n  ";

    for (char i = 0; i < 8; i++) {
        out += " ";
        out += std::string(1, 'a' + i);
    }

    out += "\n";

    std::cout << out << "\n";
}