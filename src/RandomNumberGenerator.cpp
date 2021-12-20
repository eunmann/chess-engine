#include "RandomNumberGenerator.hpp"

RandomNumberGenerator::RandomNumberGenerator(const uint64_t seed) noexcept : seed(seed) {
}

auto RandomNumberGenerator::next_random() noexcept -> uint64_t {
  this->seed ^= this->seed >> 12;
  this->seed ^= this->seed << 25;
  this->seed ^= this->seed >> 27;
  return seed * 2685821657736338717LL;
}