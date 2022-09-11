
#pragma once

#include <cinttypes>

class BitMaskValue {
public:
    constexpr BitMaskValue() noexcept: m_value(0) {}

    constexpr explicit BitMaskValue(int32_t value) noexcept: m_value(value) {}

    template<const int32_t mask, const int32_t shift>
    constexpr auto set_bits(const int32_t value) noexcept -> void {
        this->m_value = (this->m_value & ~(mask << shift)) | (value & mask) << shift;
    }

    template<const int32_t mask, const int32_t shift>
    [[nodiscard]] constexpr auto get_bits() const noexcept -> int32_t {
        return (this->m_value >> shift) & mask;
    }

    constexpr auto operator==(const BitMaskValue &bit_mask_value) const noexcept -> bool {
        return this->m_value == bit_mask_value.m_value;
    }

private:
    int32_t m_value;
};