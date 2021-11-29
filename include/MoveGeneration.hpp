#pragma once

#include "GameState.hpp"
#include "GameUtils.hpp"
#include "Move.hpp"
#include "Moves.hpp"
#include "assert.h"

namespace MoveGeneration {

  // Pseduo-Legal Moves Templates
  template <const int V, const int H, const Color color>
  auto get_moves_in_direction(const GameState& game_state, BitBoard bit_board, Moves& moves) noexcept -> void {
    Square source_square = GameUtils::bit_board_to_square(bit_board);
    for (int i = 0; i < 7; ++i) {
      if constexpr (V > 0) {
        if (GameUtils::is_piece_in_top_row(bit_board)) {
          break;
        }
      } else if constexpr (V < 0) {
        if (GameUtils::is_piece_in_bottom_row(bit_board)) {
          break;
        }
      }

      if constexpr (H > 0) {
        if (GameUtils::is_piece_in_right_col(bit_board)) {
          break;
        }
      } else if constexpr (H < 0) {
        if (GameUtils::is_piece_in_left_col(bit_board)) {
          break;
        }
      }

      bit_board = GameUtils::shift_bit_board<V, H>(bit_board);

      if (game_state.position.is_empty(bit_board)) {
        moves.push_back(Move(source_square, GameUtils::bit_board_to_square(bit_board)));
      } else if (game_state.position.is_color_occupied<color>(bit_board)) {
        break;
      } else {
        moves.push_back(Move(source_square, GameUtils::bit_board_to_square(bit_board)));
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

    GameUtils::for_each_bit_board(pawns_bit_board, [&game_state, &moves, pawn_dir](const BitBoard pawn_bit_board) {
      Square source_square = GameUtils::bit_board_to_square(pawn_bit_board);

      auto add_promotion_moves = [source_square, &moves](const BitBoard destination_bit_board) {
        Square destination_square = GameUtils::bit_board_to_square(destination_bit_board);
        for (int32_t i = PieceCodes::KNIGHT; i <= PieceCodes::QUEEN; i++) {
          Move move(source_square, destination_square);
          move.set_promotion(i);
          moves.push_back(move);
        }
      };

      // Up
      BitBoard next_pawn_bit_board = GameUtils::shift_bit_board<1 * pawn_dir, 0>(pawn_bit_board);
      if (game_state.position.is_empty(next_pawn_bit_board)) {
        // Promotion
        if (GameUtils::is_piece_in_top_row(next_pawn_bit_board)) {
          add_promotion_moves(next_pawn_bit_board);
        } else {
          moves.push_back(Move(source_square, GameUtils::bit_board_to_square(next_pawn_bit_board)));
        }

        // First Move Up 2
        if (GameUtils::is_piece_in_row(pawn_bit_board, starting_row)) {
          next_pawn_bit_board = GameUtils::shift_bit_board<2 * pawn_dir, 0>(pawn_bit_board);
          if (game_state.position.is_empty(next_pawn_bit_board)) {
            Move move(source_square, GameUtils::bit_board_to_square(next_pawn_bit_board));
            move.set_en_passant(GameUtils::get_col(next_pawn_bit_board));
            moves.push_back(move);
          }
        }
      }

      // Capture Left and Right
      BitBoard pawn_bit_board_left_capture = GameUtils::is_piece_in_left_col(pawn_bit_board) ? 0 : GameUtils::shift_bit_board<1 * pawn_dir, -1>(pawn_bit_board);
      BitBoard pawn_bit_board_right_capture = GameUtils::is_piece_in_right_col(pawn_bit_board) ? 0 : GameUtils::shift_bit_board<1 * pawn_dir, 1>(pawn_bit_board);

      if (game_state.position.is_color_occupied<opponent_color>(pawn_bit_board_left_capture)) {
        // Promotions
        if (GameUtils::is_piece_in_row(pawn_bit_board_left_capture, promotion_row)) {
          add_promotion_moves(pawn_bit_board_left_capture);
        } else {
          moves.push_back(Move(source_square, GameUtils::bit_board_to_square(pawn_bit_board_left_capture)));
        }
      }

      if (game_state.position.is_color_occupied<opponent_color>(pawn_bit_board_right_capture)) {
        // Promotions
        if (GameUtils::is_piece_in_row(pawn_bit_board_right_capture, promotion_row)) {
          add_promotion_moves(pawn_bit_board_right_capture);
        } else {
          moves.push_back(Move(source_square, GameUtils::bit_board_to_square(pawn_bit_board_right_capture)));
        }
      }

      // En Passant
      int32_t en_pawn_col = game_state.get_en_passant();
      int32_t pawn_col = GameUtils::get_col(pawn_bit_board);
      BitBoard source_bit_board = GameUtils::square_to_bit_board(source_square);
      constexpr int32_t source_row = color == Colors::WHITE ? 5 : 4;

      if (GameUtils::is_piece_in_row(source_bit_board, source_row)) {
        if ((en_pawn_col - 1) == pawn_col) {
          moves.push_back(Move(source_square, GameUtils::bit_board_to_square(pawn_bit_board_left_capture)));
        } else if ((en_pawn_col + 1) == pawn_col) {
          moves.push_back(Move(source_square, GameUtils::bit_board_to_square(pawn_bit_board_right_capture)));
        }
      }
      });
  }

  template <const Color color>
  auto get_knight_moves(const GameState& game_state, Moves& moves) noexcept -> void {
    BitBoard knights_bit_board = game_state.position.get_piece_color_bit_board<PieceCodes::KNIGHT, color>();

    GameUtils::for_each_set_square(knights_bit_board, [&game_state, &moves](const auto source_square) {
      GameUtils::for_each_set_square(PSEDUO_MOVES_KNIGHT[source_square], [source_square, &moves](auto dest_square) {
        moves.push_back(Move(source_square, dest_square));
        });
      });
  }

  template <const Color color>
  auto get_bishop_moves(const GameState& game_state, Moves& moves) noexcept -> void {
    BitBoard bishops_bit_board = game_state.position.get_piece_color_bit_board<PieceCodes::BISHOP, color>();

    GameUtils::for_each_bit_board(bishops_bit_board, [&game_state, &moves](const BitBoard bishop_bit_board) {
      MoveGeneration::get_moves_in_direction<1, 1, color>(game_state, bishop_bit_board, moves);
      MoveGeneration::get_moves_in_direction<-1, 1, color>(game_state, bishop_bit_board, moves);
      MoveGeneration::get_moves_in_direction<1, -1, color>(game_state, bishop_bit_board, moves);
      MoveGeneration::get_moves_in_direction<-1, -1, color>(game_state, bishop_bit_board, moves);
      });
  }

  template <const Color color>
  auto get_rook_moves(const GameState& game_state, Moves& moves) noexcept -> void {
    BitBoard rooks_bit_board = game_state.position.get_piece_color_bit_board<PieceCodes::ROOK, color>();

    GameUtils::for_each_bit_board(
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
    GameUtils::for_each_bit_board(queens_bit_board, [&game_state, &moves](BitBoard queen_bit_board) {
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

    GameUtils::for_each_bit_board(kings_bit_board, [&game_state, &moves](BitBoard king_bit_board) {
      Square source_square = GameUtils::bit_board_to_square(king_bit_board);

      GameUtils::for_each_set_square(PSEDUO_MOVES_KING[source_square], [source_square, &moves](auto dest_square) {
        moves.push_back(Move(source_square, dest_square));
        });

      // Castling
      if (!game_state.has_king_moved<color>() &&
        !game_state.is_color_in_check<color>()) {
        constexpr Color opponent_color = color == Colors::WHITE ? Colors::BLACK : Colors::WHITE;
        constexpr BitBoard queen_castle = color == Colors::WHITE ? BitBoards::WHITE_QUEEN_CASTLE : BitBoards::BLACK_QUEEN_CASTLE;
        // Queen Side
        if (!game_state.has_rook_A_moved<color>() &&
          game_state.position.is_empty(queen_castle) &&
          !game_state.position.is_threaten<opponent_color>(queen_castle)) {
          Move move;
          constexpr Castle queen_side = color == Colors::WHITE ? Castles::WHITE_QUEEN : Castles::BLACK_QUEEN;

          move.set_castle(queen_side);
          moves.push_back(move);
        }

        constexpr BitBoard king_castle = color == Colors::WHITE ? BitBoards::WHITE_KING_CASTLE : BitBoards::BLACK_KING_CASTLE;
        // King Side
        if (!game_state.has_rook_H_moved<color>() &&
          game_state.position.is_empty(king_castle) &&
          !game_state.position.is_threaten<opponent_color>(king_castle)) {
          Move move;
          constexpr Castle king_side =
            color == Colors::WHITE ? Castles::WHITE_KING : Castles::BLACK_KING;
          move.set_castle(king_side);
          moves.push_back(move);
        }
      }
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
        if (GameUtils::is_piece_in_top_row(bit_board)) {
          break;
        }
      } else if constexpr (V < 0) {
        if (GameUtils::is_piece_in_bottom_row(bit_board)) {
          break;
        }
      }

      if constexpr (H > 0) {
        if (GameUtils::is_piece_in_right_col(bit_board)) {
          break;
        }
      } else if constexpr (H < 0) {
        if (GameUtils::is_piece_in_left_col(bit_board)) {
          break;
        }
      }

      bit_board = GameUtils::shift_bit_board<V, H>(bit_board);
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
    GameUtils::for_each_bit_board(bit_board, [&capturable_bit_board, pawn_dir](BitBoard pawn_bit_board) {
      // Capture Left and Right
      const BitBoard pawn_bit_board_left_capture = GameUtils::is_piece_in_left_col(pawn_bit_board) * GameUtils::shift_bit_board<1 * pawn_dir, -1>(pawn_bit_board);
      const BitBoard pawn_bit_board_right_capture = GameUtils::is_piece_in_right_col(pawn_bit_board) * GameUtils::shift_bit_board<1 * pawn_dir, 1>(pawn_bit_board);
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
    GameUtils::for_each_bit_board(bishops_bit_board, [&capturable_bit_board, &position](BitBoard bishop_bit_board) {
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
    GameUtils::for_each_bit_board(rooks_bit_board, [&capturable_bit_board, &position](BitBoard rook_bit_board) {
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
    GameUtils::for_each_bit_board(queens_bit_board, [&capturable_bit_board, &position](BitBoard queen_bit_board) {
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