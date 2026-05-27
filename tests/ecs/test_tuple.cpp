import ecs_helper;
#include <cassert>

void test_constructor_forwarding() {
  TEST("forwarding_constructor");
  ecs::entity::Tuple<Position, Velocity> t(Position(3, 4), Velocity(1, 2));
  assert(t.get<Position>().x == 3);
  assert(t.get<Position>().y == 4);
  assert(t.get<Velocity>().dx == 1);
  assert(t.get<Velocity>().dy == 2);
  PASS();
}

void test_default_constructor() {
  TEST("default_constructor");
  ecs::entity::Tuple<Position, Velocity> t;
  assert(t.get<Position>().x == 0);
  assert(t.get<Velocity>().dx == 0);
  PASS();
}

void test_destruction() {
  TEST("destruction");
  destr_counter = 0;
  {
    ecs::entity::Tuple<Tracked> t(Tracked(10));
    destr_counter = 0;
  }
  assert(destr_counter == 1);
  PASS();
}

void test_data() {
  TEST("data");

  ecs::entity::Tuple<Position, Velocity> t(Position(1, 2), Velocity(3, 4));

  assert(t.get<Position>().x == 1);
  assert(t.get<Velocity>().dx == 3);

  t.get<Position>().x = 5;
  t.get<Velocity>().dx = 10;

  assert(t.get<Position>().x == 5);
  assert(t.get<Velocity>().dx == 10);

  PASS();
}

void test_data_const() {
  TEST("data_const");

  const ecs::entity::Tuple<Position, Velocity> t(Position(1, 2),
                                                 Velocity(3, 4));

  assert(t.get<Position>().x == 1);
  assert(t.get<Velocity>().dx == 3);

  PASS();
}

int main() {
  std::println("=== ECS Tuple Tests ===");

  test_constructor_forwarding();
  test_default_constructor();
  test_destruction();
  test_data();
  test_data_const();

  std::println("\n{}/{} tests passed", tests_passed, tests_run);
  return (tests_passed == tests_run) ? 0 : 1;
}
