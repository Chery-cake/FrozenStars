module;

export module concurrency_helper;

export import std.compat;
export import concurrency;

export {
  int tests_run = 0;
  int tests_passed = 0;

  void TEST(std::string_view name) {
    ++tests_run;
    std::println("[TEST] {} ... ", name);
  }

  void PASS() {
    ++tests_passed;
    std::println("PASSED");
  }
}
