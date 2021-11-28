#pragma once

#include <chrono>
#include <string>

/**
 * Timer is a utility object to record and print elapsed times.
 */
class Timer {
  public:
  /**
   * Creates the timer and starts the timer
   */
  Timer() noexcept;

  /**
   * Creates the timer and starts the timer.
   *
   * @param message A message to include when printing the elapsed time
   */
  Timer(std::string message) noexcept;

  /**
   * Start the timer. If the timer has already started, the timer
   * will overwrite the starting time.
   */
  auto start() noexcept -> void;

  /**
   * Stop the timer.
   */
  auto end() noexcept -> void;

  /**
   * @return The number of nanoseconds that have elapsed
   */
  auto get_time_elapsed() const noexcept -> int64_t;

  /**
   * Prints the elapsed time with a message if given in the constructor
   */
  auto print() const noexcept -> void;

  /**
   * Prints the elapsed time with the inputted message
   *
   * @param message The message to print along with the elapsed time
   */
  auto print(const std::string& message) const noexcept -> void;

  /**
   * Returns the time as a formated string(value with unit prefix)
   */
  auto time_string() const noexcept ->std::string;

  private:
  std::string message;
  std::chrono::steady_clock::time_point s;
  std::chrono::steady_clock::time_point e;
};