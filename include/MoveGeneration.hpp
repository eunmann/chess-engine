#pragma once

#include "GameState.hpp"
#include "GameUtils.hpp"
#include "Move.hpp"
#include "Moves.hpp"
#include "BitBoardUtils.hpp"

namespace MoveGeneration {

  // PSUEDO-Legal Moves Templates
  template <const int V, const int H, const Color color>
  auto get_moves_in_direction(const GameState& game_state, BitBoard bit_board, Moves& moves) noexcept -> void {
    Square source_square = BitBoardUtils::bit_board_to_square(bit_board);
    for (int i = 0; i < 7; ++i) {
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

      if (game_state.position.is_empty(bit_board)) {
        moves.push_back(Move(source_square, BitBoardUtils::bit_board_to_square(bit_board)));
      } else if (game_state.position.is_color_occupied<color>(bit_board)) {
        break;
      } else {
        moves.push_back(Move(source_square, BitBoardUtils::bit_board_to_square(bit_board)));
        break;
      }
    }
  }

  template <const Color color>
  auto get_pawn_moves(const GameState& game_state, Moves& moves) noexcept -> void {
    constexpr int64_t pawn_dir = color == Colors::WHITE ? 1 : -1;
    constexpr int32_t promotion_row = color == Colors::WHITE ? 7 : 0;
    constexpr int32_t starting_row = color == Colors::WHITE ? 1 : 6;
    constexpr Color opponent_color = color == Colors::WHITE ? Colors::BLACK : Colors::WHITE;
    const BitBoard pawns_bit_board = game_state.position.get_piece_color_bit_board<PieceCodes::PAWN, color>();

    BitBoardUtils::for_each_bit_board(pawns_bit_board, [&game_state, &moves](const BitBoard pawn_bit_board) {
      const Square source_square = BitBoardUtils::bit_board_to_square(pawn_bit_board);

      BitBoard pawn_forward = BitBoardUtils::shift_bit_board<1 * pawn_dir, 0>(pawn_bit_board);
      const bool is_forward_empty = game_state.position.is_empty(pawn_forward);
      pawn_forward *= is_forward_empty;

      BitBoard pawn_forward_2 = BitBoardUtils::shift_bit_board<2 * pawn_dir, 0>(pawn_bit_board);
      pawn_forward_2 *= is_forward_empty && game_state.position.is_empty(pawn_forward_2) && BitBoardUtils::is_piece_in_row(pawn_bit_board, starting_row);

      BitBoard pawn_bit_board_left_capture = BitBoardUtils::shift_bit_board<1 * pawn_dir, -1>(pawn_bit_board);
      pawn_bit_board_left_capture *= !BitBoardUtils::is_piece_in_left_col(pawn_bit_board) && game_state.position.is_color_occupied<opponent_color>(pawn_bit_board_left_capture);

      BitBoard pawn_bit_board_right_capture = BitBoardUtils::shift_bit_board<1 * pawn_dir, 1>(pawn_bit_board);
      pawn_bit_board_right_capture *= !BitBoardUtils::is_piece_in_right_col(pawn_bit_board) && game_state.position.is_color_occupied<opponent_color>(pawn_bit_board_left_capture);

      const BitBoard move_bit_board = pawn_bit_board_left_capture | pawn_bit_board_right_capture | pawn_forward | pawn_forward_2;

      if (BitBoardUtils::is_piece_in_row(move_bit_board, promotion_row)) {
        BitBoardUtils::for_each_set_square(move_bit_board, [&game_state, &moves, source_square](const Square destination_square) {
          for (int32_t i = PieceCodes::KNIGHT; i <= PieceCodes::QUEEN; i++) {
            Move move(source_square, destination_square);
            move.set_promotion(i);
            moves.push_back(move);
          }});
      } else {
        BitBoardUtils::for_each_set_square(move_bit_board, [&game_state, &moves, source_square](const Square destination_square) {
          moves.push_back(Move(source_square, destination_square));
          });
      }
      });
  }

  template <const Color color>
  auto get_knight_moves(const GameState& game_state, Moves& moves) noexcept -> void {
    BitBoard knights_bit_board = game_state.position.get_piece_color_bit_board<PieceCodes::KNIGHT, color>();

    BitBoardUtils::for_each_set_square(knights_bit_board, [&game_state, &moves](const auto source_square) {
      BitBoardUtils::for_each_set_square(PSUEDO_MOVES_KNIGHT[source_square], [source_square, &moves](auto dest_square) {
        moves.push_back(Move(source_square, dest_square));
        });
      });
  }

  template <const Color color>
  auto get_bishop_moves(const GameState& game_state, Moves& moves) noexcept -> void {
    BitBoard bishops_bit_board = game_state.position.get_piece_color_bit_board<PieceCodes::BISHOP, color>();

    BitBoardUtils::for_each_bit_board(bishops_bit_board, [&game_state, &moves](const BitBoard bishop_bit_board) {
      MoveGeneration::get_moves_in_direction<1, 1, color>(game_state, bishop_bit_board, moves);
      MoveGeneration::get_moves_in_direction<-1, 1, color>(game_state, bishop_bit_board, moves);
      MoveGeneration::get_moves_in_direction<1, -1, color>(game_state, bishop_bit_board, moves);
      MoveGeneration::get_moves_in_direction<-1, -1, color>(game_state, bishop_bit_board, moves);
      });
  }

  template <const Color color>
  auto get_rook_moves(const GameState& game_state, Moves& moves) noexcept -> void {
    BitBoard rooks_bit_board = game_state.position.get_piece_color_bit_board<PieceCodes::ROOK, color>();

    BitBoardUtils::for_each_bit_board(
      rooks_bit_board, [&game_state, &moves](const BitBoard rook_bit_board) {
        MoveGeneration::get_moves_in_direction<1, 0, color>(game_state, rook_bit_board, moves);
        MoveGeneration::get_moves_in_direction<-1, 0, color>(game_state, rook_bit_board, moves);
        MoveGeneration::get_moves_in_direction<0, -1, color>(game_state, rook_bit_board, moves);
        MoveGeneration::get_moves_in_direction<0, 1, color>(game_state, rook_bit_board, moves);
      });
  }

  template <const Color color>
  auto get_queen_moves(const GameState& game_state, Moves& moves) noexcept -> void {
    BitBoard queens_bit_board = game_state.position.get_piece_color_bit_board<PieceCodes::QUEEN, color>();
    BitBoardUtils::for_each_bit_board(queens_bit_board, [&game_state, &moves](BitBoard queen_bit_board) {
      // Diagonal
      MoveGeneration::get_moves_in_direction<1, 1, color>(game_state, queen_bit_board, moves);
      MoveGeneration::get_moves_in_direction<-1, 1, color>(game_state, queen_bit_board, moves);
      MoveGeneration::get_moves_in_direction<1, -1, color>(game_state, queen_bit_board, moves);
      MoveGeneration::get_moves_in_direction<-1, -1, color>(game_state, queen_bit_board, moves);

      // Horizontal and vertical
      MoveGeneration::get_moves_in_direction<1, 0, color>(game_state, queen_bit_board, moves);
      MoveGeneration::get_moves_in_direction<-1, 0, color>(game_state, queen_bit_board, moves);
      MoveGeneration::get_moves_in_direction<0, -1, color>(game_state, queen_bit_board, moves);
      MoveGeneration::get_moves_in_direction<0, 1, color>(game_state, queen_bit_board, moves);
      });
  }

  template <const Color color>
  auto get_king_moves(const GameState& game_state, Moves& moves) noexcept -> void {
    BitBoard kings_bit_board =
      game_state.position.get_piece_color_bit_board<PieceCodes::KING, color>();

    BitBoardUtils::for_each_bit_board(kings_bit_board, [&game_state, &moves](BitBoard king_bit_board) {
      Square source_square = BitBoardUtils::bit_board_to_square(king_bit_board);

      BitBoardUtils::for_each_set_square(PSUEDO_MOVES_KING[source_square], [source_square, &moves](auto dest_square) {
        moves.push_back(Move(source_square, dest_square));
        });

      constexpr Castle king = color == Colors::WHITE ? Castles::WHITE_KING : Castles::BLACK_KING;
      constexpr Castle queen = color == Colors::BLACK ? Castles::WHITE_QUEEN : Castles::BLACK_QUEEN;

      Move move;
      move.set_castle(king);
      moves.push_back(move);
      move.set_castle(queen);
      moves.push_back(move);
      });
  }

  template <const Color color>
  auto get_moves(const GameState& game_state, Moves& moves) noexcept -> void {
    // Add moves in order of piece value
    MoveGeneration::get_king_moves<color>(game_state, moves);
    MoveGeneration::get_queen_moves<color>(game_state, moves);
    MoveGeneration::get_rook_moves<color>(game_state, moves);
    MoveGeneration::get_bishop_moves<color>(game_state, moves);
    MoveGeneration::get_knight_moves<color>(game_state, moves);
    MoveGeneration::get_pawn_moves<color>(game_state, moves);
  }

  // Threaten Squares Templates
  template <const int V, const int H>
  auto get_captures_in_direction(const Position& position, BitBoard bit_board) noexcept -> BitBoard {
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

      if (position.is_occupied(bit_board)) {
        break;
      }
    }

    return capturable_bit_board;
  }

  template <const Color color>
  auto get_pawn_capture_positions(const BitBoard bit_board) noexcept -> BitBoard {
    constexpr int64_t pawn_dir = color == Colors::WHITE ? 1 : -1;
    BitBoard capturable_bit_board = BitBoards::EMPTY;
    BitBoardUtils::for_each_bit_board(bit_board, [&capturable_bit_board, pawn_dir](BitBoard pawn_bit_board) {
      const BitBoard pawn_bit_board_left_capture = BitBoardUtils::is_piece_in_left_col(pawn_bit_board) * BitBoardUtils::shift_bit_board<1 * pawn_dir, -1>(pawn_bit_board);
      const BitBoard pawn_bit_board_right_capture = BitBoardUtils::is_piece_in_right_col(pawn_bit_board) * BitBoardUtils::shift_bit_board<1 * pawn_dir, 1>(pawn_bit_board);
      capturable_bit_board |= pawn_bit_board_left_capture | pawn_bit_board_right_capture;
      });
    return capturable_bit_board;
  }

  auto get_knight_capture_positions(const BitBoard knights_bit_board) noexcept -> BitBoard;
  auto get_cached_knight_capture_positions(const BitBoard knights_bit_board) noexcept -> BitBoard;

  template <const Color color>
  auto get_bishop_capture_positions(const Position& position) noexcept -> BitBoard {
    BitBoard capturable_bit_board = BitBoards::EMPTY;
    BitBoard bishops_bit_board = position.get_piece_color_bit_board(PieceCodes::BISHOP, color);
    BitBoardUtils::for_each_bit_board(bishops_bit_board, [&capturable_bit_board, &position](BitBoard bishop_bit_board) {
      capturable_bit_board |= MoveGeneration::get_captures_in_direction<1, 1>(position, bishop_bit_board);
      capturable_bit_board |= MoveGeneration::get_captures_in_direction<-1, 1>(position, bishop_bit_board);
      capturable_bit_board |= MoveGeneration::get_captures_in_direction<1, -1>(position, bishop_bit_board);
      capturable_bit_board |= MoveGeneration::get_captures_in_direction<-1, -1>(position, bishop_bit_board);
      });
    return capturable_bit_board;
  }

  template <const Color color>
  auto get_rook_capture_positions(const Position& position) noexcept  -> BitBoard {
    BitBoard capturable_bit_board = BitBoards::EMPTY;
    BitBoard rooks_bit_board = position.get_piece_color_bit_board(PieceCodes::ROOK, color);
    BitBoardUtils::for_each_bit_board(rooks_bit_board, [&capturable_bit_board, &position](BitBoard rook_bit_board) {
      capturable_bit_board |= MoveGeneration::get_captures_in_direction<1, 0>(position, rook_bit_board);
      capturable_bit_board |= MoveGeneration::get_captures_in_direction<-1, 0>(position, rook_bit_board);
      capturable_bit_board |= MoveGeneration::get_captures_in_direction<0, -1>(position, rook_bit_board);
      capturable_bit_board |= MoveGeneration::get_captures_in_direction<0, 1>(position, rook_bit_board);
      });
    return capturable_bit_board;
  }

  template <const Color color>
  auto get_queen_capture_positions(const Position& position) noexcept  -> BitBoard {
    BitBoard capturable_bit_board = BitBoards::EMPTY;
    BitBoard queens_bit_board = position.get_piece_color_bit_board(PieceCodes::QUEEN, color);
    BitBoardUtils::for_each_bit_board(queens_bit_board, [&capturable_bit_board, &position](BitBoard queen_bit_board) {
      // Diagonal
      capturable_bit_board |= MoveGeneration::get_captures_in_direction<1, 1>(position, queen_bit_board);
      capturable_bit_board |= MoveGeneration::get_captures_in_direction<-1, 1>(position, queen_bit_board);
      capturable_bit_board |= MoveGeneration::get_captures_in_direction<1, -1>(position, queen_bit_board);
      capturable_bit_board |= MoveGeneration::get_captures_in_direction<-1, -1>(position, queen_bit_board);

      // Vertical/Horizontal
      capturable_bit_board |= MoveGeneration::get_captures_in_direction<1, 0>(position, queen_bit_board);
      capturable_bit_board |= MoveGeneration::get_captures_in_direction<-1, 0>(position, queen_bit_board);
      capturable_bit_board |= MoveGeneration::get_captures_in_direction<0, -1>(position, queen_bit_board);
      capturable_bit_board |= MoveGeneration::get_captures_in_direction<0, 1>(position, queen_bit_board);
      });
    return capturable_bit_board;
  }

  auto get_king_capture_positions(const BitBoard kings_bit_board) noexcept -> BitBoard;
  auto get_cached_king_capture_positions(const BitBoard kings_bit_board) noexcept -> BitBoard;

  template <const Color color>
  auto get_capture_positions(const Position& position) noexcept -> BitBoard {
    BitBoard capturable_bit_board = BitBoards::EMPTY;
    capturable_bit_board |= MoveGeneration::get_pawn_capture_positions<color>(position.get_piece_color_bit_board<PieceCodes::PAWN, color>());
    capturable_bit_board |= MoveGeneration::get_cached_knight_capture_positions(position.get_piece_color_bit_board<PieceCodes::KNIGHT, color>());
    capturable_bit_board |= MoveGeneration::get_bishop_capture_positions<color>(position);
    capturable_bit_board |= MoveGeneration::get_rook_capture_positions<color>(position);
    capturable_bit_board |= MoveGeneration::get_queen_capture_positions<color>(position);
    capturable_bit_board |= MoveGeneration::get_cached_king_capture_positions(position.get_piece_color_bit_board<PieceCodes::KNIGHT, color>());
    return capturable_bit_board;
  }
} // namespace MoveGeneration