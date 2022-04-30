
#pragma once

#include <cinttypes>

class BitMaskValue {
public:
    BitMaskValue() noexcept;

    explicit BitMaskValue(int32_t value) noexcept;

    template<const int32_t mask, const int32_t shift>
    auto set_bits(const int32_t value) noexcept -> void {
        this->m_value = (this->m_value & ~(mask << shift)) | (value & mask) << shift;
    }

    template<const int32_t mask, const int32_t shift>
    [[nodiscard]] auto get_bits() const noexcept -> int32_t {
        return (this->m_value >> shift) & mask;
    }

    auto operator==(const BitMaskValue &bit_mask_value) const noexcept -> bool;

private:
    int32_t m_value;
};