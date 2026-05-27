import ecs_helper;
#include <cassert>

template <> struct ecs::component::ComponentDependencies<Velocity> {
  using required = std::tuple<Position>;
};

using storage = ecs::component::DynamicStorage;

void test_add_remove() {
  TEST("add_remove");

  ecs::entity::Tuple<storage> t;

  bool added = t.get<storage>().add_check<Velocity>(Velocity(5, 6));
  assert(added == false);
  assert(!t.get<storage>().has<Velocity>());

  try {
    t.get<storage>().add_throw<Velocity>(Velocity(5, 6));
    added = true;
  } catch (...) {
    added = false;
  }
  assert(!t.get<storage>().has<Velocity>());

  try {
    t.get<storage>().emplace<Velocity>();
    added = true;
  } catch (...) {
    added = false;
  }
  assert(!t.get<storage>().has<Velocity>());

  added = t.get<storage>().add_check<Position>(Position(5, 6));
  assert(added == true);
  assert(t.get<storage>().has<Position>());
  t.get<storage>().remove<Position>();
  assert(!t.get<storage>().has<Position>());

  try {
    t.get<storage>().add_throw<Position>(Position(5, 6));
    added = true;
  } catch (...) {
    added = false;
  }
  assert(t.get<storage>().has<Position>());
  t.get<storage>().remove<Position>();
  assert(!t.get<storage>().has<Position>());

  try {
    t.get<storage>().emplace<Position>();
    added = true;
  } catch (...) {
    added = false;
  }
  assert(t.get<storage>().has<Position>());
  t.get<storage>().remove<Position>();
  assert(!t.get<storage>().has<Position>());

  t.get<storage>().add_unchecked<Velocity>(Velocity(5, 6));
  assert(t.get<storage>().has<Velocity>());

  PASS();
}

void test_get() {
  TEST("get");

  ecs::entity::Tuple<storage> t;

  Position p = t.get<storage>().emplace<Position>(5, 6);

  assert(t.get<storage>().has<Position>());

  assert(t.get<storage>().get<Position>()->x == 5);
  assert(p == *t.get<storage>().get<Position>());

  assert(t.get<storage>().meets_dependencies<Velocity>());

  PASS();
}

void test_clear_erase() {
  TEST("clear_erase");

  ecs::entity::Tuple<storage> t;

  t.get<storage>().emplace<Position>();
  assert(t.get<storage>().has<Position>());
  t.get<storage>().clear();
  assert(!t.get<storage>().has<Position>());

  t.get<storage>().emplace<Position>();
  assert(t.get<storage>().has<Position>());

  t.get<storage>().erase(typeid(Position));
  assert(!t.get<storage>().has<Position>());

  PASS();
}

int main() {
  std::println("=== ECS Dynamic Storage Tests ===");

  test_add_remove();
  test_get();
  test_clear_erase();

  std::println("\n{}/{} tests passed", tests_passed, tests_run);
  return (tests_passed == tests_run) ? 0 : 1;
}
