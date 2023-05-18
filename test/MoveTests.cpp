#include <cassert>

#include "../src/BitBoardUtils.hpp"
#include "../src/CommonHeaders.hpp"
#include "../src/Move.hpp"
#include "../src/TestFW.hpp"

namespace Tests {
auto add_move_tests(TestFW::UnitTest &unit_tests) -> void {
    TestFW::TestCase move_test_case("Move");
    move_test_case.tests.emplace_back("Move::get_source_square", []() {
        for (auto square = Squares::A1; square < Squares::NUM; square++) {
            Move move(square, Square(0));
            assert(square == move.get_source_square());
            assert(!move.is_castle());
            assert(!move.is_en_passantable());
            assert(!move.is_promotion());
        }
    });
    move_test_case.tests.emplace_back(
        TestFW::Test("Move::get_destination_square", []() {
            for (auto square = Squares::A1; square < Squares::NUM; square++) {
                Move move(Square(0), square);
                assert(square == move.get_destination_square());
                assert(!move.is_castle());
                assert(!move.is_en_passantable());
                assert(!move.is_promotion());
            }
        }));
    move_test_case.tests.emplace_back(TestFW::Test(
        "Move::get_source_square & Move::get_destination_square", []() {
            for (auto square = Squares::A1; square < Squares::NUM; square++) {
                auto source = 63 - square;
                auto dest = square;
                if (source == dest) {
                    continue;
                }
                Move move(source, dest);
                assert(source == move.get_source_square());
                assert(dest == move.get_destination_square());
                assert(!move.is_castle());
                assert(!move.is_en_passantable());
                assert(!move.is_promotion());
            }
        }));
    move_test_case.tests.emplace_back(TestFW::Test("Move::get_en_passant", []() {
        Move move(Square(1), Square(2));
        move.set_en_passant(3);
        TFW_ASSERT_EQ(true, move.is_en_passantable());
        TFW_ASSERT_EQ(3, move.get_en_passant());
    }));
    move_test_case.tests.emplace_back(TestFW::Test("Move::get_castle", []() {
        Move move(Square(1), Square(3));
        move.set_castle(Castles::WHITE_QUEEN);
        TFW_ASSERT_EQ(true, move.is_castle());
        TFW_ASSERT_EQ(Castles::WHITE_QUEEN.value(), move.get_castle().value());
    }));
    move_test_case.tests.emplace_back(TestFW::Test("Move::get_promotion", []() {
        Move move(Square(1), Square(4));
        TFW_ASSERT_EQ(false, move.is_promotion());
        move.set_promotion(PieceCodes::BISHOP);
        TFW_ASSERT_EQ(true, move.is_promotion());
        TFW_ASSERT_EQ(PieceCodes::BISHOP.value, move.get_promotion().value);
    }));

    move_test_case.tests.emplace_back(TestFW::Test("Move - Black Castle", []() {
        Move move(Squares::BLACK_KING_START, Squares::BLACK_KING_KING_CASTLE_END);
        move.set_castle(Castles::BLACK_KING);
        TFW_ASSERT_EQ(true, move.is_castle());
        TFW_ASSERT_EQ(Squares::BLACK_KING_START.value(),
                      Square::from_bit_board(move.get_source_bit_board()).value());
        TFW_ASSERT_EQ(Squares::BLACK_KING_KING_CASTLE_END.value(),
                      Square::from_bit_board(move.get_destination_bit_board()).value());

        move.set_destination_square(Squares::BLACK_KING_QUEEN_CASTLE_END);
        move.set_castle(Castles::BLACK_QUEEN);
        TFW_ASSERT_EQ(true, move.is_castle());
        TFW_ASSERT_EQ(Squares::BLACK_KING_START.value(),
                      Square::from_bit_board(move.get_source_bit_board()).value());
        TFW_ASSERT_EQ(Squares::BLACK_KING_QUEEN_CASTLE_END.value(),
                      Square::from_bit_board(move.get_destination_bit_board()).value());
    }));
    unit_tests.test_cases.push_back(move_test_case);
}
}  // namespace Tests