#include "SearchThreadPool.hpp"

SearchThreadPool::SearchThreadPool(const size_t number_of_threads) {
  this->taskQueue.reserve(128);
  this->threads.reserve(number_of_threads);
  for (int i = 0; i < number_of_threads; i++) {
    this->threads.emplace_back(&SearchThreadPool::work, this);
  }
}

auto SearchThreadPool::get_number_of_threads() const noexcept -> size_t {
  return this->threads.size();
}

auto SearchThreadPool::enqueue(const std::function<void()> task) noexcept -> void {
  {
    std::unique_lock lock(this->queueMutex);
    this->taskQueue.emplace_back(task);
  }
  this->queueWait.notify_one();
}

auto SearchThreadPool::stop() noexcept -> void {
  this->run_threads = false;
}

auto SearchThreadPool::work() noexcept -> void {
  while (this->run_threads) {
    auto task = this->wait_for_task();
    task();
  }
}

auto SearchThreadPool::wait_for_task() noexcept -> std::function<void()> {
  std::unique_lock lock(this->queueMutex);
  this->queueWait.wait(lock, [this] {return !this->taskQueue.empty(); });
  auto task = this->taskQueue[this->taskQueue.size() - 1];
  this->taskQueue.pop_back();
  return task;
}