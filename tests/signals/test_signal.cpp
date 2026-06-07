import signals_helper;

#include <cassert>

void test_void() {
  TEST("void");

  signals::Signals<void()> sig;

  std::atomic<int> count(0);

  signals::Signals<void()>::Slot func1 = [&count]() { ++count; };

  auto func2 = [&count]() { ++count; };

  assert(sig.empty());
  assert(sig.size() == 0);
  assert(!sig.disconnect(5));
  assert(sig.connection_ids().empty());

  assert(sig.connect(nullptr).error() == signals::ConnectionError::NullSlot);

  auto result = sig.connect(std::move(func1));
  assert(result.has_value());
  signals::ConnectionId id1 = result.value();

  result = sig.connect(func2);
  assert(result.has_value());
  signals::ConnectionId id2 = result.value();

  sig.emit();
  assert(count == 2);
  assert(!sig.empty());
  assert(sig.size() == 2);
  assert(sig.connection_ids().size() == 2);
  assert(sig.connection_ids()[0] == id1);

  assert(sig.disconnect(id1) == true);
  sig.emit();
  assert(count == 3);
  assert(sig.size() == 1);
  assert(sig.connection_ids().size() == 1);
  assert(sig.connection_ids()[0] == id2);

  sig.clear();
  sig.emit();
  assert(count == 3);
  assert(sig.empty());
  assert(sig.size() == 0);
  assert(sig.connection_ids().size() == 0);

  count.store(0);

  auto func3 = [&count]() { count.fetch_add(1, std::memory_order_relaxed); };

  result = sig.connect(func3);
  result = sig.connect(func3);
  result = sig.connect(func3);

  sig.emit_parallel();
  assert(count == 3);
  sig.clear();
  count.store(0);

  sig.emit_parallel();

  result = sig.connect(func2);
  result = sig.connect(func2);
  result = sig.connect(func2);

  sig.emit_until([]() { return true; });
  assert(count == 1);

  count.store(0);

  sig.emit_until([]() { return false; });
  assert(count == 3);

  count.store(0);

  sig.emit_until([&count]() { return count == 2; });
  assert(count == 2);

  PASS();
}

void test_int() {
  TEST("int");

  signals::Signals<int()> sig;

  std::atomic<int> count(0);

  signals::Signals<int()>::Slot func1 = [&count]() { return count++; };

  auto func2 = [&count]() { return count++; };

  assert(sig.empty());
  assert(sig.size() == 0);
  assert(!sig.disconnect(5));
  assert(sig.connection_ids().empty());

  assert(sig.connect(nullptr).error() == signals::ConnectionError::NullSlot);

  auto result = sig.connect(std::move(func1));
  assert(result.has_value());
  signals::ConnectionId id1 = result.value();

  result = sig.connect(func2);
  assert(result.has_value());
  signals::ConnectionId id2 = result.value();

  sig.emit();
  assert(count == 2);
  assert(!sig.empty());
  assert(sig.size() == 2);
  assert(sig.connection_ids().size() == 2);
  assert(sig.connection_ids()[0] == id1);

  assert(sig.disconnect(id1) == true);
  sig.emit();
  assert(count == 3);
  assert(sig.size() == 1);
  assert(sig.connection_ids().size() == 1);
  assert(sig.connection_ids()[0] == id2);

  sig.clear();
  sig.emit();
  assert(count == 3);
  assert(sig.empty());
  assert(sig.size() == 0);
  assert(sig.connection_ids().size() == 0);

  count.store(0);

  auto func3 = [&count]() {
    return count.fetch_add(1, std::memory_order_relaxed);
  };

  result = sig.connect(func3);
  result = sig.connect(func3);
  result = sig.connect(func3);

  sig.emit_parallel();
  assert(count == 3);
  sig.clear();
  count.store(0);

  sig.emit_parallel();

  result = sig.connect(func2);
  result = sig.connect(func2);
  result = sig.connect(func2);

  sig.emit_until([](int) { return true; });
  assert(count == 1);

  count.store(0);

  sig.emit_until([](int) { return false; });
  assert(count == 3);

  count.store(0);

  sig.emit_until([&count](int) { return count == 2; });
  assert(count == 2);

  PASS();
}

void test_void_param() {
  TEST("void_param");

  signals::Signals<void(int)> sig;

  std::atomic<int> count(0);

  signals::Signals<void(int)>::Slot func1 = [&count](int i) { count += i; };

  auto func2 = [&count](int i) { count += i; };

  assert(sig.empty());
  assert(sig.size() == 0);
  assert(!sig.disconnect(5));
  assert(sig.connection_ids().empty());

  assert(sig.connect(nullptr).error() == signals::ConnectionError::NullSlot);

  auto result = sig.connect(std::move(func1));
  assert(result.has_value());
  signals::ConnectionId id1 = result.value();

  result = sig.connect(func2);
  assert(result.has_value());
  signals::ConnectionId id2 = result.value();

  sig.emit(1);
  assert(count == 2);
  assert(!sig.empty());
  assert(sig.size() == 2);
  assert(sig.connection_ids().size() == 2);
  assert(sig.connection_ids()[0] == id1);

  assert(sig.disconnect(id1) == true);
  sig.emit(1);
  assert(count == 3);
  assert(sig.size() == 1);
  assert(sig.connection_ids().size() == 1);
  assert(sig.connection_ids()[0] == id2);

  sig.clear();
  sig.emit(1);
  assert(count == 3);
  assert(sig.empty());
  assert(sig.size() == 0);
  assert(sig.connection_ids().size() == 0);

  count.store(0);

  auto func3 = [&count](int i) {
    count.fetch_add(i, std::memory_order_relaxed);
  };

  result = sig.connect(func3);
  result = sig.connect(func3);
  result = sig.connect(func3);

  sig.emit_parallel(1);
  assert(count == 3);
  sig.clear();
  count.store(0);

  sig.emit_parallel(1);

  result = sig.connect(func2);
  result = sig.connect(func2);
  result = sig.connect(func2);

  sig.emit_until(1, []() { return true; });
  assert(count == 1);

  count.store(0);

  sig.emit_until(1, []() { return false; });
  assert(count == 3);

  count.store(0);

  sig.emit_until(1, [&count]() { return count == 2; });
  assert(count == 2);

  PASS();
}

void test_int_param() {
  TEST("int_param");

  signals::Signals<int(int)> sig;

  std::atomic<int> count(0);

  signals::Signals<int(int)>::Slot func1 = [&count](int i) {
    count += i;
    return count.load();
  };

  auto func2 = [&count](int i) {
    count += i;
    return count.load();
  };

  assert(sig.empty());
  assert(sig.size() == 0);
  assert(!sig.disconnect(5));
  assert(sig.connection_ids().empty());

  assert(sig.connect(nullptr).error() == signals::ConnectionError::NullSlot);

  auto result = sig.connect(std::move(func1));
  assert(result.has_value());
  signals::ConnectionId id1 = result.value();

  result = sig.connect(func2);
  assert(result.has_value());
  signals::ConnectionId id2 = result.value();

  sig.emit(1);
  assert(count == 2);
  assert(!sig.empty());
  assert(sig.size() == 2);
  assert(sig.connection_ids().size() == 2);
  assert(sig.connection_ids()[0] == id1);

  assert(sig.disconnect(id1) == true);
  sig.emit(1);
  assert(count == 3);
  assert(sig.size() == 1);
  assert(sig.connection_ids().size() == 1);
  assert(sig.connection_ids()[0] == id2);

  sig.clear();
  sig.emit(1);
  assert(count == 3);
  assert(sig.empty());
  assert(sig.size() == 0);
  assert(sig.connection_ids().size() == 0);

  count.store(0);

  auto func3 = [&count](int i) {
    return count.fetch_add(i, std::memory_order_relaxed);
  };

  result = sig.connect(func3);
  result = sig.connect(func3);
  result = sig.connect(func3);

  sig.emit_parallel(1);
  assert(count == 3);
  sig.clear();
  count.store(0);

  sig.emit_parallel(1);

  result = sig.connect(func2);
  result = sig.connect(func2);
  result = sig.connect(func2);

  sig.emit_until(1, [](int) { return true; });
  assert(count == 1);

  count.store(0);

  sig.emit_until(1, [](int) { return false; });
  assert(count == 3);

  count.store(0);

  sig.emit_until(1, [&count](int) { return count == 2; });
  assert(count == 2);

  PASS();
}

void test_void_multi_params() {
  TEST("void_param");

  signals::Signals<void(int, int)> sig;

  std::atomic<int> count(0);

  signals::Signals<void(int, int)>::Slot func1 = [&count](int i, int t) {
    count += i + t;
  };

  auto func2 = [&count](int i, int t) { count += i + t; };

  assert(sig.empty());
  assert(sig.size() == 0);
  assert(!sig.disconnect(5));
  assert(sig.connection_ids().empty());

  assert(sig.connect(nullptr).error() == signals::ConnectionError::NullSlot);

  auto result = sig.connect(std::move(func1));
  assert(result.has_value());
  signals::ConnectionId id1 = result.value();

  result = sig.connect(func2);
  assert(result.has_value());
  signals::ConnectionId id2 = result.value();

  sig.emit(1, 1);
  assert(count == 4);
  assert(!sig.empty());
  assert(sig.size() == 2);
  assert(sig.connection_ids().size() == 2);
  assert(sig.connection_ids()[0] == id1);

  assert(sig.disconnect(id1) == true);
  sig.emit(1, 1);
  assert(count == 6);
  assert(sig.size() == 1);
  assert(sig.connection_ids().size() == 1);
  assert(sig.connection_ids()[0] == id2);

  sig.clear();
  sig.emit(1, 1);
  assert(count == 6);
  assert(sig.empty());
  assert(sig.size() == 0);
  assert(sig.connection_ids().size() == 0);

  count.store(0);

  auto func3 = [&count](int i, int t) {
    count.fetch_add(i, std::memory_order_relaxed);
    count.fetch_add(t, std::memory_order_relaxed);
  };

  result = sig.connect(func3);
  result = sig.connect(func3);
  result = sig.connect(func3);

  sig.emit_parallel(1, 1);
  assert(count == 6);
  sig.clear();
  count.store(0);

  sig.emit_parallel(1, 1);

  result = sig.connect(func2);
  result = sig.connect(func2);
  result = sig.connect(func2);

  sig.emit_until(1, 1, []() { return true; });
  assert(count == 2);

  count.store(0);

  sig.emit_until(1, 1, []() { return false; });
  assert(count == 6);

  count.store(0);

  sig.emit_until(1, 1, [&count]() { return count == 4; });
  assert(count == 4);

  PASS();
}

void test_int_multi_params() {
  TEST("int_param");

  signals::Signals<int(int, int)> sig;

  std::atomic<int> count(0);

  signals::Signals<int(int, int)>::Slot func1 = [&count](int i, int t) {
    count += i + t;
    return count.load();
  };

  auto func2 = [&count](int i, int t) {
    count += i + t;
    return count.load();
  };

  assert(sig.empty());
  assert(sig.size() == 0);
  assert(!sig.disconnect(5));
  assert(sig.connection_ids().empty());

  assert(sig.connect(nullptr).error() == signals::ConnectionError::NullSlot);

  auto result = sig.connect(std::move(func1));
  assert(result.has_value());
  signals::ConnectionId id1 = result.value();

  result = sig.connect(func2);
  assert(result.has_value());
  signals::ConnectionId id2 = result.value();

  sig.emit(1, 1);
  assert(count == 4);
  assert(!sig.empty());
  assert(sig.size() == 2);
  assert(sig.connection_ids().size() == 2);
  assert(sig.connection_ids()[0] == id1);

  assert(sig.disconnect(id1) == true);
  sig.emit(1, 1);
  assert(count == 6);
  assert(sig.size() == 1);
  assert(sig.connection_ids().size() == 1);
  assert(sig.connection_ids()[0] == id2);

  sig.clear();
  sig.emit(1, 1);
  assert(count == 6);
  assert(sig.empty());
  assert(sig.size() == 0);
  assert(sig.connection_ids().size() == 0);

  count.store(0);

  auto func3 = [&count](int i, int t) {
    count.fetch_add(i, std::memory_order_relaxed);
    return count.fetch_add(t, std::memory_order_relaxed);
  };

  result = sig.connect(func3);
  result = sig.connect(func3);
  result = sig.connect(func3);

  sig.emit_parallel(1, 1);
  assert(count == 6);
  sig.clear();
  count.store(0);

  sig.emit_parallel(1, 1);

  result = sig.connect(func2);
  result = sig.connect(func2);
  result = sig.connect(func2);

  sig.emit_until(1, 1, [](int) { return true; });
  assert(count == 2);

  count.store(0);

  sig.emit_until(1, 1, [](int) { return false; });
  assert(count == 6);

  count.store(0);

  sig.emit_until(1, 1, [&count](int) { return count == 4; });
  assert(count == 4);

  PASS();
}

int main() {
  std::println("=== Signals Signal Tests ===");

  test_void();
  test_int();

  test_void_param();
  test_int_param();

  test_void_multi_params();
  test_int_multi_params();

  std::println("\n{}/{} tests passed", tests_passed, tests_run);
  return (tests_passed == tests_run) ? 0 : 1;
}
