#include "TestFW.hpp"

#include "Definitions.hpp"
#include "BitBoardUtils.hpp"
#include "Moves.hpp"
#include <cassert>
#include "GameState.hpp"
#include <unordered_set>
#include "MoveSearch.hpp"
#include "CachedCapturesGeneration.hpp"

namespace Tests {
    auto add_magic_bit_boards_tests(TestFW::UnitTest &unit_tests) -> void {
        TestFW::TestCase magic_bit_boards("Magic Bit Boards");
        magic_bit_boards.tests.emplace_back(TestFW::Test("MagicBitBoards::get_bishop_moves()", []() {

            Position position;

            auto assert_positions = [&position]() {
                const BitBoard expected_bishop_moves = CapturesGeneration::get_bishop_capture_positions(
                        position.get_piece_color_bit_board<PieceCodes::BISHOP, Colors::WHITE>(),
                        position.get_occupied_bit_board());
                const BitBoard magic_bishop_moves = CachedCapturesGeneration::get_cached_bishop_capture_positions<Colors::WHITE>(
                        position);

                if (expected_bishop_moves != magic_bishop_moves) {
                    position.to_board().print();
                    position.clear();
                    position.add(PieceCodes::BISHOP, Colors::WHITE, expected_bishop_moves);
                    position.to_board().print();
                    position.clear();
                    position.add(PieceCodes::BISHOP, Colors::WHITE, magic_bishop_moves);
                    position.to_board().print();
                }
                TFW_ASSERT_EQ(expected_bishop_moves, magic_bishop_moves);
            };

            position.add(PieceCodes::BISHOP, Colors::WHITE, BitBoardUtils::square_to_bit_board(Squares::E4));
            assert_positions();

            position.add(PieceCodes::PAWN, Colors::BLACK, BitBoardUtils::square_to_bit_board(Squares::D3));
            assert_positions();

            position.add(PieceCodes::PAWN, Colors::BLACK, BitBoardUtils::square_to_bit_board(Squares::D5));
            assert_positions();

            position.add(PieceCodes::PAWN, Colors::BLACK, BitBoardUtils::square_to_bit_board(Squares::F3));
            assert_positions();

            position.add(PieceCodes::PAWN, Colors::BLACK, BitBoardUtils::square_to_bit_board(Squares::F5));
            assert_positions();

        }));

        magic_bit_boards.tests.emplace_back(TestFW::Test("MagicBitBoards::get_rook_moves()", []() {

            Position position;
            auto assert_positions = [&position]() {
                const BitBoard expected_rook_moves = CapturesGeneration::get_rook_capture_positions(
                        position.get_piece_color_bit_board<PieceCodes::ROOK, Colors::WHITE>(),
                        position.get_occupied_bit_board());
                const BitBoard magic_rook_moves = CachedCapturesGeneration::get_cached_rook_capture_positions<Colors::WHITE>(
                        position);
                TFW_ASSERT_EQ(expected_rook_moves, magic_rook_moves);
            };

            position.add(PieceCodes::ROOK, Colors::WHITE, BitBoardUtils::square_to_bit_board(Squares::E4));
            assert_positions();

            position.add(PieceCodes::PAWN, Colors::BLACK, BitBoardUtils::square_to_bit_board(Squares::E5));
            assert_positions();

            position.add(PieceCodes::PAWN, Colors::BLACK, BitBoardUtils::square_to_bit_board(Squares::D4));
            assert_positions();

            position.add(PieceCodes::PAWN, Colors::BLACK, BitBoardUtils::square_to_bit_board(Squares::F4));
            assert_positions();

            position.add(PieceCodes::PAWN, Colors::BLACK, BitBoardUtils::square_to_bit_board(Squares::E3));
            assert_positions();

        }));
        unit_tests.test_cases.push_back(magic_bit_boards);
    }
}