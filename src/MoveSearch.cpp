#include "MoveSearch.hpp"

#include "MoveGeneration.hpp"

auto MoveSearch::get_best_move(const GameState &game_state) -> Move {
    Moves moves;
    Color color_to_move = Colors::bool_to_color(game_state.white_to_move);
    if (color_to_move == Colors::WHITE) {
        MoveGeneration::get_moves<Colors::WHITE>(game_state, moves);
    } else {
        MoveGeneration::get_moves<Colors::BLACK>(game_state, moves);
    }

    int32_t best_heuristic = color_to_move == Colors::WHITE ? PieceValues::NEG_INFINITY : PieceValues::POS_INFINITY;
    Move best_move;

    for (size_t i = 0; i < moves.size(); ++i) {
        Move &move = moves[i];

        int32_t heuristic = 0;
        if (color_to_move == Colors::WHITE) {
            heuristic = MoveSearch::alpha_beta_pruning_search<Colors::BLACK>(game_state, 6, PieceValues::NEG_INFINITY, PieceValues::POS_INFINITY);
        } else {
            heuristic = MoveSearch::alpha_beta_pruning_search<Colors::WHITE>(game_state, 6, PieceValues::NEG_INFINITY, PieceValues::POS_INFINITY);
        }

        if ((game_state.white_to_move && best_heuristic < heuristic) ||
            (!game_state.white_to_move && best_heuristic > heuristic)) {
            best_heuristic = heuristic;
            best_move = move;
        }
    }
    return best_move;
}

auto MoveSearch::get_position_heuristic(const GameState &game_state) -> int32_t {
    // TODO(EMU): This function can be optimized
    int32_t heuristic = 0;

    // Sum piece values
    for (uint64_t i = 0; i < PIECES_PER_PLAYER; ++i) {
        switch (game_state.position.get_piece_bit_board(i)) {
            case PieceCodes::PAWN: {
                heuristic += PieceValues::PAWN;
                break;
            }
            case PieceCodes::KNIGHT: {
                heuristic += PieceValues::KNIGHT;
                break;
            }
            case PieceCodes::BISHOP: {
                heuristic += PieceValues::BISHOP;
                break;
            }
            case PieceCodes::ROOK: {
                heuristic += PieceValues::ROOK;
                break;
            }
            case PieceCodes::QUEEN: {
                heuristic += PieceValues::QUEEN;
                break;
            }
            case PieceCodes::KING: {
                heuristic += PieceValues::KING;
                break;
            }
            default: {
            }
        }
    }

    for (uint64_t i = PIECES_PER_PLAYER; i < (PIECES_PER_PLAYER * 2); ++i) {
        switch (game_state.position.get_piece_bit_board(i)) {
            case PieceCodes::PAWN: {
                heuristic -= PieceValues::PAWN;
                break;
            }
            case PieceCodes::KNIGHT: {
                heuristic -= PieceValues::KNIGHT;
                break;
            }
            case PieceCodes::BISHOP: {
                heuristic -= PieceValues::BISHOP;
                break;
            }
            case PieceCodes::ROOK: {
                heuristic -= PieceValues::ROOK;
                break;
            }
            case PieceCodes::QUEEN: {
                heuristic -= PieceValues::QUEEN;
                break;
            }
            case PieceCodes::KING: {
                heuristic -= PieceValues::KING;
                break;
            }
            case PieceCodes::NUM:
            default: {
            }
        }
    }

    // Put the King in check
    if (game_state.black_king_in_check) {
        heuristic += PieceValues::PAWN / 2;
    }

    if (game_state.white_king_in_check) {
        heuristic -= PieceValues::PAWN / 2;
    }

    // Bad check for Castling
    if (game_state.black_king_moved && !(game_state.black_rook_1_moved || game_state.black_rook_2_moved)) {
        heuristic += PieceValues::PAWN / 2;
    }

    if (game_state.white_king_moved && !(game_state.white_rook_1_moved || game_state.white_rook_2_moved)) {
        heuristic -= PieceValues::PAWN / 2;
    }

    return heuristic;
}
