module;

export module resource;

export import :policy;
export import :registry;
export import :registry_impl;

/*

import std.compat;

Exemple on how to use

struct Thing {
  std::vector<int> vec;
  std::unordered_map<float, std::string> map;
};

struct ThingDefinitions {
  inline static const Thing t1{.vec = {0, 1}, .map = {{1.5, "test"}}};
  inline static const Thing t2{.vec = {1, 2}, .map = {{2.5, "test"}}};
};

class Things {
  std::vector<int> vec;
  std::unordered_map<float, std::string> map;

public:
  explicit Things(const Thing &t) : vec(t.vec), map(t.map) {}
};

class Things2 {
  std::vector<int> vec;
  std::unordered_map<float, std::string> map;
  int in = 0;
  float fl = 0.0F;

public:
  explicit Things2(const Thing &t, int i, float f)
      : vec(t.vec), map(t.map), in(i), fl(f) {}
};

using ThingRegistry =
    resource::Registry<Thing, Things, resource::WeakPtrPolicy<Thing, Things>>;
using ThingRegistry2 =
    resource::Registry<Thing, Things2, resource::WeakPtrPolicy<Thing, Things2>>;

ThingRegistry regis;
ThingRegistry2 regis2;

void init() {
  regis.emplace(&ThingDefinitions::t1);
  regis.emplace(&ThingDefinitions::t2);

  regis2.emplace(&ThingDefinitions::t1, 5, 5.0F);
  regis2.emplace(&ThingDefinitions::t2, 10, 10.0F);
}

*/
