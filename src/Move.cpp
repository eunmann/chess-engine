#include "Move.hpp"

#include <assert.h>

Moves::Moves() : m_moves_array(),
                 m_index(0) {
}

int32_t Moves::size() {
    return this->m_index;
}

Move& Moves::back_ref() {
    assert(this->m_index < this->m_moves_array.size());

    return this->m_moves_array[this->m_index++];
}

Move& Moves::operator[](int32_t index) {
    assert(index >= 0 && index <= this->m_moves_array.size());
    return this->m_moves_array[index];
}