#include "Definitions.hpp"

std::array<Square, Squares::NUM> PSUEDO_MOVES_KNIGHT;
std::array<Square, Squares::NUM> PSUEDO_MOVES_KING;
std::array<Square, Squares::NUM> PSUEDO_MOVES_WHITE_PAWNS;
std::array<Square, Squares::NUM> PSUEDO_MOVES_BLACK_PAWNS;

auto PieceCodes::to_char(PieceCode piece_code) noexcept -> char {
  switch (piece_code) {
    case PieceCodes::PAWN:
    {
      return ' ';
    }
    case PieceCodes::KNIGHT:
    {
      return 'N';
    }
    case PieceCodes::BISHOP:
    {
      return 'B';
    }
    case PieceCodes::ROOK:
    {
      return 'R';
    }
    case PieceCodes::QUEEN:
    {
      return 'Q';
    }
    case PieceCodes::KING:
    {
      return 'K';
    }
    default:
    {
      return '-';
    }
  }
}

auto Colors::bool_to_color(bool white_to_move) noexcept -> Color {
  return white_to_move ? Colors::WHITE : Colors::BLACK;
}