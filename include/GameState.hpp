#pragma once

#include <cinttypes>

#include "Move.hpp"
#include "Position.hpp"
#include "BitMaskValue.hpp"
#include "BitBoardUtils.hpp"
#include "Assert.hpp"

class GameState {
public:
    GameState() noexcept;

    auto init() noexcept -> void;

    template<const Color color>
    auto apply_move(const Move move) noexcept -> void {

        const BitBoard source_bit_board = move.get_source_bit_board();
        const BitBoard destination_bit_board = move.get_destination_bit_board();

        // Trying to move the opponents piece or no piece at all is not legal
        if (color != this->position.get_color(source_bit_board)) {
            this->set_is_legal(false);
            return;
        }

        // Trying to capture a piece of the same color is not legal
        if (this->position.is_color_occupied<color>(destination_bit_board)) {
            this->set_is_legal(false);
            return;
        }

        const bool is_castle = move.is_castle();
        PieceCode piece_code;

        if (is_castle) {

            piece_code = PieceCodes::KING;

            if (this->has_king_moved<color>() ||
                this->is_color_in_check<color>()) {
                this->set_is_legal(false);
                return;
            }

            const Castle castle = move.get_castle();
            if constexpr (color == Colors::WHITE) {
                if (castle == Castles::WHITE_KING) {
                    if (!this->can_king_castle<color>(*this)) {
                        this->set_is_legal(false);
                        return;
                    }
                    this->castle_king_side<color>();
                } else {
                    if (!this->can_queen_castle<color>(*this)) {
                        this->set_is_legal(false);
                        return;
                    }
                    this->castle_queen_side<color>();
                }
            } else if constexpr (color == Colors::BLACK) {
                if (castle == Castles::BLACK_KING) {
                    if (!this->can_king_castle<color>(*this)) {
                        this->set_is_legal(false);
                        return;
                    }
                    this->castle_king_side<color>();
                } else {
                    if (!this->can_queen_castle<color>(*this)) {
                        this->set_is_legal(false);
                        return;
                    }
                    this->castle_queen_side<color>();
                }
            }
        } else {
            if (move.is_promotion()) {
                piece_code = move.get_promotion();
            } else {
                piece_code = this->position.get_piece_type(source_bit_board);
            }

            constexpr auto starting_row = color == Colors::WHITE ? 1 : 6;
            constexpr auto forward_2_row = color == Colors::WHITE ? 3 : 4;
            constexpr auto en_passant_row = color == Colors::WHITE ? BitBoards::ROW_3 : BitBoards::ROW_6;
            constexpr auto en_passant_opponent_row = color == Colors::WHITE ? BitBoards::ROW_5 : BitBoards::ROW_4;
            auto en_passant_col = BitBoardUtils::get_col(this->get_en_passant());

            if (piece_code == PieceCodes::PAWN) {
                if (BitBoardUtils::is_piece_in_row(source_bit_board, starting_row) &&
                    BitBoardUtils::is_piece_in_row(destination_bit_board, forward_2_row)) {
                    this->set_en_passant(BitBoardUtils::get_col(source_bit_board));
                } else if (this->get_en_passant() != MASK_4_BIT &&
                           (en_passant_row & en_passant_col) == destination_bit_board) {
                    this->position.clear(en_passant_opponent_row & en_passant_col);
                }
            } else {
                this->set_en_passant(MASK_4_BIT);
            }

            this->position.clear(source_bit_board | destination_bit_board);
            this->position.add(piece_code, color, destination_bit_board);
        }

        this->position.recompute_threaten();

        this->set_is_legal(!this->is_color_in_check<color>());

        this->set_white_to_move(color != Colors::WHITE);

        this->set_king_moved<color>(piece_code == PieceCodes::KING || is_castle || this->has_king_moved<color>());

        constexpr BitBoard rook_a_start =
                color == Colors::WHITE ? BitBoards::WHITE_ROOK_A_START : BitBoards::BLACK_ROOK_A_START;
        this->set_rook_A_moved<color>((piece_code == PieceCodes::ROOK && source_bit_board == rook_a_start) ||
                                      this->has_rook_A_moved<color>());

        constexpr BitBoard rook_h_start =
                color == Colors::WHITE ? BitBoards::WHITE_ROOK_H_START : BitBoards::BLACK_ROOK_H_START;
        this->set_rook_H_moved<color>((piece_code == PieceCodes::ROOK && source_bit_board == rook_h_start) ||
                                      this->has_rook_H_moved<color>());
    }

    [[nodiscard]] auto is_white_in_check() const noexcept -> bool;

    [[nodiscard]] auto is_black_in_check() const noexcept -> bool;

    template<const Color color>
    [[nodiscard]] auto is_color_in_check() const noexcept -> bool {
        constexpr Color opponent_color = color == Colors::WHITE ? Colors::BLACK : Colors::WHITE;
        const BitBoard king_bit_board = this->position.get_piece_color_bit_board<PieceCodes::KING, color>();
        return this->position.is_threaten<opponent_color>(king_bit_board);
    }

    [[nodiscard]] auto is_white_to_move() const noexcept -> bool;

    auto set_white_to_move(bool white_move) noexcept -> void;

    template<const Color color>
    [[nodiscard]] auto has_king_moved() const noexcept -> bool {
        if constexpr (color == Colors::WHITE) {
            return this->m_flags.get_bits<GameState::MASK_1_BIT, GameState::WHITE_KING_MOVED_OFFSET>();
        } else if constexpr (color == Colors::BLACK) {
            return this->m_flags.get_bits<GameState::MASK_1_BIT, GameState::BLACK_KING_MOVED_OFFSET>();
        }
    }

    template<const Color color>
    auto set_king_moved(const bool did_move) noexcept -> void {
        if constexpr (color == Colors::WHITE) {
            this->m_flags.set_bits<GameState::MASK_1_BIT, GameState::WHITE_KING_MOVED_OFFSET>(did_move);
        } else if constexpr (color == Colors::BLACK) {
            this->m_flags.set_bits<GameState::MASK_1_BIT, GameState::BLACK_KING_MOVED_OFFSET>(did_move);
        }
    }

    template<const Color color>
    [[nodiscard]] auto has_rook_A_moved() const noexcept -> bool {
        if constexpr (color == Colors::WHITE) {
            return this->m_flags.get_bits<GameState::MASK_1_BIT, GameState::WHITE_ROOK_A_OFFSET>();
        } else if constexpr (color == Colors::BLACK) {
            return this->m_flags.get_bits<GameState::MASK_1_BIT, GameState::BLACK_ROOK_A_OFFSET>();
        }
    }

    template<const Color color>
    auto set_rook_A_moved(const bool did_move) noexcept -> void {
        if constexpr (color == Colors::WHITE) {
            this->m_flags.set_bits<GameState::MASK_1_BIT, GameState::WHITE_ROOK_A_OFFSET>(did_move);
        } else if constexpr (color == Colors::BLACK) {
            this->m_flags.set_bits<GameState::MASK_1_BIT, GameState::BLACK_ROOK_A_OFFSET>(did_move);
        }
    }

    template<const Color color>
    [[nodiscard]] auto has_rook_H_moved() const noexcept -> bool {
        if constexpr (color == Colors::WHITE) {
            return this->m_flags.get_bits<GameState::MASK_1_BIT, GameState::WHITE_ROOK_H_OFFSET>();
        } else if constexpr (color == Colors::BLACK) {
            return this->m_flags.get_bits<GameState::MASK_1_BIT, GameState::BLACK_ROOK_H_OFFSET>();
        }
    }

    template<const Color color>
    auto set_rook_H_moved(const bool did_move) noexcept -> void {
        if constexpr (color == Colors::WHITE) {
            this->m_flags.set_bits<GameState::MASK_1_BIT, GameState::WHITE_ROOK_H_OFFSET>(did_move);
        } else if constexpr (color == Colors::BLACK) {
            this->m_flags.set_bits<GameState::MASK_1_BIT, GameState::BLACK_ROOK_H_OFFSET>(did_move);
        }
    }

    auto set_is_legal(bool is_legal) noexcept -> void;

    [[nodiscard]] auto is_legal() const noexcept -> bool;

    auto set_en_passant(int32_t pawn_ep) noexcept -> void;

    [[nodiscard]] auto get_en_passant() const noexcept -> int32_t;

    template<const Color color>
    auto can_queen_castle(const GameState &game_state) -> bool {
        constexpr auto queen_castle =
                color == Colors::WHITE ? BitBoards::WHITE_QUEEN_CASTLE : BitBoards::BLACK_QUEEN_CASTLE;
        constexpr auto opponent_color = color == Colors::WHITE ? Colors::BLACK : Colors::WHITE;
        return !(game_state.has_rook_A_moved<color>() ||
                 !game_state.position.is_empty(queen_castle) ||
                 game_state.position.is_threaten<opponent_color>(queen_castle));
    };

    template<const Color color>
    auto can_king_castle(const GameState &game_state) -> bool {
        constexpr auto king_castle =
                color == Colors::WHITE ? BitBoards::WHITE_KING_CASTLE : BitBoards::BLACK_KING_CASTLE;
        constexpr auto opponent_color = color == Colors::WHITE ? Colors::BLACK : Colors::WHITE;
        return !(game_state.has_rook_H_moved<color>() ||
                 !game_state.position.is_empty(king_castle) ||
                 game_state.position.is_threaten<opponent_color>(king_castle));
    };

    template<const Color color>
    auto castle_king_side() -> void {

        constexpr auto king_start = color == Colors::WHITE ? BitBoards::WHITE_KING_START : BitBoards::BLACK_KING_START;
        constexpr auto h_rook_start =
                color == Colors::WHITE ? BitBoards::WHITE_ROOK_H_START : BitBoards::BLACK_ROOK_H_START;
        constexpr auto king_end =
                color == Colors::WHITE ? BitBoards::WHITE_KING_KING_CASTLE : BitBoards::BLACK_KING_KING_CASTLE;
        constexpr auto rook_end =
                color == Colors::WHITE ? BitBoards::WHITE_ROOK_KING_CASTLE : BitBoards::BLACK_ROOK_KING_CASTLE;

        this->position.clear(king_start | h_rook_start);
        this->position.add(PieceCodes::KING, color, king_end);
        this->position.add(PieceCodes::ROOK, color, rook_end);
        this->set_rook_H_moved<color>(true);
    }

    template<const Color color>
    auto castle_queen_side() -> void {

        constexpr auto king_start = color == Colors::WHITE ? BitBoards::WHITE_KING_START : BitBoards::BLACK_KING_START;
        constexpr auto h_rook_start =
                color == Colors::WHITE ? BitBoards::WHITE_ROOK_A_START : BitBoards::BLACK_ROOK_A_START;
        constexpr auto king_end =
                color == Colors::WHITE ? BitBoards::WHITE_KING_QUEEN_CASTLE : BitBoards::BLACK_KING_QUEEN_CASTLE;
        constexpr auto rook_end =
                color == Colors::WHITE ? BitBoards::WHITE_ROOK_QUEEN_CASTLE : BitBoards::BLACK_ROOK_QUEEN_CASTLE;

        this->position.clear(king_start | h_rook_start);
        this->position.add(PieceCodes::KING, color, king_end);
        this->position.add(PieceCodes::ROOK, color, rook_end);
        this->set_rook_H_moved<color>(true);
    }

    Position position;
private:
    BitMaskValue m_flags;
    static constexpr int32_t MASK_1_BIT = 0b1;
    static constexpr int32_t WHITE_TO_MOVE_OFFSET = 0;
    static constexpr int32_t WHITE_KING_MOVED_OFFSET = WHITE_TO_MOVE_OFFSET + MASK_1_BIT;
    static constexpr int32_t WHITE_ROOK_A_OFFSET = WHITE_KING_MOVED_OFFSET + MASK_1_BIT;
    static constexpr int32_t WHITE_ROOK_H_OFFSET = WHITE_ROOK_A_OFFSET + MASK_1_BIT;
    static constexpr int32_t BLACK_KING_MOVED_OFFSET = WHITE_ROOK_H_OFFSET + MASK_1_BIT;
    static constexpr int32_t BLACK_ROOK_A_OFFSET = BLACK_KING_MOVED_OFFSET + MASK_1_BIT;
    static constexpr int32_t BLACK_ROOK_H_OFFSET = BLACK_ROOK_A_OFFSET + MASK_1_BIT;
    static constexpr int32_t IS_LEGAL_OFFSET = BLACK_ROOK_H_OFFSET + MASK_1_BIT;
    static constexpr int32_t MASK_4_BIT = 0b1111;
    static constexpr int32_t PAWN_EN_OFFSET = IS_LEGAL_OFFSET + MASK_1_BIT;
};