#pragma once

#include <vector>
#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <future>
#include <cinttypes>

struct SearchResult {
    uint64_t moves_counter = 0;
    uint64_t leaf_nodes_counter = 0;
    uint64_t times_pruned_counter = 0;
    int32_t heuristic = 0;
    bool is_legal = false;

    auto wait() noexcept -> void {
        std::unique_lock<std::mutex> lock(this->m_mutex);
        if (!this->is_complete) {
            m_cv.wait(lock);
        }
    }

    auto notify() {
        {
            std::unique_lock<std::mutex> lock(this->m_mutex);
            this->is_complete = true;
        }
        this->m_cv.notify_all();
    }

private:
    std::mutex m_mutex;
    std::condition_variable m_cv;
    bool is_complete = false;
};

class SearchThreadPool {

public:
    explicit SearchThreadPool(size_t number_of_threads);

    [[nodiscard]] auto get_number_of_threads() const noexcept -> size_t;

    auto enqueue(const std::function<void()> &task) noexcept -> void;

    auto stop() noexcept -> void;

private:

    auto work() noexcept -> void;

    auto wait_for_task() noexcept -> std::function<void()>;

    std::vector<std::thread> threads;
    std::vector<std::function<void()>> taskQueue;
    std::mutex queueMutex;
    std::condition_variable queueWait;
    bool run_threads = true;
};
