#include "Position.hpp"

#include "GameUtils.hpp"
#include "CachedCapturesGeneration.hpp"

Position::Position() noexcept {
    this->clear();
}

auto Position::init() noexcept -> void {

    // Pawns
    this->piece_positions[PieceCodes::PAWN.value] = BitBoards::ROW_2;
    this->piece_positions[PieceCodes::PAWN.value] |= BitBoards::ROW_7;

    // Knights
    this->piece_positions[PieceCodes::KNIGHT.value] = BitBoard(0b1ULL).shift<0, 1>();
    this->piece_positions[PieceCodes::KNIGHT.value] |= BitBoard(0b1ULL).shift<0, 6>();
    this->piece_positions[PieceCodes::KNIGHT.value] |= BitBoard(0b1ULL).shift<7, 1>();
    this->piece_positions[PieceCodes::KNIGHT.value] |= BitBoard(0b1ULL).shift<7, 6>();

    // Bishops
    this->piece_positions[PieceCodes::BISHOP.value] = BitBoard(0b1ULL).shift<0, 2>();
    this->piece_positions[PieceCodes::BISHOP.value] |= BitBoard(0b1ULL).shift<0, 5>();
    this->piece_positions[PieceCodes::BISHOP.value] |= BitBoard(0b1ULL).shift<7, 2>();
    this->piece_positions[PieceCodes::BISHOP.value] |= BitBoard(0b1ULL).shift<7, 5>();

    // Rooks
    this->piece_positions[PieceCodes::ROOK.value] = BitBoard(0b1ULL).shift<0, 0>();
    this->piece_positions[PieceCodes::ROOK.value] |= BitBoard(0b1ULL).shift<0, 7>();
    this->piece_positions[PieceCodes::ROOK.value] |= BitBoard(0b1ULL).shift<7, 0>();
    this->piece_positions[PieceCodes::ROOK.value] |= BitBoard(0b1ULL).shift<7, 7>();

    // Queens
    this->piece_positions[PieceCodes::QUEEN.value] = BitBoard(0b1ULL).shift<0, 3>();
    this->piece_positions[PieceCodes::QUEEN.value] |= BitBoard(0b1ULL).shift<7, 3>();

    // Kings
    this->piece_positions[PieceCodes::KING.value] = BitBoard(0b1ULL).shift<0, 4>();
    this->piece_positions[PieceCodes::KING.value] |= BitBoard(0b1ULL).shift<7, 4>();

    // Color Position
    this->color_positions[Colors::WHITE.value] = BitBoards::ROW_1 | BitBoards::ROW_2;
    this->color_positions[Colors::BLACK.value] = BitBoards::ROW_7 | BitBoards::ROW_8;

    this->recompute_threaten();
}

auto Position::clear() noexcept -> void {
    for (auto &piece_bit_board: this->piece_positions) {
        piece_bit_board = BitBoards::EMPTY;
    }

    for (auto &color_bit_board: this->color_positions) {
        color_bit_board = BitBoards::EMPTY;
    }

    for (auto &threaten_bit_boards: this->threaten_positions) {
        threaten_bit_boards = BitBoards::EMPTY;
    }
}

auto Position::get_piece_bit_board(const PieceCode piece_code) const noexcept -> BitBoard {
    return this->piece_positions[piece_code.value];
}

auto Position::get_color_bit_board(const Color color) const noexcept -> BitBoard {
    return this->color_positions[color.value];
}

auto Position::get_piece_color_bit_board(const PieceCode piece_code,
                                         const Color color) const noexcept -> BitBoard {
    return this->get_piece_bit_board(piece_code) &
           this->get_color_bit_board(color);
}

auto Position::get_occupied_bit_board() const noexcept -> BitBoard {
    return this->get_color_bit_board<Colors::WHITE>() |
           this->get_color_bit_board<Colors::BLACK>();
}

auto Position::get_empty_bit_board() const noexcept -> BitBoard {
    return this->get_occupied_bit_board().invert();
}

auto Position::get_white_bit_board() const noexcept -> BitBoard {
    return this->get_color_bit_board<Colors::WHITE>();
}

auto Position::get_black_bit_board() const noexcept -> BitBoard {
    return this->get_color_bit_board<Colors::BLACK>();
}

auto Position::get_white_threaten() const noexcept -> BitBoard {
    return this->get_threaten<Colors::WHITE>();
}

auto Position::get_black_threaten() const noexcept -> BitBoard {
    return this->get_threaten<Colors::BLACK>();
}

auto Position::get_color(const BitBoard bit_board) const noexcept -> Color {
    if (this->is_white_occupied(bit_board)) {
        return Colors::WHITE;
    } else if (this->is_black_occupied(bit_board)) {
        return Colors::BLACK;
    } else {
        return Colors::NUM;
    }
}

auto Position::get_piece_type(const BitBoard bit_board) const noexcept -> PieceCode {
    for (PieceCode p = PieceCodes::PAWN; p < PieceCodes::NUM; p++) {
        if (this->piece_positions[p.value].overlaps(bit_board)) {
            return p;
        }
    }

    return PieceCodes::NUM;
}

auto Position::clear(const BitBoard bit_board) noexcept -> void {
    const BitBoard negated_bit_board = bit_board.invert();

    for (PieceCode pc = PieceCodes::PAWN; pc < PieceCodes::NUM; pc++) {
        this->piece_positions[pc.value] &= negated_bit_board;
    }

    this->color_positions[Colors::WHITE.value] &= negated_bit_board;
    this->color_positions[Colors::BLACK.value] &= negated_bit_board;
}

auto Position::add(const PieceCode piece_code, const Color color, const BitBoard bit_board) noexcept -> void {
    this->piece_positions[piece_code.value] |= bit_board;
    this->color_positions[color.value] |= bit_board;
}

auto Position::remove(const PieceCode piece_code, const Color color, const BitBoard bit_board) noexcept -> void {
    const BitBoard negated_bit_board = bit_board.invert();
    this->piece_positions[piece_code.value] &= negated_bit_board;
    this->color_positions[color.value] &= negated_bit_board;
}

auto Position::recompute_threaten() noexcept -> void {
    this->threaten_positions[Colors::WHITE.value] = CachedCapturesGeneration::get_capture_positions<Colors::WHITE>(
            *this);
    this->threaten_positions[Colors::BLACK.value] = CachedCapturesGeneration::get_capture_positions<Colors::BLACK>(
            *this);
}

auto Position::is_empty(const BitBoard bit_board) const noexcept -> bool {
    return !this->is_occupied(bit_board);
}

auto Position::is_occupied(const BitBoard bit_board) const noexcept -> bool {
    return this->get_occupied_bit_board().overlaps(bit_board);
}

auto Position::is_white_occupied(const BitBoard bit_board) const noexcept -> bool {
    return this->is_color_occupied<Colors::WHITE>(bit_board);
}

auto Position::is_black_occupied(const BitBoard bit_board) const noexcept -> bool {
    return this->is_color_occupied<Colors::BLACK>(bit_board);
}

auto Position::is_white_threaten(const BitBoard bit_board) const noexcept -> bool {
    return this->is_threaten<Colors::WHITE>(bit_board);
}

auto Position::is_black_threaten(const BitBoard bit_board) const noexcept -> bool {
    return this->is_threaten<Colors::BLACK>(bit_board);
}

auto Position::to_board() const noexcept -> Board {
    Board board;

    for (auto piece_code = PieceCodes::PAWN; piece_code < PieceCodes::NUM; piece_code++) {
        const int32_t board_value = BoardValues::ALL[piece_code.value];

        const BitBoard piece_bit_board = this->get_piece_bit_board(piece_code);

        BitBoardUtils::for_each_set_square(piece_bit_board, [this, board_value, &board](auto square) {
            BitBoard bit_board = square.to_bit_board();
            if (this->is_white_occupied(bit_board)) {
                board.positions[square.value()] = board_value;
            } else {
                board.positions[square.value()] = -1 * board_value;
            }
        });
    }

    return board;
}

auto Position::is_threaten(const BitBoard bit_board, const Color color) const noexcept -> bool {
    return this->threaten_positions[color.value].overlaps(bit_board);
}