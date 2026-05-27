module;

export module ecs.entity:check_templates;

import std.compat;
import :entities;

export namespace ecs::entity {

// Primary template: two different template-template parameters → false
template <template <typename...> class A, template <typename...> class B>
struct is_same_template : std::false_type {};

// Specialisation: same parameter → true
template <template <typename...> class A>
struct is_same_template<A, A> : std::true_type {};

// Convenience variable template
template <template <typename...> class A, template <typename...> class B>
inline constexpr bool is_same_template_v = is_same_template<A, B>::value;

template <template <typename...> class Entity>
concept IsEntityTemplate =
    is_same_template_v<Entity, Tuple> || is_same_template_v<Entity, Linear> ||
    is_same_template_v<Entity, Virtual>;

template <template <typename, typename...> class Entity>
concept IsEntityTuple = is_same_template_v<Entity, Tuple>;

} // namespace ecs::entity
