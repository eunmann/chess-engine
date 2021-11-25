#include "MoveSearch.hpp"

#include <algorithm>
#include <ranges>

#include "GameUtils.hpp"
#include "MoveGeneration.hpp"

auto MoveSearch::get_best_move(const GameState& game_state) -> Move {
  Moves moves;
  Color color_to_move = Colors::bool_to_color(game_state.white_to_move);
  if(color_to_move == Colors::WHITE) {
    MoveGeneration::get_moves<Colors::WHITE>(game_state, moves);
  } else {
    MoveGeneration::get_moves<Colors::BLACK>(game_state, moves);
  }

  int32_t best_heuristic = color_to_move == Colors::WHITE
    ? PieceValues::NEG_INFINITY
    : PieceValues::POS_INFINITY;

  auto is_move_legal = [&game_state](const Move move) {
    GameState check = game_state;
    check.apply_move(move);
    return check.is_legal;
  };
  Move best_move;

  for(auto move : moves) {
    if(is_move_legal(move)) {
      if(color_to_move == Colors::WHITE) {
        int32_t heuristic =
          MoveSearch::alpha_beta_pruning_search<Colors::BLACK>(
            game_state, 6, PieceValues::NEG_INFINITY,
            PieceValues::POS_INFINITY);
        if(best_heuristic < heuristic) {
          best_heuristic = heuristic;
          best_move = move;
        }
      } else {
        int32_t heuristic =
          MoveSearch::alpha_beta_pruning_search<Colors::WHITE>(
            game_state, 6, PieceValues::NEG_INFINITY,
            PieceValues::POS_INFINITY);
        if(best_heuristic > heuristic) {
          best_heuristic = heuristic;
          best_move = move;
        }
      }
    }
  }

  return best_move;
}

auto MoveSearch::get_position_heuristic(const GameState& game_state)
-> int32_t {
  // TODO(EMU): This function can be optimized
  int32_t heuristic = 0;

  std::array<PieceCode, 6> piece_codes = {
      PieceCodes::PAWN, PieceCodes::KNIGHT, PieceCodes::BISHOP,
      PieceCodes::ROOK, PieceCodes::QUEEN,  PieceCodes::KING};
  std::array<PieceCode, 6> piece_values = {
      PieceValues::PAWN, PieceValues::KNIGHT, PieceValues::BISHOP,
      PieceValues::ROOK, PieceValues::QUEEN,  PieceValues::KING};

  for(auto& piece_code : piece_codes) {
    GameUtils::for_each_bit_board(
      game_state.position.get_piece_bit_board(piece_code),
      [&heuristic, &piece_code, &game_state, &piece_values](auto bit_board) {
        if(game_state.position.is_white_occupied(bit_board)) {
          heuristic += piece_values[piece_code];
        } else {
          heuristic -= piece_values[piece_code];
        }
      });
  }

  // Put the King in check
  if(game_state.is_black_in_check()) {
    heuristic += PieceValues::PAWN / 2;
  }

  if(game_state.is_white_in_check()) {
    heuristic -= PieceValues::PAWN / 2;
  }

  // Bad check for Castling
  if(game_state.black_king_moved &&
     !(game_state.black_rook_A_moved || game_state.black_rook_H_moved)) {
    heuristic += PieceValues::PAWN / 2;
  }

  if(game_state.white_king_moved &&
     !(game_state.white_rook_A_moved || game_state.white_rook_H_moved)) {
    heuristic -= PieceValues::PAWN / 2;
  }

  return heuristic;
}
