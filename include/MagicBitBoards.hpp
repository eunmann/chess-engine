#pragma once

#include "Definitions.hpp"
#include <numeric>
#include <algorithm>
#include "BitBoardUtils.hpp"
#include "RandomNumberGenerator.hpp"
#include "CapturesGeneration.hpp"

constexpr int32_t MAX_ROOK_BLOCKERS = 12;
constexpr int32_t MAX_ROOK_PERMUTATIONS = 1 << MAX_ROOK_BLOCKERS;
constexpr int32_t MAX_BISHOP_BLOCKERS = 9;
constexpr int32_t MAX_BISHOP_PERMUTATIONS = 1 << MAX_BISHOP_BLOCKERS;

constexpr std::array<uint64_t, 64> BISHOP_MAGICS_CACHE{
        4616295173873598496ULL,
        9008308706935072ULL,
        2310347717482119297ULL,
        11265597217226816ULL,
        565218098806784ULL,
        2306143313595466752ULL,
        612772128644759680ULL,
        1166575244564038912ULL,
        148689226308845648ULL,
        9015999660818688ULL,
        864779162406159360ULL,
        414420296120929664ULL,
        9656282758917423104ULL,
        182442001006329856ULL,
        9223407501004783617ULL,
        36030059873720320ULL,
        4504012484386948ULL,
        709323143905808ULL,
        2252005974229268ULL,
        4611826790309052416ULL,
        9223952596192985152ULL,
        9225062038051522562ULL,
        220148633356535872ULL,
        140741976428577ULL,
        4757213015405757537ULL,
        4612957054406054912ULL,
        110351385043928209ULL,
        563500246632576ULL,
        2396935382237700ULL,
        2254007431090688ULL,
        291188169873884160ULL,
        709323143905808ULL,
        4684887122816009473ULL,
        6917674234110300164ULL,
        6192725439546120ULL,
        576462953474687105ULL,
        9800967493748006920ULL,
        4653152536825856ULL,
        289360691352088712ULL,
        577607543757620234ULL,
        1230054458277497088ULL,
        288938481504223874ULL,
        4900481569359397120ULL,
        4611687255780626688ULL,
        4512400082747456ULL,
        2323858524416770304ULL,
        145285070663549188ULL,
        2310347717482119297ULL,
        612772128644759680ULL,
        144163618161166368ULL,
        5224750204446048258ULL,
        4647785185297956896ULL,
        5791629194115551232ULL,
        105690597303296ULL,
        9015999660818688ULL,
        9008308706935072ULL,
        1166575244564038912ULL,
        36030059873720320ULL,
        704789168031744ULL,
        9223512774546589696ULL,
        11819697364841693700ULL,
        4504012484386948ULL,
        148689226308845648ULL,
        4616295173873598496ULL,
};

constexpr std::array<uint64_t, 64> ROOK_MAGICS_CACHE{
        756605012284543520ULL,
        9241386710510608392ULL,
        2341906990873214984ULL,
        1224996690967199748ULL,
        13979209048420122632ULL,
        2377901702830359048ULL,
        288239240972739617ULL,
        144116305338075204ULL,
        144818877734338568ULL,
        576742502174826624ULL,
        2468254208216073282ULL,
        2668664395928211460ULL,
        1450440576465371153ULL,
        1315332604832121856ULL,
        6931321310090100992ULL,
        721983317427095626ULL,
        150083408494624ULL,
        2408480757530624ULL,
        2816949061189696ULL,
        2454462896579547168ULL,
        576602039614703616ULL,
        2305984296524972544ULL,
        9011599601436752ULL,
        600333352993060ULL,
        1315121470675910656ULL,
        9223442409896050944ULL,
        17594341924864ULL,
        576478348784967937ULL,
        1450440576465371153ULL,
        288797726307516928ULL,
        2387470761058174980ULL,
        4657004597779394692ULL,
        2972446125518225792ULL,
        576742502174826624ULL,
        17594341924864ULL,
        2305860739945990656ULL,
        140771856483328ULL,
        288797726307516928ULL,
        2203385333776ULL,
        2954365755245723777ULL,
        3999196606548426752ULL,
        87961467109376ULL,
        2310346884255908096ULL,
        9800395876756226064ULL,
        4507997808132224ULL,
        1688884287176960ULL,
        27039331785638040ULL,
        281683421102082ULL,
        2972446125518225792ULL,
        9511745624398102912ULL,
        3458905320030470272ULL,
        144123986338382208ULL,
        4507997808132224ULL,
        1153202988307972352ULL,
        1688867057435136ULL,
        144255929860251776ULL,
        140746620014849ULL,
        1170945798991659137ULL,
        4828070044213772301ULL,
        9223939453590970377ULL,
        2307250418457117713ULL,
        1227794415482831106ULL,
        1688854289482754ULL,
        144115364178445314ULL,
};

namespace MagicBitBoards {

    auto get_bishop_moves(Square square, BitBoard blockers) noexcept -> BitBoard;

    auto get_rook_moves(Square square, BitBoard blockers) noexcept -> BitBoard;

    constexpr auto
    generate_blocker_board_permutation(const Square square, const BitBoard blocker_mask) noexcept -> BitBoard {

        BitBoard blocker_board = BitBoards::EMPTY;
        int32_t bit_index = 0;

        std::array<BitBoard, 64> index_checks{0};
        std::iota(index_checks.begin(), index_checks.end(), 0);
        std::for_each(index_checks.begin(), index_checks.end(),
                      [&blocker_board, &bit_index, square, blocker_mask](auto index) {

                          const BitBoard bit_board = (1ULL << index);
                          if (BitBoardUtils::do_bit_boards_overlap(blocker_mask, bit_board)) {
                              if (square & (1 << bit_index)) {
                                  blocker_board |= bit_board;
                              }
                              bit_index++;
                          }
                      });

        return blocker_board;
    }

    struct BishopMagics {
        std::array<BitBoard, Squares::NUM> blocker_masks{0};
        std::array<uint64_t, Squares::NUM> magics{0};
        std::array<std::array<BitBoard, MAX_BISHOP_PERMUTATIONS>, Squares::NUM> moves{{0}};
    };

    struct RookMagics {
        std::array<BitBoard, Squares::NUM> blocker_masks{0};
        std::array<uint64_t, Squares::NUM> magics{0};
        std::array<std::array<BitBoard, MAX_ROOK_PERMUTATIONS>, Squares::NUM> moves{{0}};
    };

    constexpr auto generate_bishop_magics() noexcept -> BishopMagics {

        BishopMagics bishop_magics;

        for (Square square = Squares::A1; square <= Squares::H8; square++) {

            // Build the blocker mask for the square
            const BitBoard bishop_bit_board = BitBoardUtils::square_to_bit_board(square);
            const BitBoard blocker_mask =
                    CapturesGeneration::get_bishop_capture_positions(bishop_bit_board, BitBoards::EMPTY) &
                    ~BitBoards::EDGES;

            // Iterate over all possible permutations of the blocker mask and calculate the possible moves
            const int32_t blocker_mask_bit_count = BitBoardUtils::get_count(blocker_mask);
            const int32_t num_of_permutations = 1ULL << blocker_mask_bit_count;
            std::array<BitBoard, MAX_BISHOP_PERMUTATIONS> bishop_blockers_permutations{};
            std::array<BitBoard, MAX_BISHOP_PERMUTATIONS> bishop_attacks{};
            for (int32_t i = 0; i < num_of_permutations; i++) {
                bishop_blockers_permutations[i] = generate_blocker_board_permutation(i, blocker_mask);
                bishop_attacks[i] = CapturesGeneration::get_bishop_capture_positions(bishop_bit_board,
                                                                                     bishop_blockers_permutations[i]);
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
                    const uint64_t magic_index =
                            (bishop_blockers_permutations[i] * magic) >> (Squares::NUM - blocker_mask_bit_count);

                    // If the magic index isn't used yet, set it
                    if (used_magics[magic_index] == 0ULL) {
                        used_magics[magic_index] = bishop_attacks[i];
                    }
                        // If the index is already used, see if it maps into the same moves
                    else if (used_magics[magic_index] != bishop_attacks[i]) {
                        magic_found = false;
                        break;
                    }
                }

                if (magic_found) {
                    bishop_magics.magics[square] = magic;
                    bishop_magics.blocker_masks[square] = blocker_mask;
                    // Set the moves table to map magic_index into the correct move
                    for (int32_t i = 0; i < num_of_permutations; i++) {
                        // Calculate the magic index into the move array
                        const uint64_t magic_index =
                                (bishop_blockers_permutations[i] * magic) >> (Squares::NUM - blocker_mask_bit_count);
                        bishop_magics.moves[square][magic_index] = bishop_attacks[i];
                    }
                    break;
                }
            }
        }

        return bishop_magics;
    }

    constexpr auto generate_bishop_magics_cached() noexcept -> BishopMagics {

        BishopMagics bishop_magics;

        for (Square square = Squares::A1; square <= Squares::H8; square++) {

            // Build the blocker mask for the square
            const BitBoard bishop_bit_board = BitBoardUtils::square_to_bit_board(square);
            const BitBoard blocker_mask =
                    CapturesGeneration::get_bishop_capture_positions(bishop_bit_board, BitBoards::EMPTY) &
                    ~BitBoards::EDGES;

            // Iterate over all possible permutations of the blocker mask and calculate the possible moves
            const int32_t blocker_mask_bit_count = BitBoardUtils::get_count(blocker_mask);
            const int32_t num_of_permutations = 1ULL << blocker_mask_bit_count;
            std::array<BitBoard, MAX_BISHOP_PERMUTATIONS> bishop_blockers_permutations{};
            std::array<BitBoard, MAX_BISHOP_PERMUTATIONS> bishop_attacks{};
            for (int32_t i = 0; i < num_of_permutations; i++) {
                bishop_blockers_permutations[i] = generate_blocker_board_permutation(i, blocker_mask);
                bishop_attacks[i] = CapturesGeneration::get_bishop_capture_positions(bishop_bit_board,
                                                                                     bishop_blockers_permutations[i]);
            }

            bishop_magics.magics[square] = BISHOP_MAGICS_CACHE[square];
            bishop_magics.blocker_masks[square] = blocker_mask;
            // Set the moves table to map magic_index into the correct move
            for (int32_t i = 0; i < num_of_permutations; i++) {
                // Calculate the magic index into the move array
                const uint64_t magic_index = (bishop_blockers_permutations[i] * BISHOP_MAGICS_CACHE[square])
                        >> (Squares::NUM - blocker_mask_bit_count);
                bishop_magics.moves[square][magic_index] = bishop_attacks[i];
            }
        }

        return bishop_magics;
    }

    constexpr auto generate_rook_magics() noexcept -> RookMagics {

        RookMagics rook_magics;

        for (Square square = Squares::A1; square <= Squares::H8; square++) {

            // Build the blocker mask for the square
            const BitBoard rook_bit_board = BitBoardUtils::square_to_bit_board(square);
            BitBoard blocker_mask = BitBoards::EMPTY;
            blocker_mask |= CapturesGeneration::get_captures_in_direction<1, 0>(rook_bit_board, BitBoards::EMPTY) &
                            ~BitBoards::ROW_8;
            blocker_mask |= CapturesGeneration::get_captures_in_direction<-1, 0>(rook_bit_board, BitBoards::EMPTY) &
                            ~BitBoards::ROW_1;
            blocker_mask |= CapturesGeneration::get_captures_in_direction<0, -1>(rook_bit_board, BitBoards::EMPTY) &
                            ~BitBoards::COL_A;
            blocker_mask |= CapturesGeneration::get_captures_in_direction<0, 1>(rook_bit_board, BitBoards::EMPTY) &
                            ~BitBoards::COL_H;

            // Iterate over all possible permutations of the blocker mask and calculate the possible moves
            const int32_t blocker_mask_bit_count = BitBoardUtils::get_count(blocker_mask);
            const int32_t num_of_permutations = 1ULL << blocker_mask_bit_count;
            std::array<BitBoard, MAX_ROOK_PERMUTATIONS> rook_blockers_permutations{};
            std::array<BitBoard, MAX_ROOK_PERMUTATIONS> rook_attacks{};
            for (int32_t i = 0; i < num_of_permutations; i++) {
                rook_blockers_permutations[i] = generate_blocker_board_permutation(i, blocker_mask);
                rook_attacks[i] = CapturesGeneration::get_rook_capture_positions(rook_bit_board,
                                                                                 rook_blockers_permutations[i]);
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
                    const uint64_t magic_index =
                            (rook_blockers_permutations[i] * magic) >> (Squares::NUM - blocker_mask_bit_count);

                    // If the magic index isn't used yet, set it
                    if (used_magics[magic_index] == 0ULL) {
                        used_magics[magic_index] = rook_attacks[i];
                    }
                        // If the index is already used, see if it maps into the same moves
                    else if (used_magics[magic_index] != rook_attacks[i]) {
                        magic_found = false;
                        break;
                    }
                }

                if (magic_found) {
                    rook_magics.magics[square] = magic;
                    rook_magics.blocker_masks[square] = blocker_mask;
                    // Set the moves table to map magic_index into the correct move
                    for (int32_t i = 0; i < num_of_permutations; i++) {
                        // Calculate the magic index into the move array
                        const uint64_t magic_index =
                                (rook_blockers_permutations[i] * magic) >> (Squares::NUM - blocker_mask_bit_count);
                        rook_magics.moves[square][magic_index] = rook_attacks[i];
                    }
                    break;
                }
            }
        }

        return rook_magics;
    }

    constexpr auto generate_rook_magics_cached() noexcept -> RookMagics {

        RookMagics rook_magics;

        for (Square square = Squares::A1; square <= Squares::H8; square++) {

            // Build the blocker mask for the square
            const BitBoard rook_bit_board = BitBoardUtils::square_to_bit_board(square);
            BitBoard blocker_mask = BitBoards::EMPTY;
            blocker_mask |= CapturesGeneration::get_captures_in_direction<1, 0>(rook_bit_board, BitBoards::EMPTY) &
                            ~BitBoards::ROW_8;
            blocker_mask |= CapturesGeneration::get_captures_in_direction<-1, 0>(rook_bit_board, BitBoards::EMPTY) &
                            ~BitBoards::ROW_1;
            blocker_mask |= CapturesGeneration::get_captures_in_direction<0, -1>(rook_bit_board, BitBoards::EMPTY) &
                            ~BitBoards::COL_A;
            blocker_mask |= CapturesGeneration::get_captures_in_direction<0, 1>(rook_bit_board, BitBoards::EMPTY) &
                            ~BitBoards::COL_H;

            // Iterate over all possible permutations of the blocker mask and calculate the possible moves
            const int32_t blocker_mask_bit_count = BitBoardUtils::get_count(blocker_mask);
            const int32_t num_of_permutations = 1ULL << blocker_mask_bit_count;
            std::array<BitBoard, MAX_ROOK_PERMUTATIONS> rook_blockers_permutations{};
            std::array<BitBoard, MAX_ROOK_PERMUTATIONS> rook_attacks{};
            for (int32_t i = 0; i < num_of_permutations; i++) {
                rook_blockers_permutations[i] = generate_blocker_board_permutation(i, blocker_mask);
                rook_attacks[i] = CapturesGeneration::get_rook_capture_positions(rook_bit_board,
                                                                                 rook_blockers_permutations[i]);
            }

            rook_magics.magics[square] = ROOK_MAGICS_CACHE[square];
            rook_magics.blocker_masks[square] = blocker_mask;
            // Set the moves table to map magic_index into the correct move
            for (int32_t i = 0; i < num_of_permutations; i++) {
                // Calculate the magic index into the move array
                const uint64_t magic_index = (rook_blockers_permutations[i] * ROOK_MAGICS_CACHE[square])
                        >> (Squares::NUM - blocker_mask_bit_count);
                rook_magics.moves[square][magic_index] = rook_attacks[i];
            }
        }

        return rook_magics;
    }

    /*
    * EMU: The following commented out code was tested to run at compile time.
    * Because the tables are so large, only the bishop table compiles at
    * a reasonable time. Implementing Fancy Bit Boards to reduce the size
    * might help, but it might not be worth it. It only takes a few
    * milliseconds to do this at runtime vs a few seconds during compile
    * time.
    constexpr BishopMagics BISHOP_MAGICS = generate_bishop_magics_cached();
    constexpr RookMagics ROOK_MAGICS = generate_rook_magics_cached();
    */

    extern const BishopMagics BISHOP_MAGICS;
    extern const RookMagics ROOK_MAGICS;
}