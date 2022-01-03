#include "CachedMoves.hpp"
#include <ranges>
#include "BitBoardUtils.hpp"
#include "MoveGeneration.hpp"

namespace CachedMoves {
  std::array<BitBoard, Squares::NUM> KNIGHT;
  std::array<BitBoard, Squares::NUM> KING;
  std::array<BitBoard, Squares::NUM* Colors::NUM> PAWN_CAPTURES;
  std::array<BitBoard, Squares::NUM* Colors::NUM> PAWN;

  auto init() noexcept -> void {
    CachedMoves::init_king();
    CachedMoves::init_knight();
    CachedMoves::init_pawn();
  }

  auto init_knight() noexcept -> void {
    for (auto square : std::views::iota(Squares::A1, Squares::NUM)) {
      const BitBoard bit_board = BitBoardUtils::square_to_bit_board(square);
      CachedMoves::KNIGHT[square] = MoveGeneration::get_knight_capture_positions(bit_board);
    }
  }
  auto init_king() noexcept -> void {
    for (auto square : std::views::iota(Squares::A1, Squares::NUM)) {
      const BitBoard bit_board = BitBoardUtils::square_to_bit_board(square);
      CachedMoves::KING[square] = MoveGeneration::get_king_capture_positions(bit_board);
    }
  }
  auto init_pawn() noexcept -> void {
    for (auto square : std::views::iota(Squares::A1, Squares::NUM)) {
      const BitBoard bit_board = BitBoardUtils::square_to_bit_board(square);
      CachedMoves::PAWN_CAPTURES[Squares::NUM * Colors::WHITE + square] = MoveGeneration::get_pawn_capture_positions<Colors::WHITE>(bit_board);
      CachedMoves::PAWN[Squares::NUM * Colors::WHITE + square] = MoveGeneration::get_pawn_move_positions<Colors::WHITE>(bit_board);

      CachedMoves::PAWN_CAPTURES[Squares::NUM * Colors::BLACK + square] = MoveGeneration::get_pawn_capture_positions<Colors::BLACK>(bit_board);
      CachedMoves::PAWN[Squares::NUM * Colors::BLACK + square] = MoveGeneration::get_pawn_move_positions<Colors::BLACK>(bit_board);
    }
  }

  auto get_knight_moves(const Square square) noexcept -> BitBoard {
    return CachedMoves::KNIGHT[square];
  }
  auto get_king_moves(const Square square) noexcept -> BitBoard {
    return CachedMoves::KING[square];
  }
}