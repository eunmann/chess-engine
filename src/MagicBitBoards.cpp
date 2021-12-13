#include "MagicBitBoards.hpp"
#include "Position.hpp"
#include "MoveGeneration.hpp"
#include <random>

namespace MagicBitBoards {

  std::array<BitBoard, Squares::NUM> BISHOP_BLOCKER_MASKS;
  std::array<uint64_t, Squares::NUM> BISHOP_MAGICS;
  std::array<BitBoard, Squares::NUM * NUM_BISHOP_MOVES> BISHOP_MOVES;

  std::array<BitBoard, Squares::NUM> ROOK_BLOCKER_MASKS;
  std::array<uint64_t, Squares::NUM> ROOK_MAGICS;
  std::array<BitBoard, Squares::NUM * NUM_ROOK_MOVES> ROOK_MOVES;

  auto init() -> void {
    init_bishop();
    init_rook();
  }

  auto init_bishop() noexcept -> void {
    for (Square s = Squares::A1; s < Squares::NUM; s++) {
        std::array<BitBoard, NUM_BISHOP_MOVES> BISHOP_BLOCKERS{};
        
      const BitBoard bit_board = BitBoardUtils::square_to_bit_board(s);

      Position position;
      position.add(PieceCodes::BISHOP, Colors::WHITE, bit_board);
      const BitBoard blocker_mask = MoveGeneration::get_bishop_capture_positions<Colors::WHITE>(position) & ~BitBoards::EDGES;

      BISHOP_BLOCKER_MASKS[s] = blocker_mask;

      const int32_t bit_count = BitBoardUtils::get_count(blocker_mask);
      const int32_t num_of_permutations = 1ULL << bit_count;
      for (int32_t i = 0; i < num_of_permutations; i++) {
        BISHOP_BLOCKERS[i] = generate_blocker_board(i, blocker_mask);

        position.clear();
        position.add(PieceCodes::BISHOP, Colors::WHITE, bit_board);
        position.add(PieceCodes::PAWN, Colors::BLACK, BISHOP_BLOCKERS[i]);

        const int index = s * NUM_BISHOP_MOVES + i;
        BISHOP_MOVES[index] = MoveGeneration::get_bishop_capture_positions<Colors::WHITE>(position);
      }

      uint64_t seed = 728;
      auto generate_magic = [&seed]() {
        seed ^= seed >> 12, seed ^= seed << 25, seed ^= seed >> 27;
        return seed * 2685821657736338717LL;
      };

      while (true) {
          uint64_t magic = 0;

        while (BitBoardUtils::get_count((magic * BISHOP_BLOCKER_MASKS[s]) & 0xFF00000000000000ULL) < 6) {
          magic = generate_magic() & generate_magic() & generate_magic();
        }

        std::array<uint64_t, NUM_BISHOP_MOVES> used_magics{};

        bool magic_found = true;

        for (int32_t i = 0; i < num_of_permutations; i++) {
          const uint32_t magic_index = (BISHOP_BLOCKERS[i] * magic) >> (64 - bit_count);

          const int index = s * NUM_BISHOP_MOVES + magic_index;
          if (used_magics[magic_index] == 0ULL) {
            used_magics[magic_index] = BISHOP_MOVES[index];
          } else if (used_magics[magic_index] != BISHOP_MOVES[index]) {
            magic_found = false;
            break;
          }
        }

        if (magic_found) {
          printf("Found Bishop Magic[%d]: %llu\n", s, magic);
          BISHOP_MAGICS[s] = magic;
          break;
        }
      }
    }
  }

  auto init_rook() noexcept -> void {
    for (Square s = Squares::A1; s < Squares::NUM; s++) {

        std::array<BitBoard, NUM_ROOK_MOVES> ROOK_BLOCKERS{};

      const BitBoard bit_board = BitBoardUtils::square_to_bit_board(s);

      Position position;
      position.add(PieceCodes::ROOK, Colors::WHITE, bit_board);
      BitBoard blocker_mask = BitBoards::EMPTY;

      blocker_mask |= MoveGeneration::get_captures_in_direction<1, 0>(position, bit_board) & ~BitBoards::ROW_8;
      blocker_mask |= MoveGeneration::get_captures_in_direction<-1, 0>(position, bit_board) & ~BitBoards::ROW_1;
      blocker_mask |= MoveGeneration::get_captures_in_direction<0, -1>(position, bit_board) & ~BitBoards::COL_A;
      blocker_mask |= MoveGeneration::get_captures_in_direction<0, 1>(position, bit_board) & ~BitBoards::COL_H;

      ROOK_BLOCKER_MASKS[s] = blocker_mask;

      const int32_t bit_count = BitBoardUtils::get_count(blocker_mask);
      const int32_t num_of_permutations = 1ULL << bit_count;
      for (int32_t i = 0; i < num_of_permutations; i++) {
        ROOK_BLOCKERS[i] = generate_blocker_board(i, blocker_mask);

        position.clear();
        position.add(PieceCodes::ROOK, Colors::WHITE, bit_board);
        position.add(PieceCodes::PAWN, Colors::BLACK, ROOK_BLOCKERS[i]);

        const int32_t index = s * NUM_ROOK_MOVES + i;
        ROOK_MOVES[index] = MoveGeneration::get_rook_capture_positions<Colors::WHITE>(position);
      }

      uint64_t seed = 728;
      auto generate_magic = [&seed]() {
        seed ^= seed >> 12, seed ^= seed << 25, seed ^= seed >> 27;
        return seed * 2685821657736338717LL;
      };

      while (true) {
          uint64_t magic = 0;

        while (BitBoardUtils::get_count((magic * ROOK_BLOCKER_MASKS[s]) & 0xFF00000000000000ULL) < 6) {
          magic = generate_magic() & generate_magic() & generate_magic();
        }

        std::array<uint64_t, NUM_ROOK_MOVES> used_magics{};

        bool magic_found = true;

        for (int32_t i = 0; i < num_of_permutations; i++) {
          const uint32_t magic_index = (ROOK_BLOCKERS[i] * magic) >> (64 - bit_count);

          const int32_t index = s * NUM_ROOK_MOVES + magic_index;
          if (used_magics[magic_index] == 0ULL) {
            used_magics[magic_index] = ROOK_MOVES[index];
          } else if (used_magics[magic_index] != ROOK_MOVES[index]) {
            magic_found = false;
            break;
          }
        }

        if (magic_found) {
          printf("Found Rook Magic[%d]: %llu\n", s, magic);
          ROOK_MAGICS[s] = magic;
          break;
        }
      }
    }
  }

  auto generate_blocker_board(const Square square, const BitBoard blocker_mask) -> BitBoard {

    BitBoard blocker_board = BitBoards::EMPTY;

    int32_t bit_index = 0;
    BitBoardUtils::for_each_set_square(blocker_mask, [&bit_index, &blocker_board, square](const Square s) {
      if (square & (1 << bit_index)) {
        blocker_board |= BitBoardUtils::square_to_bit_board(s);
      }
      bit_index++;
      });

    return blocker_board;
  }

  auto get_bishop_moves(const Square square, const BitBoard blockers) noexcept -> BitBoard {
      const BitBoard occupied_blocker = blockers & BISHOP_BLOCKER_MASKS[square];
    const uint64_t key = (occupied_blocker * BISHOP_MAGICS[square]) >> (64 - BitBoardUtils::get_count(BISHOP_BLOCKER_MASKS[square]));
    return BISHOP_MOVES[square * NUM_BISHOP_MOVES + key];
  }

  auto get_rook_moves(const Square square, const BitBoard blockers) noexcept -> BitBoard {
      const BitBoard occupied_blocker = blockers & ROOK_BLOCKER_MASKS[square];
      const uint64_t key = (occupied_blocker * ROOK_MAGICS[square]) >> (64 - BitBoardUtils::get_count(ROOK_BLOCKER_MASKS[square]));
      return ROOK_MOVES[square * NUM_ROOK_MOVES + key];
  }
}