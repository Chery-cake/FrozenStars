#include <cassert>
import concurrency_helper;

void test_create() {
  TEST("create");

  concurrency::pool::Pool p("test");
  concurrency::pool::ThreadPool t(p);

  assert(t.size() == std::thread::hardware_concurrency());

  t.resize(2);
  assert(t.size() == 2);

  t.resize(10);
  assert(t.size() == 10);

  concurrency::pool::ThreadPool tp(p, 5);
  assert(tp.size() == 5);

  PASS();
}

void test_submit() {
  TEST("submit");

  concurrency::pool::Pool p("test");
  concurrency::pool::ThreadPool t(p);

  auto f = [](int x) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return x;
  };

  std::vector<std::future<int>> fi;

  std::ranges::for_each(std::views::iota(0, 12), [&t, f, &fi](uint32_t i) {
    fi.push_back(t.submit(f, i));
  });

  assert(t.size() == std::thread::hardware_concurrency());

  t.resize(2);
  assert(t.size() == 2);
  t.resize(10);
  assert(t.size() == 10);

  std::atomic<int> result = 0;
  int expected = 0;

  std::ranges::for_each(std::views::iota(0, 12),
                        [&fi, &result, &expected](uint32_t i) {
                          result += fi[i].get();
                          expected += i;
                        });

  assert(result == expected);

  t.resize(8);
  assert(t.size() == 8);

  auto f2 = [](std::atomic<int> &r) {
    r += 1;
    assert(true);
  };
  result = 0;
  std::ranges::for_each(std::views::iota(0, 12), [&t, f2, &result](uint32_t) {
    t.submit(f2, std::ref(result));
  });
  t.wait();
  assert(result.load() == 12);

  PASS();
}

concurrency::pool::coroutine::coroutineTask<
    concurrency::pool::coroutine::policy::Suspend::Always, void>
run_scheduler_probe(concurrency::pool::ThreadPool &t,
                    std::optional<bool> &result) {
  const bool before = concurrency::pool::coroutine::isPoolWorker;
  co_await t.schedule();
  const bool after = concurrency::pool::coroutine::isPoolWorker;
  result = (!before && after);
}

concurrency::pool::coroutine::coroutineTask<
    concurrency::pool::coroutine::policy::Suspend::Always, void>
run_scheduler_probe2(concurrency::queues::TaskQueue *queue,
                     std::optional<bool> &result) {
  const bool before = concurrency::pool::coroutine::isPoolWorker;
  co_await concurrency::pool::ThreadPool::schedule(queue);
  const bool after = concurrency::pool::coroutine::isPoolWorker;
  result = (!before && after);
}

void test_scheduler() {
  TEST("scheduler");

  concurrency::pool::Pool p("test");
  concurrency::pool::ThreadPool t(p, 2);

  std::optional<bool> result;
  auto task1 = run_scheduler_probe(t, result);
  task1.start();

  while (!result.has_value()) {
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }

  result.reset();
  auto task2 = run_scheduler_probe2(t.queue(), result);
  task2.start();

  while (!result.has_value()) {
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }

  assert(*result);

  PASS();
}

concurrency::pool::coroutine::coroutineTask<
    concurrency::pool::coroutine::policy::Suspend::Always, bool>
run_scheduler_return_probe(concurrency::pool::ThreadPool &t) {
  const bool before = concurrency::pool::coroutine::isPoolWorker;
  co_await t.schedule();
  const bool after = concurrency::pool::coroutine::isPoolWorker;
  co_return (!before && after);
}

concurrency::pool::coroutine::coroutineTask<
    concurrency::pool::coroutine::policy::Suspend::Always, bool>
run_scheduler_return_probe2(concurrency::queues::TaskQueue *queue) {
  const bool before = concurrency::pool::coroutine::isPoolWorker;
  co_await concurrency::pool::ThreadPool::schedule(queue);
  const bool after = concurrency::pool::coroutine::isPoolWorker;
  co_return (!before && after);
}

void test_scheduler_return_value() {
  TEST("scheduler return value");

  concurrency::pool::Pool p("test");
  concurrency::pool::ThreadPool t(p, 2);

  auto r1 = run_scheduler_return_probe(t);
  assert(r1.get());

  auto r2 = run_scheduler_return_probe2(t.queue());
  assert(r2.get());

  PASS();
}

int main() {
  std::println("=== Concurrency Thread Pool Tests ===");

  test_create();
  test_submit();
  test_scheduler();
  test_scheduler_return_value();

  std::println("\n{}/{} tests passed", tests_passed, tests_run);
  return (tests_passed == tests_run) ? 0 : 1;
}
