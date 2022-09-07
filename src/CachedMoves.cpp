#include "CachedMoves.hpp"

#include <ranges>
#include "BitBoardUtils.hpp"
#include "CapturesGeneration.hpp"
#include "Timer.hpp"

namespace CachedMoves {

    const std::array<BitBoard, Squares::NUM.value()> KNIGHT = generate_knight_moves();
    const std::array<BitBoard, Squares::NUM.value()> KING = generate_king_moves();
    const std::array<BitBoard, Squares::NUM.value() * Colors::NUM.value> PAWN_CAPTURES = generate_pawn_captures();
    const std::array<BitBoard, Squares::NUM.value() * Colors::NUM.value> PAWN = generate_pawn_moves();
}