module;

export module concurrency.pool.coroutine:scheduler;

import std.compat;
import concurrency.queues;

export namespace concurrency::pool::coroutine {

enum class SchedulePolicy : uint8_t {
  Inline,
  Enqueue,
};

inline thread_local bool isPoolWorker = false;

struct Scheduler {
private:
  queues::TaskQueue &queue_;

  SchedulePolicy schedulePolicy_;

public:
  explicit Scheduler(queues::TaskQueue &queue, SchedulePolicy schedulePolicy)
      : queue_(queue), schedulePolicy_(schedulePolicy) {};

  bool await_ready() noexcept {
    if (!isPoolWorker) {
      return false;
    }
    switch (schedulePolicy_) {
    case SchedulePolicy::Inline:
      return true;
    case SchedulePolicy::Enqueue:
    default:
      return false;
    }
  };

  void await_suspend(std::coroutine_handle<> h) {
    queue_.push([h]() mutable { h.resume(); });
  };

  void await_resume() noexcept {};
};

} // namespace concurrency::pool::coroutine
