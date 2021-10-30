#include "MoveSearch.hpp"

#include "MoveGeneration.hpp"

auto MoveSearch::get_best_move(const GameState &game_state) -> Move {
    Moves moves;
    MoveGeneration::get_moves(game_state, moves);

    int32_t best_heuristic = game_state.white_to_move ? PieceValues::NEG_INFINITY : PieceValues::POS_INFINITY;
    Move best_move;

    for (size_t i = 0; i < moves.size(); ++i) {
        Move &move = moves[i];

        int32_t heuristic = MoveSearch::alpha_beta_pruning_search(game_state, 6, PieceValues::NEG_INFINITY, PieceValues::POS_INFINITY, !game_state.white_to_move);
        if ((game_state.white_to_move && best_heuristic < heuristic) ||
            (!game_state.white_to_move && best_heuristic > heuristic)) {
            best_heuristic = heuristic;
            best_move = move;
        }
    }
    return best_move;
}

auto MoveSearch::alpha_beta_pruning_search(const GameState &game_state, int32_t ply_depth, int32_t alpha, int32_t beta, bool max_white) -> int32_t {
    if (ply_depth == 0) {
        return MoveSearch::get_position_heuristic(game_state);
    }

    Moves moves;
    MoveGeneration::get_moves(game_state, moves);

    if (max_white) {
        int32_t best_heuristic = PieceValues::NEG_INFINITY;
        for (size_t i = 0; i < moves.size(); ++i) {
            Move &move = moves[i];
            best_heuristic = std::max(best_heuristic, MoveSearch::alpha_beta_pruning_search(game_state, ply_depth - 1, alpha, beta, false));
            if (best_heuristic >= beta) {
                break;
            }
            alpha = std::max(alpha, best_heuristic);
        }
        return best_heuristic;
    } else {
        int32_t best_heuristic = PieceValues::POS_INFINITY;
        for (size_t i = 0; i < moves.size(); ++i) {
            Move &move = moves[i];
            best_heuristic = std::min(best_heuristic, MoveSearch::alpha_beta_pruning_search(game_state, ply_depth - 1, alpha, beta, true));
            if (best_heuristic <= alpha) {
                break;
            }
            beta = std::min(beta, best_heuristic);
        }
        return best_heuristic;
    }
}

auto MoveSearch::get_position_heuristic(const GameState &game_state) -> int32_t {
    int32_t heuristic = 0;

    /* Sum piece values */
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
            case PieceCodes::NONE:
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
            case PieceCodes::NONE:
            default: {
            }
        }
    }

    /* Put the King in check */
    if (game_state.black_king_in_check) {
        heuristic += PieceValues::PAWN / 2;
    }

    if (game_state.white_king_in_check) {
        heuristic -= PieceValues::PAWN / 2;
    }

    /* Bad check for Castling */
    if (game_state.black_king_moved && !(game_state.black_rook_1_moved || game_state.black_rook_2_moved)) {
        heuristic += PieceValues::PAWN / 2;
    }

    if (game_state.white_king_moved && !(game_state.white_rook_1_moved || game_state.white_rook_2_moved)) {
        heuristic -= PieceValues::PAWN / 2;
    }

    return heuristic;
}
