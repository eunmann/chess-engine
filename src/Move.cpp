#include "Move.hpp"

#include "Assert.hpp"

#include "GameUtils.hpp"

Move::Move() noexcept: Move(Square(0), Square(0)) {
}

Move::Move(const Square source, const Square dest) noexcept: m_move() {
    //ASSERT(source != dest);
    this->set_source_square(source);
    this->set_destination_square(dest);
    this->set_promotion(PieceCode(Move::MASK_3_BITS));
    this->set_en_passant(Move::MASK_4_BITS);
    this->set_castle(Castles::NONE);
}

Move::Move(const int &move) noexcept: m_move(move) {
}

auto Move::set_source_square(const Square square) noexcept -> void {
    this->m_move.set_bits<Move::MASK_6_BITS, Move::SOURCE_OFFSET>(square.value());
}

auto Move::set_destination_square(const Square square) noexcept -> void {
    this->m_move.set_bits<Move::MASK_6_BITS, Move::DEST_OFFSET>(square.value());
}

auto Move::get_source_square() const noexcept -> Square {
    return Square(this->m_move.get_bits<Move::MASK_6_BITS, Move::SOURCE_OFFSET>());
}

auto Move::get_destination_square() const noexcept -> Square {
    return Square(this->m_move.get_bits<Move::MASK_6_BITS, Move::DEST_OFFSET>());
}

auto Move::get_source_bit_board() const noexcept -> BitBoard {
    return this->get_source_square().to_bit_board();
}

auto Move::get_destination_bit_board() const noexcept -> BitBoard {
    return this->get_destination_square().to_bit_board();
}

auto Move::get_promotion() const noexcept -> PieceCode {
    return PieceCode(this->m_move.get_bits<Move::MASK_3_BITS, Move::PROMO_OFFSET>());
}

auto Move::get_en_passant() const noexcept -> int32_t {
    return this->m_move.get_bits<Move::MASK_4_BITS, Move::EN_OFFSET>();
}

auto Move::get_castle() const noexcept -> Castle {
    return Castle(this->m_move.get_bits<Move::MASK_3_BITS, Move::CASTLE_OFFSET>());
}

auto Move::set_promotion(const PieceCode piece_code) noexcept -> void {
    this->m_move.set_bits<Move::MASK_3_BITS, Move::PROMO_OFFSET>(piece_code.value);
}

auto Move::set_en_passant(const int32_t column_index) noexcept -> void {
    this->m_move.set_bits<Move::MASK_4_BITS, Move::EN_OFFSET>(column_index);
}

auto Move::set_castle(const Castle castle) noexcept -> void {
    this->m_move.set_bits<Move::MASK_3_BITS, Move::CASTLE_OFFSET>(castle.value());
}

auto Move::is_promotion() const noexcept -> bool {
    return this->get_promotion().value != Move::MASK_3_BITS;
}

auto Move::is_en_passantable() const noexcept -> bool {
    return this->get_en_passant() != Move::MASK_4_BITS;
}

auto Move::is_castle() const noexcept -> bool {
    return this->get_castle() != Castles::NONE;
}

auto Move::to_string() const noexcept -> std::string {
    std::string move_str;

    auto is_castle = this->is_castle();
    if (is_castle) {
        auto castle = this->get_castle();
        switch (castle.value()) {
            case Castles::WHITE_KING.value(): {
                move_str = "e1g1";
                break;
            }
            case Castles::WHITE_QUEEN.value(): {
                move_str = "e1c1";
                break;
            }
            case Castles::BLACK_KING.value(): {
                move_str = "e8g8";
                break;
            }
            case Castles::BLACK_QUEEN.value(): {
                move_str = "e8c8";
                break;
            }
            default: {
                ASSERT(false);
            }
        }
    } else {
        move_str += BitBoardUtils::get_tile_name(this->get_source_bit_board());
        move_str += BitBoardUtils::get_tile_name(this->get_destination_bit_board());

        // This is the default value. "0000" is considered the nullmove, meaning we didn't find a move
        if (move_str == "a1a1") {
            return "0000";
        }

        if (this->is_promotion()) {
            switch (this->get_promotion().value) {
                case PieceCodes::BISHOP.value: {
                    move_str += 'b';
                    break;
                }
                case PieceCodes::KNIGHT.value: {
                    move_str += 'k';
                    break;
                }
                case PieceCodes::ROOK.value: {
                    move_str += 'r';
                    break;
                }
                case PieceCodes::QUEEN.value: {
                    move_str += 'q';
                    break;
                }
                default: {
                    ASSERT(false);
                }
            }
        }
    }
    return move_str;
}

auto Move::operator==(const Move &move) const noexcept -> bool {
    return this->m_move == move.m_move;
}
