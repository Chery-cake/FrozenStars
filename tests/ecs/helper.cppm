module;

export module ecs_helper;

export import std.compat;
export import ecs;

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

  /////////////

  struct Position {
    float x = 0, y = 0;
    bool operator<=>(const Position &) const = default;
  };
  struct Velocity {
    float dx = 0, dy = 0;
  };
  struct Health {
    int hp = 100;
  };
  struct Mana {
    int mp = 50;
  };
  struct Name {
    std::string name;
  };

  // Component that tracks destruction count (global)
  int destr_counter = 0;
  struct Tracked {
    int id;
    Tracked(int i = -1) : id(i) {}
    ~Tracked() { ++destr_counter; }
  };
}
