module;

export module concurrency.pool:threadPool_impl;

import std.compat;
import :threadPool;
import concurrency.queues;
import concurrency.pool.coroutine;

export namespace concurrency::pool {

inline ThreadPool::ThreadPool(const Pool &pool, size_t num_threads) {
  switch (pool.queueKind) {
  default:
  case concurrency::queues::QueueKind::FIFO:
    queue_ = std::make_unique<concurrency::queues::FifoTaskQueue>();
    break;
  }

  size_t threads =
      (num_threads == 0) ? std::thread::hardware_concurrency() : num_threads;

  threads_.reserve(threads);

  std::ranges::for_each(
      std::views::iota(threads, 0U),
      [&threads = threads_, &queue = queue_](size_t) {
        threads.emplace_back(
            [&queue](const std::stop_token &st) { worker_loop(st, *queue); });
      });
}

inline ThreadPool::~ThreadPool() {
  stop_source_.request_stop();
  if (queue_) {
    queue_->notify_all();
  }
}

inline void ThreadPool::worker_loop(const std::stop_token &stoken,
                                    concurrency::queues::TaskQueue &queue) {
  concurrency::queues::Task task;
  while (!stoken.stop_requested() && queue.try_pop(task)) {
    if (task) {
      task();
    }
  }
}

template <typename F, typename... Args>
std::future<std::invoke_result_t<F, Args...>>
ThreadPool::submit(F &&f, Args &&...args) {
  using Ret = std::invoke_result_t<F, Args...>;

  auto task = std::make_shared<std::packaged_task<Ret()>>(
      std::bind(std::forward<F>(f), std::forward<Args>(args)...));

  std::future<Ret> fut = task->get_future();
  queue_->push([task]() { (*task)(); });
  return fut;
}

inline coroutine::Scheduler ThreadPool::schedule() noexcept {
  return coroutine::Scheduler{*queue_};
}

inline void ThreadPool::resize(size_t new_size) {
  std::unique_lock lock(mutex_);
  size_t current = threads_.size();
  if (new_size >= current) {

    std::ranges::for_each(
        std::views::iota(current, new_size),
        [&threads = threads_, &queue = queue_](size_t) {
          threads.emplace_back(
              [&queue](const std::stop_token &st) { worker_loop(st, *queue); });
        });

  } else {
    stop_source_.request_stop();
    queue_->notify_all();
    std::ranges::for_each(threads_, [](std::jthread &j) { j.join(); });
    threads_.clear();
    stop_source_ = std::stop_source{};

    std::ranges::for_each(
        std::views::iota(new_size, 0U),
        [&threads = threads_, &queue = queue_](size_t) {
          threads.emplace_back(
              [&queue](const std::stop_token &st) { worker_loop(st, *queue); });
        });
  }
}

} // namespace concurrency::pool
