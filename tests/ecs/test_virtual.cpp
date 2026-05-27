import ecs_helper;
#include <cassert>

struct testEnt : public ecs::entity::Virtual<testEnt, Position, Velocity> {
  using ecs::entity::Virtual<testEnt, Position, Velocity>::Virtual;
  testEnt(Position p, Velocity v) : Position(p), Velocity(v) {};
};

struct trackedEnt : public ecs::entity::Virtual<trackedEnt, Tracked> {
  using ecs::entity::Virtual<trackedEnt, Tracked>::Virtual;
  trackedEnt(Tracked t) : Tracked(t) {};
};

void test_constructor_forwarding() {
  TEST("forwarding_constructor");
  testEnt t(Position(3, 4), Velocity(1, 2));
  std::cout << t.get<Position>().x << std::endl;
  assert(t.get<Position>().x == 3);
  assert(t.get<Position>().y == 4);
  assert(t.get<Velocity>().dx == 1);
  assert(t.get<Velocity>().dy == 2);
  PASS();
}

void test_default_constructor() {
  TEST("default_constructor");
  testEnt t;
  assert(t.get<Position>().x == 0);
  assert(t.get<Velocity>().dx == 0);
  PASS();
}

void test_destruction() {
  TEST("destruction");
  destr_counter = 0;
  {
    trackedEnt t(Tracked(10));
    destr_counter = 0;
  }
  assert(destr_counter == 1);
  PASS();
}

void test_data() {
  TEST("data");

  testEnt t(Position(1, 2), Velocity(3, 4));

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

  const testEnt t(Position(1, 2), Velocity(3, 4));

  assert(t.get<Position>().x == 1);
  assert(t.get<Velocity>().dx == 3);

  PASS();
}

int main() {
  std::println("=== ECS Virtual Tests ===");

  test_constructor_forwarding();
  test_default_constructor();
  test_destruction();
  test_data();
  test_data_const();

  std::println("\n{}/{} tests passed", tests_passed, tests_run);
  return (tests_passed == tests_run) ? 0 : 1;
}
