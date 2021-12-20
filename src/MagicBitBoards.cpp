#include "MagicBitBoards.hpp"
#include "Position.hpp"
#include "MoveGeneration.hpp"
#include "RandomNumberGenerator.hpp"

namespace MagicBitBoards {

  std::array<BitBoard, Squares::NUM> BISHOP_BLOCKER_MASKS;
  std::array<uint64_t, Squares::NUM> BISHOP_MAGICS;
  std::array<std::array<BitBoard, MAX_BISHOP_PERMUTATIONS>, Squares::NUM> BISHOP_MOVES;

  std::array<BitBoard, Squares::NUM> ROOK_BLOCKER_MASKS;
  std::array<uint64_t, Squares::NUM> ROOK_MAGICS;
  std::array<std::array<BitBoard, MAX_ROOK_PERMUTATIONS>, Squares::NUM> ROOK_MOVES;

  auto init() noexcept -> void {
    init_bishop();
    init_rook();
  }

  auto init_bishop() noexcept -> void {
    for (Square square = Squares::A1; square <= Squares::H8; square++) {

      // Build the blocker mask for the square
      const BitBoard bishop_bit_board = BitBoardUtils::square_to_bit_board(square);
      Position position;
      position.add(PieceCodes::BISHOP, Colors::WHITE, bishop_bit_board);
      const BitBoard blocker_mask = MoveGeneration::get_bishop_capture_positions<Colors::WHITE>(position) & ~BitBoards::EDGES;

      // Iterate over all possible permutations of the blocker mask and calculate the possible moves
      const int32_t blocker_mask_bit_count = BitBoardUtils::get_count(blocker_mask);
      const int32_t num_of_permutations = 1ULL << blocker_mask_bit_count;
      std::array<BitBoard, MAX_BISHOP_PERMUTATIONS> bishop_blockers_permutations{};
      for (int32_t i = 0; i < num_of_permutations; i++) {
        bishop_blockers_permutations[i] = generate_blocker_board_permutation(i, blocker_mask);

        position.clear();
        position.add(PieceCodes::BISHOP, Colors::WHITE, bishop_bit_board);
        position.add(PieceCodes::PAWN, Colors::BLACK, bishop_blockers_permutations[i]);

        BISHOP_MOVES[square][i] = MoveGeneration::get_bishop_capture_positions<Colors::WHITE>(position);
      }

      // Search random magic numbers
      RandomNumberGenerator random(728);
      while (true) {
        uint64_t magic = 0;

        // Find a random number with a low number of bits set
        while (BitBoardUtils::get_count((magic * blocker_mask) & 0xFF00000000000000ULL) < 6) {
          magic = random.next_random() & random.next_random() & random.next_random();
        }

        std::array<uint64_t, MAX_BISHOP_PERMUTATIONS> used_magics{};
        bool magic_found = true;

        // Check if the generated magic number can be used for all permutations of the blocker mask
        for (int32_t i = 0; i < num_of_permutations; i++) {
          // Calculate the magic index into the move array
          const uint64_t magic_index = (bishop_blockers_permutations[i] * magic) >> (Squares::NUM - MAX_BISHOP_BLOCKERS);

          // If the magic index isn't used yet, set it
          if (used_magics[magic_index] == 0ULL) {
            used_magics[magic_index] = BISHOP_MOVES[square][i];
          }
          // If the index is already used, see if it maps into the same moves
          else if (used_magics[magic_index] != BISHOP_MOVES[square][i]) {
            magic_found = false;
            break;
          }
        }

        if (magic_found) {
          printf("Found Bishop Magic[%d]: %llu\n", square, magic);
          BISHOP_MAGICS[square] = magic;
          BISHOP_BLOCKER_MASKS[square] = blocker_mask;
          break;
        }
      }
    }
  }

  auto init_rook() noexcept -> void {
    for (Square square = Squares::A1; square <= Squares::H8; square++) {

      // Build the blocker mask for the square
      const BitBoard rook_bit_board = BitBoardUtils::square_to_bit_board(square);
      Position position;
      BitBoard blocker_mask = BitBoards::EMPTY;
      blocker_mask |= MoveGeneration::get_captures_in_direction<1, 0>(position, rook_bit_board) & ~BitBoards::ROW_8;
      blocker_mask |= MoveGeneration::get_captures_in_direction<-1, 0>(position, rook_bit_board) & ~BitBoards::ROW_1;
      blocker_mask |= MoveGeneration::get_captures_in_direction<0, -1>(position, rook_bit_board) & ~BitBoards::COL_A;
      blocker_mask |= MoveGeneration::get_captures_in_direction<0, 1>(position, rook_bit_board) & ~BitBoards::COL_H;

      // Iterate over all possible permutations of the blocker mask and calculate the possible moves
      const int32_t blocker_mask_bit_count = BitBoardUtils::get_count(blocker_mask);
      const int32_t num_of_permutations = 1ULL << blocker_mask_bit_count;
      std::array<BitBoard, MAX_ROOK_PERMUTATIONS> rook_blockers_permutations{};
      for (int32_t i = 0; i < num_of_permutations; i++) {
        rook_blockers_permutations[i] = generate_blocker_board_permutation(i, blocker_mask);

        position.clear();
        position.add(PieceCodes::ROOK, Colors::WHITE, rook_bit_board);
        position.add(PieceCodes::PAWN, Colors::BLACK, rook_blockers_permutations[i]);

        ROOK_MOVES[square][i] = MoveGeneration::get_rook_capture_positions<Colors::WHITE>(position);
      }

      // Search random magic numbers
      RandomNumberGenerator random(728);
      while (true) {
        uint64_t magic = 0;

        // Find a random number with a low number of bits set
        while (BitBoardUtils::get_count((magic * blocker_mask) & 0xFF00000000000000ULL) < 6) {
          magic = random.next_random() & random.next_random() & random.next_random();
        }

        std::array<uint64_t, MAX_ROOK_PERMUTATIONS> used_magics{};
        bool magic_found = true;

        // Check if the generated magic number can be used for all permutations of the blocker mask
        for (int32_t i = 0; i < num_of_permutations; i++) {
          // Calculate the magic index into the move array
          const uint64_t magic_index = (rook_blockers_permutations[i] * magic) >> (Squares::NUM - MAX_ROOK_BLOCKERS);

          // If the magic index isn't used yet, set it
          if (used_magics[magic_index] == 0ULL) {
            used_magics[magic_index] = ROOK_MOVES[square][i];
          }
          // If the index is already used, see if it maps into the same moves
          else if (used_magics[magic_index] != ROOK_MOVES[square][i]) {
            magic_found = false;
            break;
          }
        }

        if (magic_found) {
          printf("Found Rook Magic[%d]: %llu\n", square, magic);
          ROOK_MAGICS[square] = magic;
          ROOK_BLOCKER_MASKS[square] = blocker_mask;
          break;
        }
      }
    }
  }

  auto generate_blocker_board_permutation(const Square square, const BitBoard blocker_mask) noexcept -> BitBoard {

    BitBoard blocker_board = BitBoards::EMPTY;

    int32_t bit_index = 0;
    BitBoardUtils::for_each_bit_board(blocker_mask, [&bit_index, &blocker_board, square](const BitBoard bit_board) {
      if (square & (1 << bit_index)) {
        blocker_board |= bit_board;
      }
      bit_index++;
      });

    return blocker_board;
  }

  auto get_bishop_moves(const Square square, const BitBoard blockers) noexcept -> BitBoard {
    const BitBoard occupied_blocker = blockers & BISHOP_BLOCKER_MASKS[square];
    const uint64_t index = (occupied_blocker * BISHOP_MAGICS[square]) >> (Squares::NUM - MAX_BISHOP_BLOCKERS);
    return BISHOP_MOVES[square][index];
  }

  auto get_rook_moves(const Square square, const BitBoard blockers) noexcept -> BitBoard {
    const BitBoard occupied_blocker = blockers & ROOK_BLOCKER_MASKS[square];
    const uint64_t index = (occupied_blocker * ROOK_MAGICS[square]) >> (Squares::NUM - MAX_ROOK_BLOCKERS);
    return ROOK_MOVES[square][index];
  }
}