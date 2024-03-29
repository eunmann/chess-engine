#pragma once

#include "GameState.hpp"
#include "GameUtils.hpp"
#include "Move.hpp"
#include "Moves.hpp"
#include "BitBoardUtils.hpp"
#include "MagicBitBoards.hpp"
#include "CachedMoves.hpp"

namespace MoveGeneration {

    template<const Color color>
    auto get_pawn_moves(const GameState &game_state, Moves &moves) noexcept -> void {
        const auto pawns_bit_board = game_state.position.get_piece_color_bit_board<PieceCodes::PAWN, color>();

        BitBoardUtils::for_each_bit_board(pawns_bit_board, [&game_state, &moves](const BitBoard pawn_bit_board) {

            constexpr auto promotion_row = BitBoards::pawn_promotion_row<color>();
            constexpr auto opponent_color = Colors::opponent_color<color>();
            constexpr auto en_passant_row = BitBoards::pawn_en_passant_row<color>();
            constexpr auto pawn_dir = BitBoards::pawn_direction<color>();

            const Square source_square = BitBoardUtils::bit_board_to_square(pawn_bit_board);

            auto move_bit_board = BitBoards::EMPTY;

            // Move Forward
            auto pawn_moves = CachedMoves::get_pawn_moves<color>(source_square);
            auto unblocked_pawn_moves = pawn_moves & game_state.position.get_empty_bit_board();
            auto is_blocked = game_state.position.get_occupied_bit_board().overlaps(
                    pawn_bit_board.shift<pawn_dir, 0>());
            move_bit_board |= !is_blocked * unblocked_pawn_moves;

            // Take diagonally
            auto pawn_capture_moves = CachedMoves::get_pawn_capture_moves<color>(source_square);
            move_bit_board |= pawn_capture_moves & game_state.position.get_color_bit_board<opponent_color>();

            // En Passant
            move_bit_board |= pawn_capture_moves & en_passant_row &
                              BitBoardUtils::col_from_index(game_state.get_en_passant());

            if (move_bit_board.overlaps(promotion_row)) {
                BitBoardUtils::for_each_set_square(move_bit_board, [&game_state, &moves, source_square](
                        const Square destination_square) {
                    for (auto i = PieceCodes::KNIGHT; i <= PieceCodes::QUEEN; i++) {
                        Move move(source_square, destination_square);
                        move.set_promotion(i);
                        moves.push_back(move);
                    }
                });
            } else {
                BitBoardUtils::for_each_set_square(move_bit_board, [&game_state, &moves, source_square, pawn_bit_board](
                        const Square destination_square) {
                    constexpr auto starting_row = BitBoards::pawn_start_row<color>();
                    constexpr auto forward_2_row = BitBoards::pawn_forward_two_row<color>();

                    auto move = Move(source_square, destination_square);

                    if (pawn_bit_board.overlaps(starting_row) &&
                        destination_square.to_bit_board().overlaps(forward_2_row)) {
                        move.set_en_passant(
                                BitBoardUtils::get_col(destination_square.to_bit_board()));
                    }
                    moves.push_back(move);
                });
            }
        });
    }

    template<const Color color>
    auto get_knight_moves(const GameState &game_state, Moves &moves) noexcept -> void {
        BitBoard knights_bit_board = game_state.position.get_piece_color_bit_board<PieceCodes::KNIGHT, color>();

        BitBoardUtils::for_each_set_square(knights_bit_board, [&game_state, &moves](const auto source_square) {
            const BitBoard non_overlapping_moves =
                    CachedMoves::get_knight_moves(source_square) &
                    game_state.position.get_color_bit_board<color>().invert();
            BitBoardUtils::for_each_set_square(non_overlapping_moves, [source_square, &moves](auto dest_square) {
                moves.push_back(Move(source_square, dest_square));
            });
        });
    }

    template<const Color color>
    auto get_bishop_moves(const GameState &game_state, Moves &moves) noexcept -> void {
        BitBoard bishops_bit_board = game_state.position.get_piece_color_bit_board<PieceCodes::BISHOP, color>();

        BitBoardUtils::for_each_set_square(bishops_bit_board, [&game_state, &moves](const Square source_square) {
            const BitBoard non_overlapping_moves =
                    MagicBitBoards::get_bishop_moves(source_square, game_state.position.get_occupied_bit_board()) &
                    game_state.position.get_color_bit_board<color>().invert();
            BitBoardUtils::for_each_set_square(non_overlapping_moves,
                                               [source_square, &moves](const Square dest_square) {
                                                   moves.push_back(Move(source_square, dest_square));
                                               });
        });
    }

    template<const Color color>
    auto get_rook_moves(const GameState &game_state, Moves &moves) noexcept -> void {
        BitBoard rooks_bit_board = game_state.position.get_piece_color_bit_board<PieceCodes::ROOK, color>();

        BitBoardUtils::for_each_set_square(rooks_bit_board, [&game_state, &moves](const Square source_square) {
            const BitBoard non_overlapping_moves =
                    MagicBitBoards::get_rook_moves(source_square, game_state.position.get_occupied_bit_board()) &
                    game_state.position.get_color_bit_board<color>().invert();
            BitBoardUtils::for_each_set_square(non_overlapping_moves,
                                               [source_square, &moves](const Square dest_square) {
                                                   moves.push_back(Move(source_square, dest_square));
                                               });
        });
    }

    template<const Color color>
    auto get_queen_moves(const GameState &game_state, Moves &moves) noexcept -> void {
        BitBoard queens_bit_board = game_state.position.get_piece_color_bit_board<PieceCodes::QUEEN, color>();

        BitBoardUtils::for_each_set_square(queens_bit_board, [&game_state, &moves](const Square source_square) {
            const BitBoard non_overlapping_moves =
                    (MagicBitBoards::get_bishop_moves(source_square, game_state.position.get_occupied_bit_board()) |
                     MagicBitBoards::get_rook_moves(source_square, game_state.position.get_occupied_bit_board())) &
                    game_state.position.get_color_bit_board<color>().invert();
            BitBoardUtils::for_each_set_square(non_overlapping_moves,
                                               [source_square, &moves](const Square dest_square) {
                                                   moves.push_back(Move(source_square, dest_square));
                                               });
        });
    }

    template<const Color color>
    auto get_king_moves(const GameState &game_state, Moves &moves) noexcept -> void {
        BitBoard kings_bit_board = game_state.position.get_piece_color_bit_board<PieceCodes::KING, color>();

        BitBoardUtils::for_each_bit_board(kings_bit_board, [&game_state, &moves](BitBoard king_bit_board) {
            Square source_square = BitBoardUtils::bit_board_to_square(king_bit_board);

            const BitBoard non_overlapping_moves =
                    CachedMoves::get_king_moves(source_square) &
                    game_state.position.get_color_bit_board<color>().invert();
            BitBoardUtils::for_each_set_square(non_overlapping_moves, [source_square, &moves](auto dest_square) {
                moves.push_back(Move(source_square, dest_square));
            });

            constexpr Castle king = color == Colors::WHITE ? Castles::WHITE_KING : Castles::BLACK_KING;
            constexpr Castle queen = color == Colors::WHITE ? Castles::WHITE_QUEEN : Castles::BLACK_QUEEN;
            constexpr Square king_start =
                    color == Colors::WHITE ? Squares::WHITE_KING_START : Squares::BLACK_KING_START;
            constexpr Square king_end_king =
                    color == Colors::WHITE ? Squares::WHITE_KING_KING_CASTLE_END : Squares::BLACK_KING_KING_CASTLE_END;
            constexpr Square king_end_queen = color == Colors::WHITE ? Squares::WHITE_KING_QUEEN_CASTLE_END
                                                                     : Squares::BLACK_KING_QUEEN_CASTLE_END;

            Move move(king_start, king_end_king);
            move.set_castle(king);
            moves.push_back(move);

            move.set_destination_square(king_end_queen);
            move.set_castle(queen);
            moves.push_back(move);
        });
    }

    template<const Color color>
    auto get_moves(const GameState &game_state, Moves &moves) noexcept -> void {
        // Add moves in order of piece value
        MoveGeneration::get_king_moves<color>(game_state, moves);
        MoveGeneration::get_queen_moves<color>(game_state, moves);
        MoveGeneration::get_rook_moves<color>(game_state, moves);
        MoveGeneration::get_bishop_moves<color>(game_state, moves);
        MoveGeneration::get_knight_moves<color>(game_state, moves);
        MoveGeneration::get_pawn_moves<color>(game_state, moves);
    }
} // namespace MoveGeneration