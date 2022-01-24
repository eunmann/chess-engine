#include "MagicBitBoards.hpp"
#include "Position.hpp"
#include "CapturesGeneration.hpp"
#include "RandomNumberGenerator.hpp"

namespace MagicBitBoards {

  const BishopMagics BISHOP_MAGICS = generate_bishop_magics_cached();
  const RookMagics ROOK_MAGICS = generate_rook_magics_cached();

  auto get_bishop_moves(const Square square, const BitBoard blockers) noexcept -> BitBoard {
    const BitBoard occupied_blocker = blockers & BISHOP_MAGICS.blocker_masks[square];
    const uint64_t index = (occupied_blocker * BISHOP_MAGICS.magics[square]) >> (Squares::NUM - BitBoardUtils::get_count(BISHOP_MAGICS.blocker_masks[square]));
    return BISHOP_MAGICS.moves[square][index];
  }

  auto get_rook_moves(const Square square, const BitBoard blockers) noexcept -> BitBoard {
    const BitBoard occupied_blocker = blockers & ROOK_MAGICS.blocker_masks[square];
    const uint64_t index = (occupied_blocker * ROOK_MAGICS.magics[square]) >> (Squares::NUM - BitBoardUtils::get_count(ROOK_MAGICS.blocker_masks[square]));
    return ROOK_MAGICS.moves[square][index];
  }
}