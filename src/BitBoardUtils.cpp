#include "BitBoardUtils.hpp"
#include "Assert.hpp"
#include <bit>

auto BitBoardUtils::square_to_bit_board(const Square square) noexcept -> BitBoard {
  return 0b1ULL << square;
}

auto BitBoardUtils::print_position(const BitBoard bit_board) noexcept -> void {
  const BitBoard left_bit = 1ULL << 63;
  BitBoard temp_bit_board = bit_board;
  std::string output;
  output.reserve(64);

  for (int32_t j = 0; j < 8; ++j) {
    for (int32_t i = 0; i < 8; ++i) {
      if (left_bit & temp_bit_board) {
        output += '1';
      } else {
        output += '0';
      }
      temp_bit_board <<= 1;
    }
    output += '\n';
  }

  printf("%s", output.c_str());
}

auto BitBoardUtils::shift_bit_board(const BitBoard bit_board,
  const int32_t vertical,
  const int32_t horizontal) noexcept -> BitBoard {
  ASSERT(vertical > -8 && vertical < 8);
  ASSERT(horizontal > -8 && horizontal < 8);

  int32_t shift = vertical * 8 + horizontal;
  if (shift >= 0) {
    return bit_board << shift;
  } else {
    return bit_board >> (shift * -1);
  }
}

auto BitBoardUtils::get_row(const BitBoard bit_board) noexcept -> int32_t {
  BitBoard row_mask = BitBoards::ROW_1;

  for (int i = 0; i < 8; ++i) {
    if ((bit_board & row_mask) != 0) {
      return i;
    }
    row_mask = row_mask << 8;
  }

  return -1;
}

auto BitBoardUtils::get_col(const BitBoard bit_board) noexcept -> int32_t {
  BitBoard col_mask = BitBoards::COL_A;

  for (int i = 0; i < 8; ++i) {
    if ((bit_board & col_mask) != 0) {
      return i;
    }
    col_mask = col_mask << 1;
  }

  return -1;
}

auto BitBoardUtils::get_row_col(const BitBoard bit_board, int32_t& row,
  int32_t& col) noexcept -> void {
  BitBoard row_mask = BitBoards::ROW_1;
  BitBoard col_mask = BitBoards::COL_A;

  row = -1;
  col = -1;

  for (int i = 0; i < 8; ++i) {
    if ((bit_board & row_mask) != 0) {
      row = i;
    }
    if ((bit_board & col_mask) != 0) {
      col = i;
    }
    row_mask = row_mask << 8;
    col_mask = col_mask << 1;
  }
}

auto BitBoardUtils::get_tile_name(const BitBoard bit_board) noexcept -> std::string {
  int32_t row, col;
  BitBoardUtils::get_row_col(bit_board, row, col);

  std::string name = "";
  name += (char)(col + 'a');
  name += (char)(row + '1');

  return name;
}

auto BitBoardUtils::do_bit_boards_overlap(const BitBoard bit_board_1, const BitBoard bit_board_2) noexcept -> bool {
  return (bit_board_1 & bit_board_2) != 0;
}

auto BitBoardUtils::is_piece_in_row(const BitBoard bit_board, const int32_t row) noexcept -> bool {
  const BitBoard row_mask = BitBoards::ROW_1 << (row * 8);
  return BitBoardUtils::do_bit_boards_overlap(row_mask, bit_board);
}

auto BitBoardUtils::is_piece_in_col(const BitBoard bit_board, const int32_t col) noexcept -> bool {
  const BitBoard col_mask = BitBoards::COL_A << col;
  return BitBoardUtils::do_bit_boards_overlap(col_mask, bit_board);
}

auto BitBoardUtils::is_piece_in_top_row(const BitBoard bit_board) noexcept -> bool {
  return BitBoardUtils::do_bit_boards_overlap(BitBoards::ROW_8, bit_board);
}


auto BitBoardUtils::is_piece_in_bottom_row(const BitBoard bit_board) noexcept -> bool {
  return BitBoardUtils::do_bit_boards_overlap(BitBoards::ROW_1, bit_board);
}

auto BitBoardUtils::is_piece_in_left_col(const BitBoard bit_board) noexcept -> bool {
  return BitBoardUtils::do_bit_boards_overlap(BitBoards::COL_A, bit_board);
}

auto BitBoardUtils::is_piece_in_right_col(const BitBoard bit_board) noexcept -> bool {
  return BitBoardUtils::do_bit_boards_overlap(BitBoards::COL_H, bit_board);
}

auto BitBoardUtils::bit_board_to_square(const BitBoard bit_board) noexcept -> Square {

  if (bit_board == BitBoards::EMPTY) {
    return -1;
  }

  unsigned long index = 0;
  _BitScanForward64(&index, bit_board);
  return index;
}

auto BitBoardUtils::get_count(const BitBoard bit_board) noexcept -> int32_t {
  return std::popcount(bit_board);
}