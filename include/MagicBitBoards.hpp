#pragma once

#include "Definitions.hpp"

constexpr int32_t MAX_ROOK_BLOCKERS = 12;
constexpr int32_t MAX_ROOK_PERMUTATIONS = 1 << MAX_ROOK_BLOCKERS;
constexpr int32_t MAX_BISHOP_BLOCKERS = 9;
constexpr int32_t MAX_BISHOP_PERMUTATIONS = 1 << MAX_BISHOP_BLOCKERS;

namespace MagicBitBoards {
  extern std::array<BitBoard, Squares::NUM> BISHOP_BLOCKER_MASKS;
  extern std::array<uint64_t, Squares::NUM> BISHOP_MAGICS;
  extern std::array<std::array<BitBoard, MAX_BISHOP_PERMUTATIONS>, Squares::NUM> BISHOP_MOVES;


  extern std::array<BitBoard, Squares::NUM> ROOK_BLOCKER_MASKS;
  extern std::array<uint64_t, Squares::NUM> ROOK_MAGICS;
  extern std::array<std::array<BitBoard, MAX_ROOK_PERMUTATIONS>, Squares::NUM> ROOK_MOVES;

  auto init() noexcept -> void;

  auto init_bishop() noexcept -> void;
  auto init_rook() noexcept -> void;

  auto get_bishop_moves(const Square square, const BitBoard blockers) noexcept -> BitBoard;
  auto get_rook_moves(const Square square, const BitBoard blockers) noexcept -> BitBoard;

  auto generate_blocker_board_permutation(const Square square, const BitBoard blocker_mask) noexcept -> BitBoard;
}