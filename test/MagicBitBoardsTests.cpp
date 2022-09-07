#include "TestFW.hpp"

#include "CommonHeaders.hpp"
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
                TFW_ASSERT_EQ(expected_bishop_moves.value(), magic_bishop_moves.value());
            };

            position.add(PieceCodes::BISHOP, Colors::WHITE, Squares::E4.to_bit_board());
            assert_positions();

            position.add(PieceCodes::PAWN, Colors::BLACK, Squares::D3.to_bit_board());
            assert_positions();

            position.add(PieceCodes::PAWN, Colors::BLACK, Squares::D5.to_bit_board());
            assert_positions();

            position.add(PieceCodes::PAWN, Colors::BLACK, Squares::F3.to_bit_board());
            assert_positions();

            position.add(PieceCodes::PAWN, Colors::BLACK, Squares::F5.to_bit_board());
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
                TFW_ASSERT_EQ(expected_rook_moves.value(), magic_rook_moves.value());
            };

            position.add(PieceCodes::ROOK, Colors::WHITE, Squares::E4.to_bit_board());
            assert_positions();

            position.add(PieceCodes::PAWN, Colors::BLACK, Squares::E5.to_bit_board());
            assert_positions();

            position.add(PieceCodes::PAWN, Colors::BLACK, Squares::D4.to_bit_board());
            assert_positions();

            position.add(PieceCodes::PAWN, Colors::BLACK, Squares::F4.to_bit_board());
            assert_positions();

            position.add(PieceCodes::PAWN, Colors::BLACK, Squares::E3.to_bit_board());
            assert_positions();

        }));
        unit_tests.test_cases.push_back(magic_bit_boards);
    }
}