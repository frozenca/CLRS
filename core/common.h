#ifndef __CLRS4_COMMON_H__
#define __CLRS4_COMMON_H__

#include <algorithm>
#include <cassert>
#include <compare>
#include <concepts>
#include <cstdint>
#include <functional>
#include <iterator>
#include <limits>
#include <memory>
#include <ranges>
#include <stdexcept>
#include <tuple>
#include <type_traits>
#include <utility>

namespace frozenca {

using namespace std;

using index_t = ptrdiff_t;
using attr_t = int32_t;
using uindex_t = make_unsigned_t<index_t>;

namespace {

template <typename... Ts>
concept AllSame =
    sizeof...(Ts) < 2 ||
    conjunction_v<is_same<tuple_element_t<0, tuple<Ts...>>, Ts>...>;

template <typename T>
concept Scalar = is_scalar_v<T>;

template <typename T>
concept Arithmetic = is_arithmetic_v<T>;

template <typename T>
concept SpanType = !Scalar<T> && requires(T t) {
  t.data();
  t.size();
};

template <typename T>
concept Containable = is_same_v<remove_cvref_t<T>, T>;

template <typename T>
concept Descriptor = is_default_constructible_v<T> && is_assignable_v<T &, T> &&
    equality_comparable<T>;

template <Descriptor Vertex> struct Edge {
  Vertex first;
  Vertex second;
};

template <Descriptor Vertex>
bool operator==(const Edge<Vertex> &e1, const Edge<Vertex> &e2) {
  return e1.first == e2.first && e1.second == e2.second;
}

template <Descriptor Vertex>
bool operator!=(const Edge<Vertex> &e1, const Edge<Vertex> &e2) {
  return !(e1 == e2);
}

template <Descriptor Vertex> struct UndirectedEdge {
  Vertex first;
  Vertex second;
};

template <Descriptor Vertex>
bool operator==(const UndirectedEdge<Vertex> &e1_,
                const UndirectedEdge<Vertex> &e2_) {
  auto e1 = e1_;
  auto e2 = e2_;
  if (e1.first > e1.second) {
    swap(e1.first, e1.second);
  }
  if (e2.first > e2.second) {
    swap(e2.first, e2.second);
  }
  return e1.first == e2.first && e1.second == e2.second;
}

template <Descriptor Vertex>
bool operator!=(const UndirectedEdge<Vertex> &e1,
                const UndirectedEdge<Vertex> &e2) {
  return !(e1 == e2);
}

template <typename T>
concept GraphConcept = T::is_graph_;

template <GraphConcept G> using V = typename G::vertex_type;

template <GraphConcept G> using Vs = typename G::vertices_type;

template <GraphConcept G> using E = typename G::edge_type;

template <typename T>
concept UndirGraphConcept = GraphConcept<T> && !T::directed_;

template <typename T>
concept DirGraphConcept = GraphConcept<T> && T::directed_;

template <typename T>
concept MultiGraphConcept = GraphConcept<T> && T::multi_;

template <typename T>
concept DiskAllocable = is_same_v<remove_cvref_t<T>, T> &&
    is_trivially_copyable_v<T> &&(sizeof(T) % alignof(T) == 0);

template <typename H, typename K>
concept Hashable = requires(H h, K k) {
  { h(k) } -> convertible_to<size_t>;
};

template <typename F, typename T>
concept PartiallyOrderedBy = requires(F &&f, const remove_reference_t<T> &t1,
                                      const remove_reference_t<T> &t2) {
  { invoke(forward<F>(f), t1, t2) } -> convertible_to<partial_ordering>;
  { invoke(forward<F>(f), t2, t1) } -> convertible_to<partial_ordering>;
};

template <typename T> struct Max {
  constexpr float operator()(const T &a, const T &b) const noexcept {
    return max(a, b);
  }
};

template <typename T>
concept Allocable = Containable<T> && is_trivially_copyable_v<T> &&
    (sizeof(T) % alignof(T) == 0);

inline constexpr size_t size_bytes() {
  if constexpr (numeric_limits<size_t>::max() == 0xFFFF) {
    return 16;
  } else if constexpr (numeric_limits<size_t>::max() == 0xFFFFFFFF) {
    return 32;
  } else {
    return 64;
  }
}

inline constexpr size_t size_half() {
  if constexpr (numeric_limits<size_t>::max() == 0xFFFF) {
    return 8;
  } else if constexpr (numeric_limits<size_t>::max() == 0xFFFFFFFF) {
    return 16;
  } else {
    return 32;
  }
}

constexpr size_t arr_default_length = 128;
constexpr size_t hashtable_default_width = 14u;
constexpr size_t hashtable_default_length = (1u << hashtable_default_width);
constexpr size_t default_pool_size_ = (1ul << 20ul);

} // anonymous namespace
} // namespace frozenca

#endif //__CLRS4_COMMON_H__
