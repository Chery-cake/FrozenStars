module;

export module concurrency.pool.coroutine:task_void;

import std.compat;
import concurrency.pool.coroutine.policy;
import :task;

export namespace concurrency::pool::coroutine {

template <policy::Suspend SP> class coroutineTask<SP, void> {
public:
  struct promise_type;
  using handle_type = std::coroutine_handle<promise_type>;

private:
  handle_type handle_{};

public:
  explicit coroutineTask(handle_type handle) noexcept : handle_(handle) {}

  coroutineTask(const coroutineTask &) = delete;
  coroutineTask &operator=(const coroutineTask &) = delete;

  coroutineTask(coroutineTask &&other) noexcept
      : handle_(std::exchange(other.handle_, {})) {}

  coroutineTask &operator=(coroutineTask &&other) noexcept {
    if (this != &other) {
      if (handle_) {
        handle_.destroy();
      }
      handle_ = std::exchange(other.handle_, {});
    }
    return *this;
  }

  ~coroutineTask() {
    if (handle_) {
      handle_.destroy();
    }
  }

  [[nodiscard]] bool valid() const noexcept {
    return static_cast<bool>(handle_);
  }
  [[nodiscard]] bool done() const noexcept {
    return !handle_ || handle_.done();
  }

  void start() {
    if (handle_ && !handle_.done()) {
      handle_.resume();
    }
  }

  void get() {
    if (handle_ && !handle_.done()) {
      handle_.resume();
    }

    auto &promise = handle_.promise();
    promise.doneSignal.wait();

    if (promise.exception) {
      std::rethrow_exception(promise.exception);
    }
  }

  struct awaiter {
    handle_type handle_;

    explicit awaiter(handle_type h) noexcept : handle_(h) {}

    awaiter(const awaiter &) = delete;
    awaiter &operator=(const awaiter &) = delete;

    awaiter(awaiter &&other) noexcept
        : handle_(std::exchange(other.handle_, {})) {}

    ~awaiter() {
      if (handle_) {
        handle_.destroy();
      }
    }

    [[nodiscard]] bool await_ready() const noexcept {
      return !handle_ || handle_.done();
    }

    std::coroutine_handle<>
    await_suspend(std::coroutine_handle<> awaiting) noexcept {
      auto &p = handle_.promise();
      p.continuation = awaiting;
      return handle_;
    }

    void await_resume() {
      auto &p = handle_.promise();
      if (p.exception) {
        std::rethrow_exception(p.exception);
      }

      handle_.destroy();
      handle_ = {};
    }
  };

  auto operator co_await() & noexcept { return awaiter{handle_}; }

  auto operator co_await() && noexcept { return awaiter{handle_}; }

  struct promise_type {
    std::exception_ptr exception;
    std::coroutine_handle<> continuation = nullptr;
    std::latch doneSignal{1};

    coroutineTask get_return_object() noexcept {
      return coroutineTask{handle_type::from_promise(*this)};
    }

    constexpr auto initial_suspend() noexcept {
      if constexpr (SP == policy::Suspend::Always) {
        return std::suspend_always{};
      }
      if constexpr (SP == policy::Suspend::Never) {
        return std::suspend_never{};
      }
    };

    auto final_suspend() noexcept {
      struct final_awaiter {
        bool await_ready() noexcept { return false; }

        std::coroutine_handle<> await_suspend(handle_type h) noexcept {
          auto &p = h.promise();
          p.doneSignal.count_down();

          if (p.continuation) {
            return p.continuation;
          }

          return std::noop_coroutine();
        }

        void await_resume() noexcept {}
      };
      return final_awaiter{};
    }

    void return_void() noexcept {}

    void unhandled_exception() noexcept {
      exception = std::current_exception();
    }
  };
};

} // namespace concurrency::pool::coroutine
