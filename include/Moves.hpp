#pragma once

#include "Move.hpp"

class Moves {
public:
    Moves() noexcept;

    auto push_back(Move move) noexcept -> void;

    [[nodiscard]] auto size() const noexcept -> std::size_t;

    [[nodiscard]] auto begin() const noexcept -> std::array<Move, MAX_NUM_MOVES>::const_iterator;

    [[nodiscard]] auto end() const noexcept -> std::array<Move, MAX_NUM_MOVES>::const_iterator;

    auto operator[](std::size_t index) -> Move &;

private:
    std::array<Move, MAX_NUM_MOVES> m_moves_array;
    std::size_t m_index;
};