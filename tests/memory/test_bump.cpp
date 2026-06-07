import memory_helper;
#include <cassert>

void test_create() {
  TEST("create");

  memory::allocator::Bump<sizeof(int) * 10> mem;

  int *i;
  float *f;

  i = mem.allocate<int>();
  f = mem.allocate<float>();

  *i = 10;
  *f = 5.5F;

  assert(mem.bytes_allocated() == (sizeof(int) + sizeof(float)));

  mem.reset();

  assert(*i == 10);
  assert(*f == 5.5F);

  int *n;
  float *l;

  n = mem.allocate<int>();
  l = mem.allocate<float>();

  assert(i == n);
  assert(f == l);

  PASS();
}

int main() {
  std::println("=== Memory bump allocator Tests ===");

  test_create();

  std::println("\n{}/{} tests passed", tests_passed, tests_run);
  return (tests_passed == tests_run) ? 0 : 1;
}
