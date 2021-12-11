#include "MoveSearch.hpp"

#include <inttypes.h>

#include <algorithm>
#include <ranges>

#include "GameUtils.hpp"
#include "MoveGeneration.hpp"
#include "Timer.hpp"

auto MoveSearch::get_best_move(const GameState& game_state) noexcept -> Move {

  Timer timer;
  Moves moves;
  Color color_to_move = Colors::bool_to_color(game_state.is_white_to_move());
  if (color_to_move == Colors::WHITE) {
    MoveGeneration::get_moves<Colors::WHITE>(game_state, moves);
  } else {
    MoveGeneration::get_moves<Colors::BLACK>(game_state, moves);
  }

  int32_t best_heuristic = color_to_move == Colors::WHITE ? PieceValues::NEG_INFINITY : PieceValues::POS_INFINITY;
  Move best_move;
  constexpr int32_t max_search_depth = 4;

  counter = moves.size();
  leaf_nodes_counter = 0;
  times_pruned = 0;
  for (auto& move : moves) {
    GameState check = game_state;
    if (color_to_move == Colors::WHITE) {
      check.apply_move<Colors::WHITE>(move);
    } else {
      check.apply_move<Colors::BLACK>(move);
    }
    if (check.is_legal()) {

      if (color_to_move == Colors::WHITE) {
        int32_t heuristic = MoveSearch::alpha_beta_pruning_search<Colors::BLACK>(check, max_search_depth, PieceValues::NEG_INFINITY, PieceValues::POS_INFINITY);
        if (best_heuristic < heuristic) {
          best_heuristic = heuristic;
          best_move = move;
        }
      } else {
        int32_t heuristic = MoveSearch::alpha_beta_pruning_search<Colors::WHITE>(check, max_search_depth, PieceValues::NEG_INFINITY, PieceValues::POS_INFINITY);
        if (best_heuristic > heuristic) {
          best_heuristic = heuristic;
          best_move = move;
        }
      }
    }
  }
  #ifdef DEBUG
  timer.end();
  timer.print();
  printf("info Moves considered: %llu\n", counter);
  printf("info Leaf nodes: %llu\n", leaf_nodes_counter);
  printf("info Time Pruned: %llu \n", times_pruned);
  #endif

  return best_move;
}

auto MoveSearch::get_position_heuristic(const GameState& game_state) noexcept -> int32_t {
  int32_t heuristic = 0;

  std::array<PieceCode, 6> piece_codes = {
      PieceCodes::PAWN, PieceCodes::KNIGHT, PieceCodes::BISHOP,
      PieceCodes::ROOK, PieceCodes::QUEEN,  PieceCodes::KING};
  std::array<PieceCode, 6> piece_values = {
      PieceValues::PAWN, PieceValues::KNIGHT, PieceValues::BISHOP,
      PieceValues::ROOK, PieceValues::QUEEN,  PieceValues::KING};

  for (auto& piece_code : piece_codes) {
    const BitBoard white_piece_bit_board = game_state.position.get_piece_color_bit_board(piece_code, Colors::WHITE);
    BitBoardUtils::for_each_bit_board(white_piece_bit_board, [&heuristic, &piece_code, &game_state, &piece_values](auto bit_board) {
      heuristic += piece_values[piece_code];
      });

    const BitBoard black_piece_bit_board = game_state.position.get_piece_color_bit_board(piece_code, Colors::BLACK);
    BitBoardUtils::for_each_bit_board(white_piece_bit_board, [&heuristic, &piece_code, &game_state, &piece_values](auto bit_board) {
      heuristic -= piece_values[piece_code];
      });
  }

  // Put the King in check
  heuristic += game_state.is_black_in_check() * PieceValues::PAWN / 2;
  heuristic -= game_state.is_white_in_check() * PieceValues::PAWN / 2;

  // Center Control
  heuristic += game_state.position.is_white_occupied(BitBoards::CENTER_4_SQUARES) * PieceValues::PAWN / 2;
  heuristic -= game_state.position.is_black_occupied(BitBoards::CENTER_4_SQUARES) * PieceValues::PAWN / 2;
  heuristic += game_state.position.is_white_threaten(BitBoards::CENTER_16_SQUARES) * PieceValues::PAWN / 4;
  heuristic -= game_state.position.is_black_threaten(BitBoards::CENTER_16_SQUARES) * PieceValues::PAWN / 4;

  return heuristic;
}
