#include "Move.hpp"

#include "Assert.hpp"

#include "GameUtils.hpp"

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
