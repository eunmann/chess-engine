#pragma once

#include <inttypes.h>

#include <array>

constexpr uint64_t BOARD_DIM = 8;
constexpr uint64_t NUM_PIECES = 6;
constexpr uint64_t MAX_NUM_MOVES = 256;

typedef int32_t Square;

namespace Squares {
  constexpr Square A1 = 0, A2 = 1, A3 = 2, A4 = 3, A5 = 4, A6 = 5, A7 = 6, A8 = 7,
    B1 = 8, B2 = 9, B3 = 10, B4 = 11, B5 = 12, B6 = 13, B7 = 14,
    B8 = 15, C1 = 16, C2 = 17, C3 = 18, C4 = 19, C5 = 20, C6 = 21,
    C7 = 22, C8 = 23, D1 = 24, D2 = 25, D3 = 26, D4 = 27, D5 = 28,
    D6 = 29, D7 = 30, D8 = 31, E1 = 32, E2 = 33, E3 = 34, E4 = 35,
    E5 = 36, E6 = 37, E7 = 38, E8 = 39, F1 = 40, F2 = 41, F3 = 42,
    F4 = 43, F5 = 44, F6 = 45, F7 = 46, F8 = 47, G1 = 48, G2 = 49,
    G3 = 50, G4 = 51, G5 = 52, G6 = 53, G7 = 54, G8 = 55, H1 = 56,
    H2 = 57, H3 = 58, H4 = 59, H5 = 60, H6 = 61, H7 = 62, H8 = 63,
    NUM = 64;
}

typedef int32_t PieceCode;
namespace PieceCodes {
  constexpr PieceCode PAWN = 0, KNIGHT = 1, BISHOP = 2, ROOK = 3, QUEEN = 4,
    KING = 5, NUM = 6;
  constexpr std::array<PieceCode, 6> ALL{PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING};
  auto to_char(PieceCode piece_code) noexcept -> char;
}  // namespace PieceCodes

typedef int32_t PieceValue;

namespace PieceValues {
  constexpr PieceValue PAWN = 100, KNIGHT = 300, BISHOP = 300, ROOK = 500,
    QUEEN = 900, KING = 300000, POS_INFINITY = KING + 1,
    NEG_INFINITY = -1 * POS_INFINITY;
};

typedef int32_t Color;
namespace Colors {
  constexpr Color WHITE = 0, BLACK = 1, NUM = 2;
  auto bool_to_color(bool white_to_move)noexcept ->Color;
};  // namespace Colors

typedef uint64_t BitBoard;

namespace BitBoards {

  // Entire Boards
  constexpr BitBoard EMPTY = 0ULL;
  constexpr BitBoard ALL_SQUARES = ~EMPTY;

  // Rows
  constexpr BitBoard ROW_1 = 0xFFULL;
  constexpr BitBoard ROW_2 = ROW_1 << 8;
  constexpr BitBoard ROW_3 = ROW_2 << 8;
  constexpr BitBoard ROW_4 = ROW_3 << 8;
  constexpr BitBoard ROW_5 = ROW_4 << 8;
  constexpr BitBoard ROW_6 = ROW_5 << 8;
  constexpr BitBoard ROW_7 = ROW_6 << 8;
  constexpr BitBoard ROW_8 = ROW_7 << 8;

  // Cols
  constexpr BitBoard COL_A = 0x0101010101010101ULL;
  constexpr BitBoard COL_B = COL_A << 1;
  constexpr BitBoard COL_C = COL_B << 1;
  constexpr BitBoard COL_D = COL_C << 1;
  constexpr BitBoard COL_E = COL_D << 1;
  constexpr BitBoard COL_F = COL_E << 1;
  constexpr BitBoard COL_G = COL_F << 1;
  constexpr BitBoard COL_H = COL_G << 1;

  // King Starting
  constexpr BitBoard WHITE_KING_START = ROW_1 & COL_E;
  constexpr BitBoard BLACK_KING_START = ROW_8 & COL_E;

  // Rook Starting
  constexpr BitBoard WHITE_ROOK_A_START = ROW_1 & COL_A;
  constexpr BitBoard WHITE_ROOK_H_START = ROW_1 & COL_H;
  constexpr BitBoard BLACK_ROOK_A_START = ROW_8 & COL_A;
  constexpr BitBoard BLACK_ROOK_H_START = ROW_8 & COL_H;

  // Castle Empty
  constexpr BitBoard WHITE_KING_CASTLE = ROW_1 & (COL_F | COL_G);
  constexpr BitBoard WHITE_QUEEN_CASTLE = ROW_1 & (COL_B | COL_C | COL_D);

  constexpr BitBoard BLACK_KING_CASTLE = ROW_8 & (COL_F | COL_G);
  constexpr BitBoard BLACK_QUEEN_CASTLE = ROW_8 & (COL_B | COL_C | COL_D);

  // King Castle End Positions
  constexpr BitBoard WHITE_KING_KING_CASTLE = ROW_1 & COL_G;
  constexpr BitBoard WHITE_ROOK_KING_CASTLE = ROW_1 & COL_F;
  constexpr BitBoard WHITE_KING_QUEEN_CASTLE = ROW_1 & COL_C;
  constexpr BitBoard WHITE_ROOK_QUEEN_CASTLE = ROW_1 & COL_D;

  constexpr BitBoard BLACK_KING_KING_CASTLE = ROW_8 & COL_G;
  constexpr BitBoard BLACK_ROOK_KING_CASTLE = ROW_8 & COL_F;
  constexpr BitBoard BLACK_KING_QUEEN_CASTLE = ROW_8 & COL_C;
  constexpr BitBoard BLACK_ROOK_QUEEN_CASTLE = ROW_8 & COL_D;

  constexpr BitBoard CENTER_4_SQUARES = (ROW_4 | ROW_5) & (COL_D | COL_E);
  constexpr BitBoard CENTER_16_SQUARES = (ROW_3 | ROW_4 | ROW_5 | ROW_6) & (COL_C | COL_D | COL_E | COL_F);
}  // namespace BitBoards

extern std::array<Square, Squares::NUM> PSUEDO_MOVES_KNIGHT;
extern std::array<Square, Squares::NUM> PSUEDO_MOVES_KING;

typedef int32_t Castle;
namespace Castles {
  constexpr Castle NONE = 0b000;
  constexpr Castle WHITE_KING = 0b100;
  constexpr Castle WHITE_QUEEN = 0b101;
  constexpr Castle BLACK_KING = 0b110;
  constexpr Castle BLACK_QUEEN = 0b111;
  constexpr std::array<Castle, 4> ALL{WHITE_KING,WHITE_QUEEN, BLACK_KING, BLACK_QUEEN};
}  // namespace Castles
