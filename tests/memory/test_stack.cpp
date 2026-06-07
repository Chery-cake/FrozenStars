import memory_helper;
#include <cassert>

void test_create() {
  TEST("create");

  memory::allocator::Stack<sizeof(int) * 10> mem;

  int *i;
  float *f;

  i = mem.push<int>();
  f = mem.push<float>();

  *i = 10;
  *f = 5.5F;

  assert(mem.bytes_allocated() ==
         (sizeof(int) + sizeof(float) + (sizeof(size_t) * 2)));

  mem.pop(i);
  mem.pop(f);

  assert(i == nullptr);
  assert(f == nullptr);

  assert(mem.bytes_allocated() == 0);

  PASS();
}

int main() {
  std::println("=== Memory stack allocator Tests ===");

  test_create();

  std::println("\n{}/{} tests passed", tests_passed, tests_run);
  return (tests_passed == tests_run) ? 0 : 1;
}
