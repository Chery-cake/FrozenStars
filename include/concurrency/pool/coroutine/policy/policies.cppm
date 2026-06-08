module;

export module concurrency.pool.coroutine.policy:policies;

import std.compat;

export namespace concurrency::pool::coroutine::policy {

enum class Queue : uint8_t {
  Inline,
  Enqueue,
};

enum class Suspend : uint8_t {
  Always,
  Never,
};

} // namespace concurrency::pool::coroutine::policy
