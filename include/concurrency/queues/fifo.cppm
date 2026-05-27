module;

export module concurrency.queues:fifo;

import std.compat;
import :queue;

export namespace concurrency::queues {

struct FifoTaskQueue : public TaskQueue {
private:
  std::queue<Task> queue_;
  std::mutex mutex_;
  std::condition_variable cv_;
  std::atomic<bool> stop_{false};

public:
  void push(Task t) override {
    {
      std::scoped_lock lock(mutex_);
      queue_.push(std::move(t));
    }
    cv_.notify_one();
  }

  bool try_pop(Task &t) override {
    std::unique_lock lock(mutex_);
    cv_.wait(lock, [&queue = queue_, &stop = stop_] {
      return !queue.empty() || stop.load();
    });
    if (queue_.empty()) {
      return false;
    }
    t = std::move(queue_.front());
    queue_.pop();
    return true;
  }

  void notify_all() override {
    stop_.store(true);
    cv_.notify_all();
  }
};

} // namespace concurrency::queues
