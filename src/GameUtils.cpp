#include "GameUtils.hpp"

#include <assert.h>
#include <stdio.h>
#include <intrin.h>

#include <algorithm>
#include <bit>
#include <iostream>
#include <string>

#include "Definitions.hpp"
#include "Move.hpp"
#include "MoveGeneration.hpp"
#include "Moves.hpp"


auto GameUtils::square_to_bit_board(const Square square) noexcept -> BitBoard {
  return 0b1ULL << square;
}

auto GameUtils::print_position(const BitBoard bit_board) noexcept -> void {
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

auto GameUtils::shift_bit_board(const BitBoard bit_board,
  const int32_t vertical,
  const int32_t horizontal) noexcept -> BitBoard {
  assert(vertical > -8 && vertical < 8);
  assert(horizontal > -8 && horizontal < 8);

  int32_t shift = vertical * 8 + horizontal;
  if (shift >= 0) {
    return bit_board << shift;
  } else {
    return bit_board >> (shift * -1);
  }
}

auto GameUtils::get_row(const BitBoard bit_board) noexcept -> int32_t {
  BitBoard row_mask = BitBoards::ROW_1;

  for (int i = 0; i < 8; ++i) {
    if ((bit_board & row_mask) != 0) {
      return i;
    }
    row_mask = row_mask << 8;
  }

  return -1;
}

auto GameUtils::get_col(const BitBoard bit_board) noexcept -> int32_t {
  BitBoard col_mask = BitBoards::COL_A;

  for (int i = 0; i < 8; ++i) {
    if ((bit_board & col_mask) != 0) {
      return i;
    }
    col_mask = col_mask << 1;
  }

  return -1;
}

auto GameUtils::get_row_col(const BitBoard bit_board, int32_t& row,
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

auto GameUtils::get_tile_name(const BitBoard bit_board) noexcept -> std::string {
  int32_t row, col;
  GameUtils::get_row_col(bit_board, row, col);

  std::string name = "";
  name += (char)(col + 'a');
  name += (char)(row + '1');

  return name;
}

<<<<<<< HEAD
void GameUtils::get_pawn_moves(const GameState &game_state, Moves &moves) {
    const bool is_white_piece = piece_index < PIECES_PER_PLAYER;
    const int64_t pawn_dir = is_white_piece ? 1 : -1;
    const BitBoard pawn_position = game_state.position.get_piece_bit_board(piece_index);

    if (pawn_position) {
        /* Up */
        BitBoard next_pawn_position = GameUtils::move(pawn_position, 1 * pawn_dir, 0);
        if (GameUtils::is_empty(game_state.position, next_pawn_position) &&
            GameUtils::is_valid(game_state, next_pawn_position, piece_index)) {
            if ((is_white_piece && GameUtils::is_piece_in_top_row(next_pawn_position)) ||
                (!is_white_piece && GameUtils::is_piece_in_bottom_row(next_pawn_position))) {
                for (int32_t i = PieceCodes::KNIGHT; i <= PieceCodes::QUEEN; ++i) {
                    GameState &move = moves.back_ref();
                    move = game_state;
                    move.position.get_piece_bit_board(piece_index) = i;
                    GameUtils::apply_move(move, next_pawn_position, piece_index);
                }
            } else {
                GameState &move = moves.back_ref();
                move = game_state;
                GameUtils::apply_move(move, next_pawn_position, piece_index);
            }

            /* Up 2 first-move */
            if ((is_white_piece && GameUtils::is_piece_in_row(pawn_position, 1)) ||
                (!is_white_piece && GameUtils::is_piece_in_row(pawn_position, 6))) {
                next_pawn_position = GameUtils::move(pawn_position, 2 * pawn_dir, 0);
                if (GameUtils::is_empty(game_state.position, next_pawn_position) &&
                    GameUtils::is_valid(game_state, next_pawn_position, piece_index)) {
                    GameState &move = moves.back_ref();
                    move = game_state;
                    GameUtils::apply_move(move, next_pawn_position, piece_index);
                    move.pawn_ep = piece_index;
                }
            }
        }

        /* Capture Left and Right */
        BitBoard pawn_position_left_capture = GameUtils::is_piece_in_left_col(pawn_position) ? 0 : GameUtils::move(pawn_position, 1 * pawn_dir, -1);
        bool left_capture_valid = GameUtils::is_valid(game_state, pawn_position_left_capture, piece_index);
        BitBoard pawn_position_right_capture = GameUtils::is_piece_in_right_col(pawn_position) ? 0 : GameUtils::move(pawn_position, 1 * pawn_dir, 1);
        bool right_capture_valid = GameUtils::is_valid(game_state, pawn_position_right_capture, piece_index);

        if (left_capture_valid &&
            ((is_white_piece && ((game_state.position.get_black_bit_board() & pawn_position_left_capture) != 0)) ||
             (!is_white_piece && ((game_state.position.get_white_bit_board() & pawn_position_left_capture) != 0)))) {
            if ((is_white_piece && GameUtils::is_piece_in_top_row(pawn_position_left_capture)) ||
                (!is_white_piece && GameUtils::is_piece_in_bottom_row(pawn_position_left_capture))) {
                for (int32_t i = PieceCodes::KNIGHT; i <= PieceCodes::QUEEN; ++i) {
                    GameState &move = moves.back_ref();
                    move = game_state;
                    move.position.get_piece_bit_board(piece_index) = i;
                    GameUtils::apply_move(move, pawn_position_left_capture, piece_index);
                }
            } else {
                GameState &move = moves.back_ref();
                move = game_state;
                GameUtils::apply_move(move, pawn_position_left_capture, piece_index);
            }
        }

        if (right_capture_valid &&
            ((is_white_piece && ((game_state.position.get_black_bit_board() & pawn_position_right_capture) != 0)) ||
             (!is_white_piece && ((game_state.position.get_white_bit_board() & pawn_position_right_capture) != 0)))) {
            if ((is_white_piece && GameUtils::is_piece_in_top_row(pawn_position_right_capture)) ||
                (!is_white_piece && GameUtils::is_piece_in_bottom_row(pawn_position_right_capture))) {
                for (int32_t i = PieceCodes::KNIGHT; i <= PieceCodes::QUEEN; ++i) {
                    GameState &move = moves.back_ref();
                    move = game_state;
                    move.position.get_piece_bit_board(piece_index) = i;
                    GameUtils::apply_move(move, pawn_position_right_capture, piece_index);
                }
            } else {
                GameState &move = moves.back_ref();
                move = game_state;
                GameUtils::apply_move(move, pawn_position_right_capture, piece_index);
            }
        }

        /* En Passant */
        int32_t row, col;
        GameUtils::get_row_col(pawn_position, row, col);
        const int32_t left_ep = is_white_piece ? col - 1 + PIECES_PER_PLAYER : col - 1 - PIECES_PER_PLAYER;
        const int32_t right_ep = is_white_piece ? col + 1 + PIECES_PER_PLAYER : col + 1 - PIECES_PER_PLAYER;
        const int32_t en_row = is_white_piece ? 4 : 3;

        if (game_state.pawn_ep == left_ep && GameUtils::is_piece_in_row(pawn_position, en_row)) {
            GameState &move = moves.back_ref();
            move = game_state;
            GameUtils::apply_move(move, GameUtils::move(pawn_position_left_capture, -1 * pawn_dir, 0), piece_index);
            move.position.get_piece_bit_board(piece_index) = pawn_position_left_capture;
        }

        if (game_state.pawn_ep == right_ep && GameUtils::is_piece_in_row(pawn_position, en_row)) {
            GameState &move = moves.back_ref();
            move = game_state;
            GameUtils::apply_move(move, GameUtils::move(pawn_position_right_capture, -1 * pawn_dir, 0), piece_index);
            move.position.get_piece_bit_board(piece_index) = pawn_position_right_capture;
        }
    }
}

void GameUtils::get_knight_moves(const GameState &game_state, Moves &moves) {
    BitBoard knight_position = game_state.position.get_piece_bit_board(piece_index);

    if (knight_position) {
        if (!(GameUtils::is_piece_in_top_2_row(knight_position) || GameUtils::is_piece_in_left_col(knight_position))) {
            BitBoard next_knight_position = GameUtils::move(knight_position, 2, -1);
            if (GameUtils::is_valid(game_state, next_knight_position, piece_index)) {
                GameState &move = moves.back_ref();
                move = game_state;
                GameUtils::apply_move(move, next_knight_position, piece_index);
            }
        }
        if (!(GameUtils::is_piece_in_top_row(knight_position) || GameUtils::is_piece_in_left_2_col(knight_position))) {
            BitBoard next_knight_position = GameUtils::move(knight_position, 1, -2);
            if (GameUtils::is_valid(game_state, next_knight_position, piece_index)) {
                GameState &move = moves.back_ref();
                move = game_state;
                GameUtils::apply_move(move, next_knight_position, piece_index);
            }
        }

        if (!(GameUtils::is_piece_in_bottom_row(knight_position) || GameUtils::is_piece_in_left_2_col(knight_position))) {
            BitBoard next_knight_position = GameUtils::move(knight_position, -1, -2);
            if (GameUtils::is_valid(game_state, next_knight_position, piece_index)) {
                GameState &move = moves.back_ref();
                move = game_state;
                GameUtils::apply_move(move, next_knight_position, piece_index);
            }
        }

        if (!(GameUtils::is_piece_in_bottom_2_row(knight_position) || GameUtils::is_piece_in_left_col(knight_position))) {
            BitBoard next_knight_position = GameUtils::move(knight_position, -2, -1);
            if (GameUtils::is_valid(game_state, next_knight_position, piece_index)) {
                GameState &move = moves.back_ref();
                move = game_state;
                GameUtils::apply_move(move, next_knight_position, piece_index);
            }
        }

        if (!(GameUtils::is_piece_in_bottom_2_row(knight_position) || GameUtils::is_piece_in_right_col(knight_position))) {
            BitBoard next_knight_position = GameUtils::move(knight_position, -2, 1);
            if (GameUtils::is_valid(game_state, next_knight_position, piece_index)) {
                GameState &move = moves.back_ref();
                move = game_state;
                GameUtils::apply_move(move, next_knight_position, piece_index);
            }
        }

        if (!(GameUtils::is_piece_in_bottom_row(knight_position) || GameUtils::is_piece_in_right_2_col(knight_position))) {
            BitBoard next_knight_position = GameUtils::move(knight_position, -1, 2);
            if (GameUtils::is_valid(game_state, next_knight_position, piece_index)) {
                GameState &move = moves.back_ref();
                move = game_state;
                GameUtils::apply_move(move, next_knight_position, piece_index);
            }
        }

        if (!(GameUtils::is_piece_in_top_row(knight_position) || GameUtils::is_piece_in_right_2_col(knight_position))) {
            BitBoard next_knight_position = GameUtils::move(knight_position, 1, 2);
            if (GameUtils::is_valid(game_state, next_knight_position, piece_index)) {
                GameState &move = moves.back_ref();
                move = game_state;
                GameUtils::apply_move(move, next_knight_position, piece_index);
            }
        }

        if (!(GameUtils::is_piece_in_top_2_row(knight_position) || GameUtils::is_piece_in_right_col(knight_position))) {
            BitBoard next_knight_position = GameUtils::move(knight_position, 2, 1);
            if (GameUtils::is_valid(game_state, next_knight_position, piece_index)) {
                GameState &move = moves.back_ref();
                move = game_state;
                GameUtils::apply_move(move, next_knight_position, piece_index);
            }
        }
    }
}

void GameUtils::get_bishop_moves(const GameState &game_state, Moves &moves) {
    if (game_state.position.get_piece_bit_board(piece_index)) {
        GameUtils::get_moves_in_direction(game_state, piece_index, 1, 1, moves);
        GameUtils::get_moves_in_direction(game_state, piece_index, -1, 1, moves);
        GameUtils::get_moves_in_direction(game_state, piece_index, 1, -1, moves);
        GameUtils::get_moves_in_direction(game_state, piece_index, -1, -1, moves);
    }
}

void GameUtils::get_rook_moves(const GameState &game_state, Moves &moves) {
    if (game_state.position.get_piece_bit_board(piece_index)) {
        GameUtils::get_moves_in_direction(game_state, piece_index, 1, 0, moves);
        GameUtils::get_moves_in_direction(game_state, piece_index, -1, 0, moves);
        GameUtils::get_moves_in_direction(game_state, piece_index, 0, -1, moves);
        GameUtils::get_moves_in_direction(game_state, piece_index, 0, 1, moves);
    }
}

void GameUtils::get_queen_moves(const GameState &game_state, Moves &moves) {
    GameUtils::get_bishop_moves(game_state, piece_index, moves);
    GameUtils::get_rook_moves(game_state, piece_index, moves);
}

void GameUtils::get_king_moves(const GameState &game_state, Moves &moves) {
    BitBoard king_position = game_state.position.get_piece_bit_board(piece_index);
    if (king_position) {
        /* Up */
        if (!GameUtils::is_piece_in_top_row(king_position)) {
            BitBoard next_king_position = GameUtils::move(king_position, 1, 0);
            if (GameUtils::is_valid(game_state, next_king_position, piece_index)) {
                GameState &move = moves.back_ref();
                move = game_state;
                GameUtils::apply_move(move, next_king_position, piece_index);
            }
        }

        /* Down */
        if (!GameUtils::is_piece_in_bottom_row(king_position)) {
            BitBoard next_king_position = GameUtils::move(king_position, -1, 0);
            if (GameUtils::is_valid(game_state, next_king_position, piece_index)) {
                GameState &move = moves.back_ref();
                move = game_state;
                GameUtils::apply_move(move, next_king_position, piece_index);
            }
        }

        /* Left */
        if (!GameUtils::is_piece_in_left_col(king_position)) {
            BitBoard next_king_position = GameUtils::move(king_position, 0, -1);
            if (GameUtils::is_valid(game_state, next_king_position, piece_index)) {
                GameState &move = moves.back_ref();
                move = game_state;
                GameUtils::apply_move(move, next_king_position, piece_index);
            }
        }

        /* Right */
        if (!GameUtils::is_piece_in_right_col(king_position)) {
            BitBoard next_king_position = GameUtils::move(king_position, 0, 1);
            if (GameUtils::is_valid(game_state, next_king_position, piece_index)) {
                GameState &move = moves.back_ref();
                move = game_state;
                GameUtils::apply_move(move, next_king_position, piece_index);
            }
        }

        /* Up-Right */
        if (!(GameUtils::is_piece_in_top_row(king_position) || GameUtils::is_piece_in_right_col(king_position))) {
            BitBoard next_king_position = GameUtils::move(king_position, 1, 1);
            if (GameUtils::is_valid(game_state, next_king_position, piece_index)) {
                GameState &move = moves.back_ref();
                move = game_state;
                GameUtils::apply_move(move, next_king_position, piece_index);
            }
        }

        /* Up-Left */
        if (!(GameUtils::is_piece_in_top_row(king_position) || GameUtils::is_piece_in_left_col(king_position))) {
            BitBoard next_king_position = GameUtils::move(king_position, 1, -1);
            if (GameUtils::is_valid(game_state, next_king_position, piece_index)) {
                GameState &move = moves.back_ref();
                move = game_state;
                GameUtils::apply_move(move, next_king_position, piece_index);
            }
        }

        /* Down-Right */
        if (!(GameUtils::is_piece_in_bottom_row(king_position) || GameUtils::is_piece_in_right_col(king_position))) {
            BitBoard next_king_position = GameUtils::move(king_position, -1, 1);
            if (GameUtils::is_valid(game_state, next_king_position, piece_index)) {
                GameState &move = moves.back_ref();
                move = game_state;
                GameUtils::apply_move(move, next_king_position, piece_index);
            }
        }

        /* Down-Left */
        if (!(GameUtils::is_piece_in_bottom_row(king_position) || GameUtils::is_piece_in_left_col(king_position))) {
            BitBoard next_king_position = GameUtils::move(king_position, -1, -1);
            if (GameUtils::is_valid(game_state, next_king_position, piece_index)) {
                GameState &move = moves.back_ref();
                move = game_state;
                GameUtils::apply_move(move, next_king_position, piece_index);
            }
        }

        /* Castling */
        bool is_white_piece = piece_index < PIECES_PER_PLAYER;
        if ((is_white_piece && !game_state.white_king_moved && !game_state.white_king_in_check) ||
            (!is_white_piece && !game_state.black_king_moved && !game_state.black_king_in_check)) {
            /* Queen Side */
            if ((is_white_piece && !game_state.white_rook_1_moved) || (!is_white_piece && !game_state.black_rook_1_moved)) {
                BitBoard next_king_position = GameUtils::move(king_position, 0, -2);
                BitBoard next_rook_position = GameUtils::move(next_king_position, 0, 1);
                BitBoard empty_position = GameUtils::move(next_king_position, 0, -1);

                BitBoard castle_threw_positions = next_rook_position | next_king_position;
                BitBoard attack_positions = GameUtils::get_capture_positions(game_state, !game_state.white_to_move);

                if (((castle_threw_positions & attack_positions) == 0) &&
                    GameUtils::is_empty(game_state.position, castle_threw_positions | empty_position)) {
                    GameState &move = moves.back_ref();
                    move = game_state;
                    int32_t rook_piece_index = is_white_piece ? 12 : 12 + PIECES_PER_PLAYER;

                    if (is_white_piece) {
                        move.position.get_white_bit_board() ^= (move.position.get_piece_bit_board(rook_piece_index) | next_rook_position);
                    } else {
                        move.position.get_black_bit_board() ^= (move.position.get_piece_bit_board(rook_piece_index) | next_rook_position);
                    }
                    move.position.get_piece_bit_board(rook_piece_index) = next_rook_position;

                    if (is_white_piece) {
                        move.white_rook_1_moved = true;
                    } else {
                        move.black_rook_1_moved = true;
                    }

                    GameUtils::apply_move(move, next_king_position, piece_index);
                }
            }

            /* King Side */
            if ((is_white_piece && !game_state.white_rook_2_moved) || (!is_white_piece && !game_state.black_rook_2_moved)) {
                BitBoard next_king_position = GameUtils::move(king_position, 0, 2);
                BitBoard next_rook_position = GameUtils::move(next_king_position, 0, -1);

                BitBoard castle_threw_positions = next_rook_position | next_king_position;
                BitBoard attack_positions = GameUtils::get_capture_positions(game_state, !game_state.white_to_move);

                if (((castle_threw_positions & attack_positions) == 0) &&
                    GameUtils::is_empty(game_state.position, castle_threw_positions)) {
                    GameState &move = moves.back_ref();
                    move = game_state;
                    int32_t rook_piece_index = is_white_piece ? 13 : 13 + PIECES_PER_PLAYER;

                    if (is_white_piece) {
                        move.position.get_white_bit_board() ^= (move.position.get_piece_bit_board(rook_piece_index) | next_rook_position);
                    } else {
                        move.position.get_black_bit_board() ^= (move.position.get_piece_bit_board(rook_piece_index) | next_rook_position);
                    }
                    move.position.get_piece_bit_board(rook_piece_index) = next_rook_position;
                    if (is_white_piece) {
                        move.white_rook_2_moved = true;
                    } else {
                        move.black_rook_2_moved = true;
                    }

                    GameUtils::apply_move(move, next_king_position, piece_index);
                }
            }
        }
    }
=======
auto GameUtils::do_bit_boards_overlap(const BitBoard bit_board_1, const BitBoard bit_board_2) noexcept -> bool {
  return (bit_board_1 & bit_board_2) != 0;
}

auto GameUtils::is_piece_in_row(const BitBoard bit_board, const int32_t row) noexcept -> bool {
  const BitBoard row_mask = BitBoards::ROW_1 << (row * 8);
  return GameUtils::do_bit_boards_overlap(row_mask, bit_board);
}

auto GameUtils::is_piece_in_col(const BitBoard bit_board, const int32_t col) noexcept -> bool {
  const BitBoard col_mask = BitBoards::COL_A << col;
  return GameUtils::do_bit_boards_overlap(col_mask, bit_board);
}

auto GameUtils::is_piece_in_top_row(const BitBoard bit_board) noexcept -> bool {
  return GameUtils::do_bit_boards_overlap(BitBoards::ROW_8, bit_board);
}

auto GameUtils::is_piece_in_top_2_row(const BitBoard bit_board) noexcept -> bool {
  return GameUtils::do_bit_boards_overlap(BitBoards::ROW_8 | BitBoards::ROW_7,
    bit_board);
}

auto GameUtils::is_piece_in_bottom_row(const BitBoard bit_board) noexcept -> bool {
  return GameUtils::do_bit_boards_overlap(BitBoards::ROW_1, bit_board);
>>>>>>> good
}

auto GameUtils::is_piece_in_bottom_2_row(const BitBoard bit_board) noexcept -> bool {
  return GameUtils::do_bit_boards_overlap(BitBoards::ROW_1 | BitBoards::ROW_2,
    bit_board);
}

auto GameUtils::is_piece_in_left_col(const BitBoard bit_board) noexcept -> bool {
  return GameUtils::do_bit_boards_overlap(BitBoards::COL_A, bit_board);
}

auto GameUtils::is_piece_in_left_2_col(const BitBoard bit_board) noexcept -> bool {
  return GameUtils::do_bit_boards_overlap(BitBoards::COL_A | BitBoards::COL_B,
    bit_board);
}

auto GameUtils::is_piece_in_right_col(const BitBoard bit_board) noexcept -> bool {
  return GameUtils::do_bit_boards_overlap(BitBoards::COL_H, bit_board);
}

auto GameUtils::is_piece_in_right_2_col(const BitBoard bit_board) noexcept -> bool {
  return GameUtils::do_bit_boards_overlap(BitBoards::COL_H | BitBoards::COL_G,
    bit_board);
}

auto GameUtils::perform_user_move(GameState& game_state) noexcept -> bool {
  bool need_input = true;

  while (need_input) {
    printf("Input move: ");
    auto input = GameUtils::get_user_input();

    if (input.size() < 4) {
      printf("Invalid input.\n");
      continue;
    }

    if (input == "exit") {
      return false;
    }

    Moves moves;
    Color color_to_move = Colors::bool_to_color(game_state.is_white_to_move());
    if (color_to_move == Colors::WHITE) {
      MoveGeneration::get_moves<Colors::WHITE>(game_state, moves);
    } else {
      MoveGeneration::get_moves<Colors::BLACK>(game_state, moves);
    }
    printf("Found %llu psuedo-legal moves for %s\n", moves.size(),
      color_to_move == Colors::WHITE ? "white" : "black");

    auto is_move_legal = [&game_state](const Move move) {
      GameState check = game_state;
      check.apply_move(move);
      return check.is_legal();
    };

    Moves legal_moves;
    for (auto move : moves) {
      if (is_move_legal(move)) {
        legal_moves.push_back(move);
      } else {
        printf("Illegal Move: %s\n", move.to_string().c_str());
      }
    }

    printf("Found %llu legal moves for %s\n", legal_moves.size(),
      color_to_move == Colors::WHITE ? "white" : "black");

    for (auto legal_move : legal_moves) {
      printf("Legal Move: %s\n", legal_move.to_string().c_str());
      if (legal_move.to_string() == input) {
        game_state.apply_move(legal_move);
        need_input = false;
        break;
      }
    }

    if (need_input) {
      printf("Invalid input.\n");
    } else {
      break;
    }
  }

  return true;
}

auto GameUtils::get_user_input() noexcept -> std::string {
  std::string input;
  std::getline(std::cin, input);
  return input;
}

auto GameUtils::process_user_move(GameState& game_state, const std::string& move_str) noexcept -> bool {

  Moves moves;
  Color color_to_move = Colors::bool_to_color(game_state.is_white_to_move());
  if (color_to_move == Colors::WHITE) {
    MoveGeneration::get_moves<Colors::WHITE>(game_state, moves);
  } else {
    MoveGeneration::get_moves<Colors::BLACK>(game_state, moves);
  }

  auto is_move_legal = [&game_state](const Move move) {
    GameState check = game_state;
    check.apply_move(move);
    return check.is_legal();
  };

  for (auto move : moves) {
    if (is_move_legal(move) && move.to_string() == move_str) {
      game_state.apply_move(move);
      return true;
    }
  }

  return false;
}

<<<<<<< HEAD
void GameUtils::get_best_move(ThreadState &thread_state) {
    Moves moves;
    GameUtils::get_moves(thread_state.game_state, moves);

    int32_t best_heuristics[omp_get_max_threads()];
    ThreadState thread_states[omp_get_max_threads()];

    int32_t heuristic_init = thread_state.game_state.white_to_move ? PieceValues::NEG_INFINITY : PieceValues::POS_INFINITY;

    for (int i = 0; i < omp_get_max_threads(); ++i) {
        best_heuristics[i] = heuristic_init;
        thread_states[i] = thread_state;
    }

#pragma omp parallel for schedule(dynamic, 1)
    for (size_t i = 0; i < moves.size(); ++i) {
        GameState &move = moves[i];
        ThreadState &ts = thread_states[omp_get_thread_num()];

        int32_t heuristic = GameUtils::alpha_beta_pruning_search(ts, move, 6, PieceValues::NEG_INFINITY, PieceValues::POS_INFINITY, !ts.game_state.white_to_move);
        if ((ts.game_state.white_to_move && best_heuristics[omp_get_thread_num()] < heuristic) ||
            (!ts.game_state.white_to_move && best_heuristics[omp_get_thread_num()] > heuristic)) {
            best_heuristics[omp_get_thread_num()] = heuristic;
            ts.best_move = move;
        }
    }

    int32_t best_heuristic = heuristic_init;

    for (int i = 0; i < omp_get_max_threads(); ++i) {
        if ((thread_state.game_state.white_to_move && best_heuristic <= best_heuristics[i]) ||
            (!thread_state.game_state.white_to_move && best_heuristic >= best_heuristics[i])) {
            best_heuristic = best_heuristics[i];
            thread_state.best_move = thread_states[i].best_move;
        }
    }
}

int32_t GameUtils::alpha_beta_pruning_search(const ThreadState &thread_state, const GameState &game_state, int32_t ply_depth, int32_t alpha, int32_t beta, bool max_white) {
    if (!thread_state.should_search) {
        /* TODO(EMU): What should this value be? */
        return 0;
    }

    if (ply_depth == 0) {
        return GameUtils::get_position_heuristic(game_state);
    }

    Moves moves;
    GameUtils::get_moves(game_state, moves);

    if (max_white) {
        int32_t best_heuristic = PieceValues::NEG_INFINITY;
        for (size_t i = 0; i < moves.size(); ++i) {
            GameState &move = moves[i];
            best_heuristic = std::max(best_heuristic, GameUtils::alpha_beta_pruning_search(thread_state, move, ply_depth - 1, alpha, beta, false));
            if (best_heuristic >= beta) {
                break;
            }
            alpha = std::max(alpha, best_heuristic);
        }
        return best_heuristic;
    } else {
        int32_t best_heuristic = PieceValues::POS_INFINITY;
        for (size_t i = 0; i < moves.size(); ++i) {
            GameState &move = moves[i];
            best_heuristic = std::min(best_heuristic, GameUtils::alpha_beta_pruning_search(thread_state, move, ply_depth - 1, alpha, beta, true));
            if (best_heuristic <= alpha) {
                break;
            }
            beta = std::min(beta, best_heuristic);
        }
        return best_heuristic;
    }
=======
auto GameUtils::square_name_to_square(const std::string& square_name) noexcept -> Square {
  // Check string length, min 2 characters required for a move
  if (square_name.size() < 2) {
    return 0;
  }

  int32_t col_index = square_name[0] - 'a';
  int32_t row_index = square_name[1] - '1';

  auto invalid_input_range = [](int32_t num) { return (num < 0 || num > 7); };

  if (invalid_input_range(col_index) || invalid_input_range(row_index)) {
    return 0;
  }

  return row_index * 8 + col_index;
>>>>>>> good
}

auto GameUtils::move_str_to_move(const std::string& move_str) noexcept -> Move {
  // Check string length, min 4 characters required for a move
  if (move_str.size() < 4) {
    // TODO(EMU): This might cause problems. It might think this is a valid move
    return Move();
  }

  auto source_square = GameUtils::square_name_to_square(move_str);
  auto destintion_square =
    GameUtils::square_name_to_square(move_str.substr(2, 2));

  Move move(source_square, destintion_square);

  // Check if moves promotes a pawn
  if (move_str.size() == 5) {
    PieceCode promotion_piece_code = PieceCodes::NUM;
    switch (move_str[4]) {
      case 'n':
      {
        promotion_piece_code = PieceCodes::KNIGHT;
        break;
      }
      case 'b':
      {
        promotion_piece_code = PieceCodes::BISHOP;
        break;
      }
      case 'r':
      {
        promotion_piece_code = PieceCodes::ROOK;
        break;
      }
      case 'q':
      {
        promotion_piece_code = PieceCodes::QUEEN;
        break;
      }
      default:
      {
        break;
      }
    }
    move.set_promotion(promotion_piece_code);
  }

  return move;
}

auto GameUtils::bit_board_to_square(const BitBoard bit_board) noexcept -> Square {

  if (bit_board == BitBoards::EMPTY) {
    return -1;
  }

  unsigned long index = 0;
  _BitScanForward64(&index, bit_board);
  return index;
}

auto GameUtils::for_each_set_square(const BitBoard bit_board, const std::function<void(Square square)>& func) noexcept -> void {
  BitBoard temp_bit_board = bit_board;
  while (true) {
    const Square square = GameUtils::bit_board_to_square(temp_bit_board);
    if (square == -1) {
      break;
    }
    func(square);
    temp_bit_board &= ~GameUtils::square_to_bit_board(square);
  };
}

auto GameUtils::for_each_bit_board(const BitBoard bit_board, const std::function<void(BitBoard bit_board)>& func) noexcept -> void {
  BitBoard temp_bit_board = bit_board;
  while (true) {
    const Square square = GameUtils::bit_board_to_square(temp_bit_board);
    if (square == -1) {
      break;
    }
    const BitBoard single_bit_board = GameUtils::square_to_bit_board(square);
    func(single_bit_board);
    temp_bit_board &= ~single_bit_board;
  };
}
