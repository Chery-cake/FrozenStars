module;

export module concurrency.pool.coroutine:scheduler;

import std.compat;
import concurrency.queues;

export namespace concurrency::pool::coroutine {

struct Scheduler {
private:
  concurrency::queues::TaskQueue &queue_;

public:
  explicit Scheduler(concurrency::queues::TaskQueue &queue) : queue_(queue) {};
  bool await_ready() const noexcept {
    return false;
  }; // TODO handle both treu and false
  void await_suspend(std::coroutine_handle<> h) {
    queue_.push([h]() mutable { h.resume(); });
  };
  void await_resume() noexcept {};
};

} // namespace concurrency::pool::coroutine
