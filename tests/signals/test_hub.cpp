import signals_helper;
#include <cassert>

void test_create() {
  auto sig1 = std::make_shared<signals::Signals<void()>>();
  auto sig2 = std::make_shared<signals::Signals<void(int)>>();

  int count = 0;

  signals::SignalHub hub;

  assert(hub.size() == 0);
  assert(hub.empty());

  auto res1 = hub.connect(sig1, [&count]() { ++count; });
  auto scope = std::move(res1.value());
  assert(scope.is_connected());
  assert(hub.size() == 1);
  assert(!hub.empty());

  auto res2 = sig2->connect([&count](int i) { count += i; });
  signals::ConnectionId id2 = res2.value();
  hub.add_disconnector([&id2, &sig2]() { sig2->disconnect(id2); });
  assert(hub.size() == 2);

  hub.clear();
  assert(hub.size() == 0);
  assert(hub.empty());

  assert(sig1->empty());
  assert(sig2->empty());
}

int main() {
  std::println("=== Signals Signal Hub Tests ===");

  test_create();

  std::println("\n{}/{} tests passed", tests_passed, tests_run);
  return (tests_passed == tests_run) ? 0 : 1;
}
