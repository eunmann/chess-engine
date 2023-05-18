#pragma once

#include <cinttypes>

class RandomNumberGenerator {
public:
    constexpr explicit RandomNumberGenerator(const uint64_t seed) noexcept: seed(seed) {

    }

    constexpr auto next_random() noexcept -> uint64_t {
        this->seed ^= this->seed >> 12;
        this->seed ^= this->seed << 25;
        this->seed ^= this->seed >> 27;
        return seed * 2685821657736338717LL;
    }

private:
    uint64_t seed;
};