import memory_helper;
#include <cassert>

void test_create() {
  TEST("create");

  memory::allocator::Memory<sizeof(int) * 10> mem;

  assert(mem.capacity() == (sizeof(int) * 10));

  PASS();
}

int main() {
  std::println("=== Memory memory allocator Tests ===");

  test_create();

  std::println("\n{}/{} tests passed", tests_passed, tests_run);
  return (tests_passed == tests_run) ? 0 : 1;
}
