#pragma once

#include <array>
#include "Definitions.hpp"

namespace CachedMoves {
  extern std::array<BitBoard, Squares::NUM> KNIGHT;
  extern std::array<BitBoard, Squares::NUM> KING;
  extern std::array<BitBoard, Squares::NUM* Colors::NUM> PAWN_CAPTURES;
  extern std::array<BitBoard, Squares::NUM* Colors::NUM> PAWN;

  auto init() noexcept -> void;

  auto init_knight() noexcept -> void;
  auto init_king() noexcept -> void;
  auto init_pawn() noexcept -> void;

  auto get_knight_moves(const Square square) noexcept -> BitBoard;
  auto get_king_moves(const Square square) noexcept -> BitBoard;


  template<const Color color>
  auto get_pawn_capture_moves(const Square square) noexcept -> BitBoard {
    return CachedMoves::PAWN_CAPTURES[Squares::NUM * color + square];
  }

  template<const Color color>
  auto get_pawn_moves(const Square square) noexcept -> BitBoard {
    return CachedMoves::PAWN[Squares::NUM * color + square];
  }
}