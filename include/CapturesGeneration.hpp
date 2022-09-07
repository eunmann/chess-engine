#pragma once

#include "CommonHeaders.hpp"
#include "BitBoardUtils.hpp"

namespace CapturesGeneration {

    // Threaten Squares Templates
    template<const int V, const int H>
    constexpr auto
    get_captures_in_direction(BitBoard bit_board, const BitBoard occupied_bit_board) noexcept -> BitBoard {
        BitBoard capturable_bit_board = BitBoards::EMPTY;

        for (int i = 0; i < 8; ++i) {
            if constexpr (V > 0) {
                if (BitBoardUtils::is_piece_in_top_row(bit_board)) {
                    break;
                }
            } else if constexpr (V < 0) {
                if (BitBoardUtils::is_piece_in_bottom_row(bit_board)) {
                    break;
                }
            }

            if constexpr (H > 0) {
                if (BitBoardUtils::is_piece_in_right_col(bit_board)) {
                    break;
                }
            } else if constexpr (H < 0) {
                if (BitBoardUtils::is_piece_in_left_col(bit_board)) {
                    break;
                }
            }

            bit_board = BitBoardUtils::shift_bit_board<V, H>(bit_board);
            capturable_bit_board |= bit_board;

            if (occupied_bit_board.overlaps(bit_board)) {
                break;
            }
        }

        return capturable_bit_board;
    }

    template<const Color color>
    constexpr auto get_pawn_capture_positions(const BitBoard pawn_bit_board) noexcept -> BitBoard {
        constexpr auto pawn_dir = BitBoards::pawn_direction<color>();
        auto capturable_bit_board = BitBoards::EMPTY;
        const auto pawn_bit_board_left_capture = !BitBoardUtils::is_piece_in_left_col(pawn_bit_board) *
                                                 pawn_bit_board.shift<pawn_dir, -1>();
        const auto pawn_bit_board_right_capture = !BitBoardUtils::is_piece_in_right_col(pawn_bit_board) *
                                                  pawn_bit_board.shift<pawn_dir, 1>();
        capturable_bit_board |= pawn_bit_board_left_capture | pawn_bit_board_right_capture;
        return capturable_bit_board;
    }

    template<const Color color>
    constexpr auto get_pawn_move_positions(const BitBoard pawn_bit_board) noexcept -> BitBoard {
        constexpr auto pawn_dir = BitBoards::pawn_direction<color>();
        constexpr auto starting_row = BitBoards::pawn_start_row<color>();
        BitBoard moves_bit_board = BitBoards::EMPTY;

        moves_bit_board |= pawn_bit_board.shift<pawn_dir, 0>();
        moves_bit_board |= pawn_bit_board.overlaps(starting_row) * pawn_bit_board.shift<2 * pawn_dir, 0>();

        return moves_bit_board;
    }

    constexpr auto get_knight_capture_positions(const BitBoard knight_bit_board) noexcept -> BitBoard {
        BitBoard capturable_bit_board = BitBoards::EMPTY;
        {
            constexpr BitBoard negate_top_2_left_1 = (BitBoards::ROW_8 | BitBoards::ROW_7 | BitBoards::COL_A).invert();
            const BitBoard next_knight_bit_board = knight_bit_board.overlaps(negate_top_2_left_1) *
                                                   knight_bit_board.shift<2, -1>();
            capturable_bit_board |= next_knight_bit_board;
        }

        {
            constexpr BitBoard negate_top_2_right_1 = (BitBoards::ROW_8 | BitBoards::ROW_7 | BitBoards::COL_H).invert();
            const BitBoard next_knight_bit_board = knight_bit_board.overlaps(negate_top_2_right_1) *
                                                   knight_bit_board.shift<2, 1>();
            capturable_bit_board |= next_knight_bit_board;
        }

        {
            constexpr BitBoard negate_top_1_left_2 = (BitBoards::ROW_8 | BitBoards::COL_A | BitBoards::COL_B).invert();
            const BitBoard next_knight_bit_board = knight_bit_board.overlaps(negate_top_1_left_2) *
                                                   knight_bit_board.shift<1, -2>();
            capturable_bit_board |= next_knight_bit_board;
        }

        {
            constexpr BitBoard negate_top_1_right_2 = (BitBoards::ROW_8 | BitBoards::COL_G | BitBoards::COL_H).invert();
            const BitBoard next_knight_bit_board = knight_bit_board.overlaps(negate_top_1_right_2) *
                                                   knight_bit_board.shift<1, 2>();
            capturable_bit_board |= next_knight_bit_board;
        }

        {
            constexpr BitBoard negate_bottom_1_left_2 = (BitBoards::ROW_1 | BitBoards::COL_A |
                                                         BitBoards::COL_B).invert();
            const BitBoard next_knight_bit_board = knight_bit_board.overlaps(negate_bottom_1_left_2) *
                                                   knight_bit_board.shift<-1, -2>();
            capturable_bit_board |= next_knight_bit_board;
        }

        {
            constexpr BitBoard negate_bottom_1_right_2 = (BitBoards::ROW_1 | BitBoards::COL_G |
                                                          BitBoards::COL_H).invert();
            const BitBoard next_knight_bit_board = knight_bit_board.overlaps(negate_bottom_1_right_2) *
                                                   knight_bit_board.shift<-1, 2>();
            capturable_bit_board |= next_knight_bit_board;
        }

        {
            constexpr BitBoard negate_bottom_2_left_1 = (BitBoards::ROW_1 | BitBoards::ROW_2 |
                                                         BitBoards::COL_A).invert();
            BitBoard next_knight_bit_board = knight_bit_board.overlaps(negate_bottom_2_left_1) *
                                             knight_bit_board.shift<-2, -1>();
            capturable_bit_board |= next_knight_bit_board;
        }

        {
            constexpr BitBoard negate_bottom_2_right_1 = (BitBoards::ROW_1 | BitBoards::ROW_2 |
                                                          BitBoards::COL_H).invert();
            const BitBoard next_knight_bit_board = knight_bit_board.overlaps(negate_bottom_2_right_1) *
                                                   knight_bit_board.shift<-2, 1>();
            capturable_bit_board |= next_knight_bit_board;
        }

        return capturable_bit_board;
    }

    constexpr auto get_bishop_capture_positions(const BitBoard bishop_bit_board,
                                                const BitBoard occupied_bit_board) noexcept -> BitBoard {
        BitBoard capturable_bit_board = BitBoards::EMPTY;
        capturable_bit_board |= CapturesGeneration::get_captures_in_direction<1, 1>(bishop_bit_board,
                                                                                    occupied_bit_board);
        capturable_bit_board |= CapturesGeneration::get_captures_in_direction<-1, 1>(bishop_bit_board,
                                                                                     occupied_bit_board);
        capturable_bit_board |= CapturesGeneration::get_captures_in_direction<1, -1>(bishop_bit_board,
                                                                                     occupied_bit_board);
        capturable_bit_board |= CapturesGeneration::get_captures_in_direction<-1, -1>(bishop_bit_board,
                                                                                      occupied_bit_board);
        return capturable_bit_board;
    }

    constexpr auto
    get_rook_capture_positions(const BitBoard rook_bit_board, const BitBoard occupied_bit_board) noexcept -> BitBoard {
        BitBoard capturable_bit_board = BitBoards::EMPTY;
        capturable_bit_board |= CapturesGeneration::get_captures_in_direction<1, 0>(rook_bit_board, occupied_bit_board);
        capturable_bit_board |= CapturesGeneration::get_captures_in_direction<-1, 0>(rook_bit_board,
                                                                                     occupied_bit_board);
        capturable_bit_board |= CapturesGeneration::get_captures_in_direction<0, -1>(rook_bit_board,
                                                                                     occupied_bit_board);
        capturable_bit_board |= CapturesGeneration::get_captures_in_direction<0, 1>(rook_bit_board, occupied_bit_board);
        return capturable_bit_board;
    }

    constexpr auto get_queen_capture_positions(const BitBoard queen_bit_board,
                                               const BitBoard occupied_bit_board) noexcept -> BitBoard {
        BitBoard capturable_bit_board = BitBoards::EMPTY;
        // Diagonal
        capturable_bit_board |= CapturesGeneration::get_captures_in_direction<1, 1>(queen_bit_board,
                                                                                    occupied_bit_board);
        capturable_bit_board |= CapturesGeneration::get_captures_in_direction<-1, 1>(queen_bit_board,
                                                                                     occupied_bit_board);
        capturable_bit_board |= CapturesGeneration::get_captures_in_direction<1, -1>(queen_bit_board,
                                                                                     occupied_bit_board);
        capturable_bit_board |= CapturesGeneration::get_captures_in_direction<-1, -1>(queen_bit_board,
                                                                                      occupied_bit_board);

        // Vertical/Horizontal
        capturable_bit_board |= CapturesGeneration::get_captures_in_direction<1, 0>(queen_bit_board,
                                                                                    occupied_bit_board);
        capturable_bit_board |= CapturesGeneration::get_captures_in_direction<-1, 0>(queen_bit_board,
                                                                                     occupied_bit_board);
        capturable_bit_board |= CapturesGeneration::get_captures_in_direction<0, -1>(queen_bit_board,
                                                                                     occupied_bit_board);
        capturable_bit_board |= CapturesGeneration::get_captures_in_direction<0, 1>(queen_bit_board,
                                                                                    occupied_bit_board);

        return capturable_bit_board;
    }

    constexpr auto get_king_capture_positions(const BitBoard king_bit_board) noexcept -> BitBoard {
        BitBoard capturable_bit_board = BitBoards::EMPTY;

        // Up
        {
            const BitBoard next_king_position = king_bit_board.overlaps(BitBoards::ROW_8.invert()) *
                                                king_bit_board.shift<1, 0>();
            capturable_bit_board |= next_king_position;
        }

        // Down
        {
            const BitBoard next_king_position = king_bit_board.overlaps(BitBoards::ROW_1.invert()) *
                                                BitBoardUtils::shift_bit_board<-1, 0>(king_bit_board);
            capturable_bit_board |= next_king_position;
        }

        // Left
        {
            const BitBoard next_king_position = king_bit_board.overlaps(BitBoards::COL_A.invert()) *
                                                BitBoardUtils::shift_bit_board<0, -1>(king_bit_board);
            capturable_bit_board |= next_king_position;
        }

        // Right
        {
            const BitBoard next_king_position = king_bit_board.overlaps(BitBoards::COL_H.invert()) *
                                                BitBoardUtils::shift_bit_board<0, 1>(king_bit_board);
            capturable_bit_board |= next_king_position;
        }

        // Up-Right
        {
            constexpr BitBoard top_1_right_1 = (BitBoards::ROW_8 | BitBoards::COL_H).invert();
            const BitBoard next_king_position = king_bit_board.overlaps(top_1_right_1) *
                                                BitBoardUtils::shift_bit_board<1, 1>(king_bit_board);
            capturable_bit_board |= next_king_position;
        }

        // Up-Left
        {
            constexpr BitBoard top_1_left_1 = (BitBoards::ROW_8 | BitBoards::COL_A).invert();
            const BitBoard next_king_position = king_bit_board.overlaps(top_1_left_1) *
                                                BitBoardUtils::shift_bit_board<1, -1>(king_bit_board);
            capturable_bit_board |= next_king_position;
        }

        // Down-Right
        {
            constexpr BitBoard bottom_1_right_1 = (BitBoards::ROW_1 | BitBoards::COL_H).invert();
            const BitBoard next_king_position = king_bit_board.overlaps(bottom_1_right_1) *
                                                BitBoardUtils::shift_bit_board<-1, 1>(king_bit_board);
            capturable_bit_board |= next_king_position;
        }

        // Down-Left
        {
            constexpr BitBoard bottom_1_left_1 = (BitBoards::ROW_1 | BitBoards::COL_A).invert();
            BitBoard next_king_position = king_bit_board.overlaps(bottom_1_left_1) *
                                          BitBoardUtils::shift_bit_board<-1, -1>(king_bit_board);
            capturable_bit_board |= next_king_position;
        }

        return capturable_bit_board;
    }
}