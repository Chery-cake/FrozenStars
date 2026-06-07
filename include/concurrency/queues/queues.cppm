module;

export module concurrency.queues;

export import :queue;
export import :fifo;

export namespace concurrency::queues {
enum class QueueKind : uint8_t { FIFO };
}

// TODO
// change to use lock-free queue with semaphores
// will need to implement the queue, std doen't have one lock-free
