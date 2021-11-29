#include "MoveGeneration.hpp"


auto MoveGeneration::get_knight_capture_positions(const BitBoard knights_bit_board) noexcept -> BitBoard {
  BitBoard capturable_bit_board = BitBoards::EMPTY;
  GameUtils::for_each_bit_board(knights_bit_board, [&capturable_bit_board](const auto knight_bit_board) {
    {
      constexpr BitBoard negate_top_2_left_1 = ~(BitBoards::ROW_8 | BitBoards::ROW_7 | BitBoards::COL_A);
      const BitBoard next_knight_bit_board = GameUtils::do_bit_boards_overlap(knight_bit_board, negate_top_2_left_1) * GameUtils::shift_bit_board<2, -1>(knight_bit_board);
      capturable_bit_board |= next_knight_bit_board;
    }

    {
      constexpr BitBoard negate_top_2_right_1 = ~(BitBoards::ROW_8 | BitBoards::ROW_7 | BitBoards::COL_H);
      const BitBoard next_knight_bit_board = GameUtils::do_bit_boards_overlap(knight_bit_board, negate_top_2_right_1) * GameUtils::shift_bit_board<2, 1>(knight_bit_board);
      capturable_bit_board |= next_knight_bit_board;
    }

    {
      constexpr BitBoard negate_top_1_left_2 = ~(BitBoards::ROW_8 | BitBoards::COL_A | BitBoards::COL_B);
      const BitBoard next_knight_bit_board = GameUtils::do_bit_boards_overlap(knight_bit_board, negate_top_1_left_2) * GameUtils::shift_bit_board<1, -2>(knight_bit_board);
      capturable_bit_board |= next_knight_bit_board;
    }

    {
      constexpr BitBoard negate_top_1_right_2 = ~(BitBoards::ROW_8 | BitBoards::COL_G | BitBoards::COL_H);
      const BitBoard next_knight_bit_board = GameUtils::do_bit_boards_overlap(knight_bit_board, negate_top_1_right_2) * GameUtils::shift_bit_board<1, 2>(knight_bit_board);
      capturable_bit_board |= next_knight_bit_board;
    }

    {
      constexpr BitBoard negate_bottom_1_left_2 = ~(BitBoards::ROW_1 | BitBoards::COL_A | BitBoards::COL_B);
      const BitBoard next_knight_bit_board = GameUtils::do_bit_boards_overlap(knight_bit_board, negate_bottom_1_left_2) * GameUtils::shift_bit_board<-1, -2>(knight_bit_board);
      capturable_bit_board |= next_knight_bit_board;
    }

    {
      constexpr BitBoard negate_bottom_1_right_2 = ~(BitBoards::ROW_1 | BitBoards::COL_G | BitBoards::COL_H);
      const BitBoard next_knight_bit_board = GameUtils::do_bit_boards_overlap(knight_bit_board, negate_bottom_1_right_2) * GameUtils::shift_bit_board<-1, 2>(knight_bit_board);
      capturable_bit_board |= next_knight_bit_board;
    }

    {
      constexpr BitBoard negate_bottom_2_left_1 = ~(BitBoards::ROW_1 | BitBoards::ROW_2 | BitBoards::COL_A);
      BitBoard next_knight_bit_board = GameUtils::do_bit_boards_overlap(knight_bit_board, negate_bottom_2_left_1) * GameUtils::shift_bit_board<-2, -1>(knight_bit_board);
      capturable_bit_board |= next_knight_bit_board;
    }

    {
      constexpr BitBoard negate_bottom_2_right_1 = ~(BitBoards::ROW_1 | BitBoards::ROW_2 | BitBoards::COL_H);
      const BitBoard next_knight_bit_board = GameUtils::do_bit_boards_overlap(knight_bit_board, negate_bottom_2_right_1) * GameUtils::shift_bit_board<-2, 1>(knight_bit_board);
      capturable_bit_board |= next_knight_bit_board;
    }
    });

  return capturable_bit_board;
}

auto MoveGeneration::get_cached_knight_capture_positions(const BitBoard knights_bit_board) noexcept ->BitBoard {
  BitBoard capturable_bit_board = BitBoards::EMPTY;
  GameUtils::for_each_set_square(knights_bit_board, [&capturable_bit_board](auto knight_square) {
    capturable_bit_board |= PSEDUO_MOVES_KNIGHT[knight_square];
    });
  return capturable_bit_board;
}

auto MoveGeneration::get_king_capture_positions(const BitBoard bit_board) noexcept -> BitBoard {
  BitBoard capturable_bit_board = BitBoards::EMPTY;

  GameUtils::for_each_bit_board(bit_board, [&capturable_bit_board](const BitBoard king_bit_board) {
    // Up
    {
      const BitBoard next_king_position = GameUtils::do_bit_boards_overlap(king_bit_board, ~BitBoards::ROW_8) * GameUtils::shift_bit_board<1, 0>(king_bit_board);
      capturable_bit_board |= next_king_position;
    }

    // Down
    {
      const BitBoard next_king_position = GameUtils::do_bit_boards_overlap(king_bit_board, ~BitBoards::ROW_1) * GameUtils::shift_bit_board<-1, 0>(king_bit_board);
      capturable_bit_board |= next_king_position;
    }

    // Left
    {
      const BitBoard next_king_position = GameUtils::do_bit_boards_overlap(king_bit_board, ~BitBoards::COL_A) * GameUtils::shift_bit_board<0, -1>(king_bit_board);
      capturable_bit_board |= next_king_position;
    }

    // Right
    {
      const BitBoard next_king_position = GameUtils::do_bit_boards_overlap(king_bit_board, ~BitBoards::COL_H) * GameUtils::shift_bit_board<0, 1>(king_bit_board);
      capturable_bit_board |= next_king_position;
    }

    // Up-Right
    {
      constexpr BitBoard top_1_right_1 = BitBoards::ROW_8 | BitBoards::COL_H;
      const BitBoard next_king_position = GameUtils::do_bit_boards_overlap(king_bit_board, ~top_1_right_1) * GameUtils::shift_bit_board<1, 1>(king_bit_board);
      capturable_bit_board |= next_king_position;
    }

    // Up-Left
    {
      constexpr BitBoard top_1_left_1 = BitBoards::ROW_8 | BitBoards::COL_A;
      const BitBoard next_king_position = GameUtils::do_bit_boards_overlap(king_bit_board, ~top_1_left_1) * GameUtils::shift_bit_board<1, -1>(king_bit_board);
      capturable_bit_board |= next_king_position;
    }

    // Down-Right
    {
      constexpr BitBoard bottom_1_right_1 = BitBoards::ROW_1 | BitBoards::COL_H;
      const BitBoard next_king_position = GameUtils::do_bit_boards_overlap(king_bit_board, ~bottom_1_right_1) * GameUtils::shift_bit_board<-1, 1>(king_bit_board);
      capturable_bit_board |= next_king_position;
    }

    // Down-Left
    {
      constexpr BitBoard bottom_1_left_1 = BitBoards::ROW_1 | BitBoards::COL_A;
      BitBoard next_king_position = GameUtils::do_bit_boards_overlap(king_bit_board, ~bottom_1_left_1) * GameUtils::shift_bit_board<-1, -1>(king_bit_board);
      capturable_bit_board |= next_king_position;
    }
    });

  return capturable_bit_board;
}

auto MoveGeneration::get_cached_king_capture_positions(const BitBoard kings_bit_board) noexcept -> BitBoard {
  BitBoard capturable_bit_board = BitBoards::EMPTY;
  GameUtils::for_each_set_square(kings_bit_board, [&capturable_bit_board](auto king_square) {
    capturable_bit_board |= PSEDUO_MOVES_KNIGHT[king_square];
    });
  return capturable_bit_board;

}