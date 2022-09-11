#pragma once

#include <array>
#include <string>

#include "CommonHeaders.hpp"
#include "BitMaskValue.hpp"

class Move {
public:
    constexpr Move() noexcept = default;

    constexpr Move(const Square source, const Square dest) noexcept {
        //ASSERT(source != dest);
        this->set_source_square(source);
        this->set_destination_square(dest);
        this->set_promotion(PieceCode(Move::MASK_3_BITS));
        this->set_en_passant(Move::MASK_4_BITS);
        this->set_castle(Castles::NONE);
    }

    constexpr explicit Move(const int &move) noexcept: m_move(move) {}

    constexpr auto set_source_square(const Square square) noexcept -> void {
        this->m_move.set_bits<Move::MASK_6_BITS, Move::SOURCE_OFFSET>(square.value());
    }

    constexpr auto set_destination_square(const Square square) noexcept -> void {
        this->m_move.set_bits<Move::MASK_6_BITS, Move::DEST_OFFSET>(square.value());
    }

    [[nodiscard]] constexpr auto get_source_square() const noexcept -> Square {
        return Square(this->m_move.get_bits<Move::MASK_6_BITS, Move::SOURCE_OFFSET>());
    }

    [[nodiscard]] constexpr auto get_destination_square() const noexcept -> Square {
        return Square(this->m_move.get_bits<Move::MASK_6_BITS, Move::DEST_OFFSET>());
    }

    [[nodiscard]] constexpr auto get_source_bit_board() const noexcept -> BitBoard {
        return this->get_source_square().to_bit_board();
    }

    [[nodiscard]] constexpr auto get_destination_bit_board() const noexcept -> BitBoard {
        return this->get_destination_square().to_bit_board();
    }

    [[nodiscard]] constexpr auto get_promotion() const noexcept -> PieceCode {
        return PieceCode(this->m_move.get_bits<Move::MASK_3_BITS, Move::PROMO_OFFSET>());
    }

    [[nodiscard]] constexpr auto get_en_passant() const noexcept -> int32_t {
        return this->m_move.get_bits<Move::MASK_4_BITS, Move::EN_OFFSET>();
    }

    [[nodiscard]] constexpr auto get_castle() const noexcept -> Castle {
        return Castle(this->m_move.get_bits<Move::MASK_3_BITS, Move::CASTLE_OFFSET>());
    }

    constexpr auto set_promotion(PieceCode piece_code) noexcept -> void {
        this->m_move.set_bits<Move::MASK_3_BITS, Move::PROMO_OFFSET>(piece_code.value);
    }

    constexpr auto set_en_passant(int32_t column_index) noexcept -> void {
        this->m_move.set_bits<Move::MASK_4_BITS, Move::EN_OFFSET>(column_index);
    }

    constexpr auto set_castle(Castle castle) noexcept -> void {
        this->m_move.set_bits<Move::MASK_3_BITS, Move::CASTLE_OFFSET>(castle.value());
    }

    [[nodiscard]] constexpr auto is_promotion() const noexcept -> bool {
        return this->get_promotion().value != Move::MASK_3_BITS;
    }

    [[nodiscard]] constexpr auto is_en_passantable() const noexcept -> bool {
        return this->get_en_passant() != Move::MASK_4_BITS;
    }

    [[nodiscard]] constexpr auto is_castle() const noexcept -> bool {
        return this->get_castle() != Castles::NONE;
    }

    [[nodiscard]] auto to_string() const noexcept -> std::string;

    constexpr auto operator==(const Move &move) const noexcept -> bool {
        return this->m_move == move.m_move;
    }

private:
    BitMaskValue m_move;
    static constexpr uint32_t SOURCE_OFFSET = 0;
    static constexpr uint32_t SOURCE_BIT_NUM = 6;

    static constexpr uint32_t DEST_OFFSET = SOURCE_OFFSET + SOURCE_BIT_NUM;
    static constexpr uint32_t DEST_BIT_NUM = 6;

    static constexpr uint32_t PROMO_OFFSET = DEST_OFFSET + DEST_BIT_NUM;
    static constexpr uint32_t PROMO_BIT_NUM = 3;

    static constexpr uint32_t EN_OFFSET = PROMO_OFFSET + PROMO_BIT_NUM;
    static constexpr uint32_t EN_BIT_NUM = 4;

    static constexpr uint32_t CASTLE_OFFSET = EN_OFFSET + EN_BIT_NUM;
    static constexpr uint32_t CASTLE_BIT_NUM = 3;

    static constexpr uint32_t MASK_6_BITS = 0b0011'1111;
    static constexpr uint32_t MASK_4_BITS = 0b0000'1111;
    static constexpr uint32_t MASK_3_BITS = 0b0000'0111;
};
