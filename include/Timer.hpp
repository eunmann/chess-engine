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
     * @param message A m_message to include when printing the elapsed time
     */
    explicit Timer(std::string message) noexcept;

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
    [[nodiscard]] auto get_time_elapsed() const noexcept -> int64_t;

    /**
     * Prints the elapsed time with a m_message if given in the constructor
     */
    auto print() const noexcept -> void;

    /**
     * Prints the elapsed time with the inputted m_message
     *
     * @param message The m_message to print along with the elapsed time
     */
    auto print(const std::string &message) const noexcept -> void;

    /**
     * Returns the time as a formated string(value with unit prefix)
     */
    [[nodiscard]] auto time_string() const noexcept -> std::string;

private:
    std::string m_message;
    std::chrono::high_resolution_clock::time_point m_start_timestamp;
    std::chrono::high_resolution_clock::time_point m_end_timestamp;
};