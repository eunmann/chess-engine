#pragma once

#include "Definitions.hpp"

constexpr int32_t NUM_ROOK_MOVES = 4096;
constexpr int32_t NUM_BISHOP_MOVES = 1024;

namespace MagicBitBoards {
  extern std::array<BitBoard, Squares::NUM> BISHOP_MASKS;
  extern std::array<BitBoard, Squares::NUM> ROOK_MASKS;
  extern std::array<BitBoard, Squares::NUM* NUM_ROOK_MOVES> ROOK;
  extern std::array<BitBoard, Squares::NUM* NUM_BISHOP_MOVES> BISHOP;

  auto init() -> void;

  auto init_bishop() noexcept -> void;
  auto init_rook() noexcept -> void;

  auto get_bishop_moves(const Square square, const BitBoard blockers) noexcept -> BitBoard;
  auto get_rook_moves(const Square square, const BitBoard blockers) noexcept -> BitBoard;
}