
#pragma once

#include <array>
#include "CommonHeaders.hpp"
#include "RandomNumberGenerator.hpp"

class ZobristHash {

public:
    constexpr ZobristHash() {
        this->init();
    }

private:
    static constexpr auto
    piece_square_index(const Color color, const PieceCode piece_code, const Square square) noexcept -> int32_t {

        const auto color_offset = color.value * (PieceCodes::NUM.value * Squares::NUM.value());
        const auto piece_offset = piece_code.value * Squares::NUM.value();
        return color_offset + piece_offset +
               square.value();
    }

    static constexpr auto castle_index(const Castle castle) noexcept -> int32_t {
        return castle.value() & 0b11;
    }

    [[nodiscard]] constexpr auto get_piece_square_hash(const Color color, const PieceCode piece_code,
                                                       const Square square) const noexcept -> BitBoard {
        return this->m_piece_square[piece_square_index(color, piece_code, square)];
    }

    [[nodiscard]] constexpr auto get_black_to_move_hash() const noexcept -> BitBoard {
        return this->m_black_to_move;
    }

    [[nodiscard]] constexpr auto get_castle_hash(const Castle castle) const noexcept -> BitBoard {
        return this->m_castle[castle_index(castle)];
    }

    [[nodiscard]] constexpr auto get_en_passant_hash(const int32_t col_index) const noexcept -> BitBoard {
        return this->m_en_passant[col_index];
    }

    constexpr auto init() noexcept -> void {

        RandomNumberGenerator random_number_generator(728);

        for (auto color = Colors::WHITE; color <= Colors::NUM; color++) {
            for (auto piece_code = PieceCodes::PAWN; piece_code <= PieceCodes::NUM; piece_code++) {
                for (auto square = Squares::A1; square <= Squares::NUM; square++) {
                    const auto index = ZobristHash::piece_square_index(color, piece_code, square);
                    this->m_piece_square[index] = random_number_generator.next_random();
                }
            }
        }

        this->m_black_to_move = random_number_generator.next_random();

        for (auto castle = Castles::WHITE_KING; castle <= Castles::BLACK_QUEEN; castle++) {
            this->m_castle[castle_index(castle)] = random_number_generator.next_random();
        }

        for (auto col_index = 0; col_index < BOARD_DIM; col_index++) {
            this->m_en_passant[col_index] = random_number_generator.next_random();
        }
    }

    std::array<BitBoard, Squares::NUM.value() * PieceCodes::NUM.value * Colors::NUM.value> m_piece_square;
    BitBoard m_black_to_move;
    std::array<BitBoard, Castles::NUM.value()> m_castle;
    std::array<BitBoard, BOARD_DIM> m_en_passant;
};

namespace ZobristHashes {
    extern const ZobristHash ZOBRIST_HASH = ZobristHash();
};