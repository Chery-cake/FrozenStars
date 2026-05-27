module;

export module concurrency.queues;

export import :queue;
export import :fifo;

export namespace concurrency::queues {
enum class QueueKind : uint8_t { FIFO };
}
