#pragma once

#include "Definitions.hpp"

constexpr int32_t NUM_ROOK_MOVES = 1 << 12;
constexpr int32_t NUM_BISHOP_MOVES = 1 << 10;

namespace MagicBitBoards {
  extern std::array<BitBoard, Squares::NUM> BISHOP_BLOCKER_MASKS;
  extern std::array<BitBoard, Squares::NUM* NUM_BISHOP_MOVES> BISHOP_BLOCKERS;
  extern std::array<BitBoard, Squares::NUM* NUM_BISHOP_MOVES> BISHOP_MOVES;
  extern std::array<uint64_t, Squares::NUM> BISHOP_MAGICS;

  extern std::array<BitBoard, Squares::NUM> ROOK_BLOCKER_MASKS;
  extern std::array<BitBoard, Squares::NUM* NUM_ROOK_MOVES> ROOK_BLOCKERS;
  extern std::array<BitBoard, Squares::NUM* NUM_ROOK_MOVES> ROOK_MOVES;
  extern std::array<uint64_t, Squares::NUM> ROOK_MAGICS;

  auto init() -> void;

  auto init_bishop() noexcept -> void;
  auto init_rook() noexcept -> void;

  auto get_bishop_moves(const Square square, const BitBoard blockers) noexcept -> BitBoard;
  auto get_rook_moves(const Square square, const BitBoard blockers) noexcept -> BitBoard;

  auto generate_blocker_board(const Square square, const BitBoard blocker_mask)->BitBoard;
}