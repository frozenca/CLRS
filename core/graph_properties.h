#ifndef __CLRS4_GRAPH_PROPERTIES_H__
#define __CLRS4_GRAPH_PROPERTIES_H__

#include <common.h>
#include <concepts>
#include <map>
#include <unordered_map>
#include <vector>

namespace frozenca {

using namespace std;

template <typename T>
concept Descriptor = is_default_constructible_v<T> && is_assignable_v<T &, T> &&
    equality_comparable<T>;

template <typename T>
concept GraphConcept = T::is_graph_;

template <typename T>
concept DiGraphConcept = GraphConcept<T> && T::directed_;

template <typename Derived> struct VertexPropertyTag {
  Derived &derived() noexcept { return static_cast<Derived &>(*this); }
  const Derived &derived() const noexcept {
    return static_cast<const Derived &>(*this);
  }
};

template <typename Derived> struct EdgePropertyTag {
  Derived &derived() noexcept { return static_cast<Derived &>(*this); }
  const Derived &derived() const noexcept {
    return static_cast<const Derived &>(*this);
  }
};

struct EmptyPropertyImpl {
  void operator()() const noexcept {
    // do nothing
  }
};

struct EmptyProperty {
  template <Descriptor VertexType> using Impl = EmptyPropertyImpl;
};

template <Descriptor VertexType, typename... BaseProperties>
struct GraphProperties : public BaseProperties::template Impl<VertexType>... {
  using BaseProperties::template Impl<VertexType>::operator()...;
};

struct VertexDistanceTag : public VertexPropertyTag<VertexDistanceTag> {};

VertexDistanceTag v_dist;

template <Arithmetic DistanceType, Descriptor VertexType>
struct VertexDistanceImpl {

  static constexpr bool int_vertex_ = is_integral_v<VertexType>;

  DistanceType &operator()(VertexDistanceTag, const VertexType &vertex) {
    if constexpr (int_vertex_) {
      if (vertex >= ssize(vertex_distances_)) {
        vertex_distances_.resize(vertex + 1);
      }
    }
    return vertex_distances_[vertex];
  }
  const DistanceType &operator()(VertexDistanceTag,
                                 const VertexType &vertex) const {
    return vertex_distances_.at(vertex);
  }

  const auto &operator()(VertexDistanceTag) const noexcept {
    return vertex_distances_;
  }

  auto &&operator()(VertexDistanceTag) noexcept {
    return move(vertex_distances_);
  }

  conditional_t<int_vertex_, vector<DistanceType>,
                unordered_map<VertexType, DistanceType>>
      vertex_distances_;
};

template <Arithmetic DistanceType, Descriptor VertexType>
DistanceType
getVertexDistType(const VertexDistanceImpl<DistanceType, VertexType> &);

template <typename U>
concept HasVertexDistanceProperty = requires(U u) {
  getVertexDistType(u);
};

template <HasVertexDistanceProperty T>
using VertexDistanceType = decltype(getVertexDistType(declval<T>()));

template <Arithmetic DistanceType> struct VertexDistanceProperty {
  template <Descriptor VertexType>
  using Impl = VertexDistanceImpl<DistanceType, VertexType>;
};

struct VertexVisitedTag : public VertexPropertyTag<VertexVisitedTag> {};

VertexVisitedTag v_visited;

enum class Visited { Unvisited, Visiting, Visited };

template <Descriptor VertexType> struct VertexVisitedImpl {

  static constexpr bool int_vertex_ = is_integral_v<VertexType>;

  Visited &operator()(VertexVisitedTag, const VertexType &vertex) {
    if constexpr (int_vertex_) {
      if (vertex >= ssize(vertex_visited_)) {
        vertex_visited_.resize(vertex + 1);
      }
    }
    return vertex_visited_[vertex];
  }
  const Visited &operator()(VertexVisitedTag, const VertexType &vertex) const {
    return vertex_visited_.at(vertex);
  }

  const auto &operator()(VertexVisitedTag) const noexcept {
    return vertex_visited_;
  }

  auto &&operator()(VertexVisitedTag) noexcept { return move(vertex_visited_); }

  conditional_t<int_vertex_, vector<Visited>,
                unordered_map<VertexType, Visited>>
      vertex_visited_;
};

template <Descriptor VertexType>
Visited getVertexVisitedType(const VertexVisitedImpl<VertexType> &);

template <typename U>
concept HasVertexVisitedProperty = requires(U u) {
  getVertexVisitedType(u);
};

template <HasVertexVisitedProperty T>
using VertexVisitedType = decltype(getVertexVisitedType(declval<T>()));

struct VertexVisitedProperty {
  template <Descriptor VertexType> using Impl = VertexVisitedImpl<VertexType>;
};

struct EdgeWeightTag : public EdgePropertyTag<EdgeWeightTag> {};

EdgeWeightTag e_w;

template <Arithmetic WeightType, typename EdgeType> struct EdgeWeightImpl {
  WeightType &operator()(EdgeWeightTag, const EdgeType &edge) {
    return edge_weights_[edge];
  }
  const WeightType &operator()(EdgeWeightTag, const EdgeType &edge) const {
    return edge_weights_.at(edge);
  }

  const auto &operator()(EdgeWeightTag) const noexcept { return edge_weights_; }

  auto &&operator()(EdgeWeightTag) noexcept { return move(edge_weights_); }

  map<EdgeType, WeightType> edge_weights_;
};

template <Arithmetic WeightType, typename EdgeType>
WeightType getEdgeWeightType(const EdgeWeightImpl<WeightType, EdgeType> &);

template <typename U>
concept HasEdgeWeightProperty = requires(U u) {
  getEdgeWeightType(u);
};

template <HasEdgeWeightProperty T>
using EdgeWeightType = decltype(getEdgeWeightType(declval<T>()));

template <Arithmetic WeightType> struct EdgeWeightProperty {
  template <Descriptor VertexType>
  using Impl = EdgeWeightImpl<WeightType, pair<VertexType, VertexType>>;
};

} // namespace frozenca

#endif //__CLRS4_GRAPH_PROPERTIES_H__
