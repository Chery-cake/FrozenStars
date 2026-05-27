module;

export module concurrency.pool.coroutine:scheduler;

import std.compat;
import concurrency.queues;

export namespace concurrency::pool::coroutine {

inline thread_local bool isPoolWorker = false;

struct Scheduler {
private:
  queues::TaskQueue &queue_;

public:
  explicit Scheduler(queues::TaskQueue &queue) : queue_(queue) {};

  static bool await_ready() noexcept { return isPoolWorker; };

  void await_suspend(std::coroutine_handle<> h) {
    queue_.push([h]() mutable { h.resume(); });
  };

  void await_resume() noexcept {};
};

} // namespace concurrency::pool::coroutine
