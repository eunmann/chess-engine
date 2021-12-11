#include "MagicBitBoards.hpp"
#include "Position.hpp"
#include "MoveGeneration.hpp"

namespace MagicBitBoards {

  std::array<BitBoard, Squares::NUM> BISHOP_MASKS;
  std::array<BitBoard, Squares::NUM> ROOK_MASKS;
  std::array<BitBoard, Squares::NUM* NUM_ROOK_MOVES> ROOK;
  std::array<BitBoard, Squares::NUM* NUM_BISHOP_MOVES> BISHOP;

  auto init() -> void {
    init_bishop();
    init_rook();
  }

  auto init_bishop() noexcept -> void {

    for (Square s = Squares::A1; s < Squares::NUM; s++) {
      const BitBoard bit_board = BitBoardUtils::square_to_bit_board(s);

      Position position;
      position.add(PieceCodes::BISHOP, Colors::WHITE, bit_board);
      BitBoard moves = BitBoards::EMPTY;

      moves |= MoveGeneration::get_captures_in_direction<1, 1>(position, bit_board);
      moves |= MoveGeneration::get_captures_in_direction<-1, 1>(position, bit_board);
      moves |= MoveGeneration::get_captures_in_direction<1, -1>(position, bit_board);
      moves |= MoveGeneration::get_captures_in_direction<-1, -1>(position, bit_board);
      moves &= ~BitBoards::EDGES;

      BISHOP_MASKS[s] = moves;
    }

    int RBits[64] = {
  12, 11, 11, 11, 11, 11, 11, 12,
  11, 10, 10, 10, 10, 10, 10, 11,
  11, 10, 10, 10, 10, 10, 10, 11,
  11, 10, 10, 10, 10, 10, 10, 11,
  11, 10, 10, 10, 10, 10, 10, 11,
  11, 10, 10, 10, 10, 10, 10, 11,
  11, 10, 10, 10, 10, 10, 10, 11,
  12, 11, 11, 11, 11, 11, 11, 12
    };

    int BBits[64] = {
      6, 5, 5, 5, 5, 5, 5, 6,
      5, 5, 5, 5, 5, 5, 5, 5,
      5, 5, 7, 7, 7, 7, 5, 5,
      5, 5, 7, 9, 9, 7, 5, 5,
      5, 5, 7, 9, 9, 7, 5, 5,
      5, 5, 7, 7, 7, 7, 5, 5,
      5, 5, 5, 5, 5, 5, 5, 5,
      6, 5, 5, 5, 5, 5, 5, 6
    };
  }

  auto init_rook() noexcept -> void {

    for (Square s = Squares::A1; s < Squares::NUM; s++) {
      const BitBoard bit_board = BitBoardUtils::square_to_bit_board(s);

      Position position;
      position.add(PieceCodes::BISHOP, Colors::WHITE, bit_board);
      BitBoard moves = BitBoards::EMPTY;

      moves |= MoveGeneration::get_captures_in_direction<1, 0>(position, bit_board) & ~BitBoards::COL_A;
      moves |= MoveGeneration::get_captures_in_direction<-1, 0>(position, bit_board) & ~BitBoards::COL_H;
      moves |= MoveGeneration::get_captures_in_direction<0, -1>(position, bit_board) & ~BitBoards::ROW_1;
      moves |= MoveGeneration::get_captures_in_direction<0, 1>(position, bit_board) & ~BitBoards::ROW_8;

      ROOK_MASKS[s] = moves;
    }
  }

  auto get_bishop_moves(const Square square, const BitBoard blockers) noexcept -> BitBoard {
    return 0;
  }

  auto get_rook_moves(const Square square, const BitBoard blockers) noexcept -> BitBoard {
    return 0;
  }
}