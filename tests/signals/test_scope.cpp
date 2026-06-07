import signals_helper;
#include <cassert>

void test_create() {
  TEST("create");

  auto sig = std::make_shared<signals::Signals<void()>>();

  int count = 0;

  auto result = sig->connect([&count]() { ++count; });
  signals::ConnectionId id = result.value();

  {
    signals::ScopedConnection<void()> scope;
    assert(!scope.is_connected());
    sig->emit();
    assert(count == 1);
  }

  {
    signals::ScopedConnection<void()> scope(sig, id, false);
    assert(scope.is_connected());
    signals::ConnectionId idR = scope.release();
    assert(id == idR);
    sig->emit();
    assert(count == 2);
  }

  {
    signals::ScopedConnection<void()> scope(sig, id, false);
    assert(scope.is_connected());
    scope.reset();
    sig->emit();
    assert(count == 2);
  }

  result = sig->connect([&count]() { ++count; });
  id = result.value();

  {
    signals::ScopedConnection<void()> scope(sig, id, false);
    assert(scope.is_connected());
  }
  sig->emit();
  assert(count == 3);

  {
    signals::ScopedConnection<void()> scope(sig, id, true);
    assert(scope.is_connected());
  }
  sig->emit();
  assert(count == 3);

  PASS();
}

void test_move() {
  TEST("move");

  auto sig = std::make_shared<signals::Signals<void()>>();
  int count = 0;

  auto result = sig->connect([&count]() { ++count; });
  signals::ConnectionId id = result.value();

  {
    signals::ScopedConnection<void()> scope1(sig, id, false);
    assert(scope1.is_connected());

    signals::ScopedConnection<void()> scope2(std::move(scope1));
    assert(scope2.is_connected());
    assert(!scope1.is_connected());
  }
  sig->emit();
  assert(count == 1);

  {
    signals::ScopedConnection<void()> scope1(sig, id, false);
    signals::ScopedConnection<void()> scope2;

    scope2 = std::move(scope1);
    assert(scope2.is_connected());
    assert(!scope1.is_connected());
  }
  sig->emit();
  assert(count == 2);

  {
    auto result2 = sig->connect([&count]() { count += 10; });
    signals::ConnectionId id2 = result2.value();

    signals::ScopedConnection<void()> scope1(sig, id, false);
    signals::ScopedConnection<void()> scope2(sig, id2, true);

    scope2 = std::move(scope1);

    assert(scope2.is_connected());
    assert(!scope1.is_connected());
  }
  sig->emit();
  assert(count == 3);

  {
    signals::ScopedConnection<void()> scope1(sig, id, true);
    assert(scope1.is_connected());

    signals::ScopedConnection<void()> scope2(std::move(scope1));
    assert(scope2.is_connected());
    assert(!scope1.is_connected());
  }
  sig->emit();
  assert(count == 3);

  result = sig->connect([&count]() { ++count; });
  id = result.value();

  {
    signals::ScopedConnection<void()> scope1(sig, id, true);
    signals::ScopedConnection<void()> scope2;

    scope2 = std::move(scope1);
    assert(scope2.is_connected());
    assert(!scope1.is_connected());
  }
  sig->emit();
  assert(count == 3);

  PASS();
}

int main() {
  std::println("=== Signals Scoped Connection Tests ===");

  test_create();
  test_move();

  std::println("\n{}/{} tests passed", tests_passed, tests_run);
  return (tests_passed == tests_run) ? 0 : 1;
}
