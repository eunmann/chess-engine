#include "Definitions.hpp"

std::array<BitBoard, Squares::NUM> PSUEDO_MOVES_KNIGHT;
std::array<BitBoard, Squares::NUM> PSUEDO_MOVES_KING;

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