#include "Move.hpp"

Moves::Moves() : m_moves_array(),
                 m_size(0) {
}

int32_t Moves::size() {
    return this->m_size;
}

Move& Moves::back_ref() {
    return this->m_moves_array[this->m_size++];
}

Move& Moves::operator[](int32_t index) {
    return this->m_moves_array[index];
}