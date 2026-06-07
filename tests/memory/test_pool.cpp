import memory_helper;
#include <cassert>

void test_create() {
  TEST("create");

  memory::allocator::Pool<int, 10> mem;

  int *i;

  i = mem.allocate();

  *i = 10;

  assert(mem.available() == 9);

  mem.deallocate(i);

  assert(i == nullptr);

  assert(mem.available() == 10);

  PASS();
}

void test_random() {
  TEST("random");

  memory::allocator::Pool<int, 10> mem;

  std::array<int *, 10> arr;

  std::ranges::for_each(std::views::iota(0, 10),
                        [&arr, &mem](uint32_t i) { arr[i] = mem.allocate(); });

  assert(mem.available() == 0);

  std::ranges::for_each(
      []() {
        std::random_device rd;
        std::mt19937 g(rd());
        auto v = std::views::iota(0, 10) | std::ranges::to<std::vector>();
        std::ranges::shuffle(v, g);
        return v;
      }(),
      [&arr, &mem](uint32_t i) { mem.deallocate(arr[i]); });

  assert(mem.available() == 10);

  PASS();
}

int main() {
  std::println("=== Memory pool allocator Tests ===");

  test_create();
  test_random();

  std::println("\n{}/{} tests passed", tests_passed, tests_run);
  return (tests_passed == tests_run) ? 0 : 1;
}
