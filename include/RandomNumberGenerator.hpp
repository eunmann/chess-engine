#pragma once

#include <inttypes.h>

class RandomNumberGenerator {
  public:
  RandomNumberGenerator(const uint64_t seed) noexcept;

  auto next_random() noexcept -> uint64_t;

  private:
  uint64_t seed;
};