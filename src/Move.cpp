#include "Move.hpp"

#include "GameUtils.hpp"

Move::Move() : m_move(0) {}

Move::Move(const int source, const int dest) : m_move(source | (dest << 6)) {
    this->set_promotion(PieceCodes::NUM);
    this->set_en_passant(9);
}

Move::Move(const int& move) : m_move(move) {}

auto Move::get_source_square() const -> Square {
    return static_cast<Square>(this->m_move & Move::MASK_6_BITS);
}

auto Move::get_destination_square() const -> Square {
    return static_cast<Square>((this->m_move >> 6) & Move::MASK_6_BITS);
}

auto Move::get_source_bit_board() const -> BitBoard {
    return GameUtils::square_to_bit_board(this->get_source_square());
}

auto Move::get_destination_bit_board() const -> BitBoard {
    GameUtils::square_to_bit_board(this->get_destination_square());
    return 0x1UL << (this->get_destination_square() - 1);
}

auto Move::get_promotion() const -> PieceCode {
    return (this->m_move >> PROMO_OFFSET) & MASK_6_BITS;
}

auto Move::get_en_passant() const -> int32_t {
    return (this->m_move >> EN_OFFSET) & MASK_4_BITS;
}

auto Move::get_castle() const -> Castle {
    return (this->m_move >> CASTLE_OFFSET) & MASK_3_BITS;
}

auto Move::set_promotion(const PieceCode piece_code) -> void {
    this->m_move |= piece_code << PROMO_OFFSET;
}

auto Move::set_en_passant(const int32_t column_index) -> void {
    this->m_move |= column_index << EN_OFFSET;
}

auto Move::set_castle(const Castle castle) -> void {
    this->m_move |= castle << CASTLE_OFFSET;
}

auto Move::is_promotion() const -> bool {
    return ((this->m_move >> PROMO_OFFSET) & MASK_6_BITS) != PieceCodes::NUM;
}

auto Move::is_en_passantable() const -> bool {
    return ((this->m_move >> EN_OFFSET) & MASK_4_BITS) < 8;
}

auto Move::is_castle() const -> bool {
    return ((this->m_move >> CASTLE_OFFSET) & MASK_3_BITS) != 0;
}

auto Move::to_string() const -> std::string {
    std::string move_str = "";

    if (this->is_castle()) {
        switch (this->get_castle()) {
            case Castles::WHITE_KING: {
                move_str = "e1g1";
                break;
            }
            case Castles::WHITE_QUEEN: {
                move_str = "e1c1";
                break;
            }
            case Castles::BLACK_KING: {
                move_str = "e8g8";
                break;
            }
            case Castles::BLACK_QUEEN: {
                move_str = "e8c8";
                break;
            }
        }
    } else {
        move_str += GameUtils::get_tile_name(this->get_source_bit_board());
        move_str += GameUtils::get_tile_name(this->get_destination_bit_board());
        if (this->is_promotion()) {
            switch (this->get_promotion()) {
                case PieceCodes::BISHOP: {
                    move_str += 'b';
                    break;
                }
                case PieceCodes::KNIGHT: {
                    move_str += 'k';
                    break;
                }
                case PieceCodes::ROOK: {
                    move_str += 'r';
                    break;
                }
                case PieceCodes::QUEEN: {
                    move_str += 'q';
                    break;
                }
            }
        }
    }
    return move_str;
}

auto Move::operator==(const Move& move) const -> bool {
    return this->m_move == move.m_move;
}
