module;

export module concurrency.queues:queue;

import std.compat;

export namespace concurrency::queues {

using Task = std::move_only_function<void()>;

struct TaskQueue {
    virtual ~TaskQueue() = default;
    virtual void push(Task t) = 0;
    virtual bool try_pop(Task &t) = 0;
    virtual void notify_all() = 0;
};

} // namespace concurrency::queues
