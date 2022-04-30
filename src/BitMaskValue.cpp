#include "BitMaskValue.hpp"

BitMaskValue::BitMaskValue() noexcept: m_value(0) {
}

BitMaskValue::BitMaskValue(int32_t value) noexcept: m_value(value) {
}

auto BitMaskValue::operator==(const BitMaskValue &bit_mask_value) const noexcept -> bool {
    return this->m_value == bit_mask_value.m_value;
}
