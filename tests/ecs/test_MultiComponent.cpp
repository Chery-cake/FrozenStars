import ecs_helper;
#include <cassert>

template <typename T, size_t N>
using comp = ecs::component::MultiComponent<T, N>;

template <typename T> using dyComp = ecs::component::DynamicMultiComponent<T>;

template <> struct ecs::component::ComponentDependencies<Velocity> {
  using required = std::tuple<Position>;
};

void test_fixed() {
  TEST("fixed");

  ecs::entity::Tuple<comp<Position, 2>> t(
      comp<Position, 2>(Position(2, 6), Position()));

  auto multi = t.get<comp<Position, 2>>();

  assert(multi[0] == Position(2, 6));
  assert(multi[1] == Position());

  std::ranges::for_each(multi.components,
                        [](Position &p) { p = Position(5, 5); });

  std::ranges::for_each(multi.components,
                        [](Position &p) { assert(p == Position(5, 5)); });

  int count = 0;
  std::ranges::for_each(multi.begin(), multi.end(),
                        [&count](Position) { ++count; });
  assert(count == 2);

  ecs::entity::Tuple<comp<Velocity, 2>, Position> t2;
  assert((t2.get<comp<Velocity, 2>>().size() == 2));

  PASS();
}

void test_dyn() {
  TEST("dynamic");

  ecs::entity::Tuple<dyComp<Position>> t(
      dyComp<Position>({{.x = 2, .y = 6}, {}}));
  auto multi = t.get<dyComp<Position>>();

  assert(multi.size() == 2);
  assert(multi[0] == Position(2, 6));
  assert(multi[1] == Position());

  multi.components.clear();
  assert(multi.size() == 0);

  std::ranges::for_each(std::views::iota(0, 5),
                        [&multi](uint32_t i) { multi.add(Position(i, i)); });

  assert(multi.size() == 5);

  std::ranges::for_each(std::views::iota(0, 5), [&multi](uint32_t i) {
    assert(multi[i] == Position(i, i));
  });

  int count = 0;
  std::ranges::for_each(multi.begin(), multi.end(),
                        [&count](Position) { ++count; });
  assert(count == 5);

  ecs::entity::Tuple<dyComp<Velocity>, Position> t2;
  assert((t2.get<dyComp<Velocity>>().size() == 0));

  PASS();
}

int main() {
  std::println("=== ECS Dynamic Storage Tests ===");

  test_fixed();
  test_dyn();

  std::println("\n{}/{} tests passed", tests_passed, tests_run);
  return (tests_passed == tests_run) ? 0 : 1;
}
