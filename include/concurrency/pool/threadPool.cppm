module;

#include <cstddef>

export module concurrency.pool:threadPool;

import std.compat;
import concurrency.queues;
import concurrency.pool.coroutine;

export namespace concurrency::pool {

struct Pool {
  std::string name;
  concurrency::queues::QueueKind queueKind =
      concurrency::queues::QueueKind::FIFO;

  bool operator<=>(const Pool &) const = default;
};

class ThreadPool {
private:
  static void worker_loop(const std::stop_token &stoken,
                          concurrency::queues::TaskQueue &queue);

  std::unique_ptr<concurrency::queues::TaskQueue> queue_;
  std::vector<std::jthread> threads_;
  std::stop_source stop_source_;

  std::mutex mutex_;

public:
  ThreadPool(const Pool &pool, size_t threads = 0);
  ~ThreadPool();

  ThreadPool(const ThreadPool &) = delete;
  ThreadPool &operator=(const ThreadPool &) = delete;
  ThreadPool(ThreadPool &&) = delete;
  ThreadPool &operator=(ThreadPool &&) = delete;

  template <typename F, typename... Args>
  std::future<std::invoke_result_t<F, Args...>> submit(F &&f, Args &&...args);

  coroutine::Scheduler schedule() noexcept;

  void resize(size_t new_size);
  [[nodiscard]] size_t size() const noexcept { return threads_.size(); }
};

} // namespace concurrency::pool
