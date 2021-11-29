#include "GameState.hpp"

GameState::GameState()
    : position(),
      white_to_move(true),
      white_king_moved(false),
      white_rook_1_moved(false),
      white_rook_2_moved(false),
      black_king_moved(false),
      black_rook_1_moved(false),
      black_rook_2_moved(false),
      black_king_in_check(false),
      pawn_ep(-128) {
}