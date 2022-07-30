#ifndef __CLRS4_GRAPH_PROPERTIES_H__
#define __CLRS4_GRAPH_PROPERTIES_H__

#include <common.h>
#include <concepts>
#include <functional>
#include <hashfunction.h>
#include <unordered_map>
#include <vector>

namespace frozenca {

using namespace std;

template <Descriptor V> struct Hash<EdgePair<V>> {
  size_t operator()(const EdgePair<V> &e) const {
    return Hash<V>{}(e.second) + 1234567ULL * Hash<V>{}(e.first);
  }
};

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

template <typename DistanceType, Descriptor VertexType>
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

template <typename DistanceType, Descriptor VertexType>
DistanceType
getVertexDistType(const VertexDistanceImpl<DistanceType, VertexType> &);

template <typename U>
concept HasVertexDistanceProperty = requires(U u) {
  getVertexDistType(u);
};

template <typename DistanceType> struct VertexDistanceProperty {
  template <Descriptor VertexType>
  using Impl = VertexDistanceImpl<DistanceType, VertexType>;
};

struct VertexRankTag : public VertexPropertyTag<VertexRankTag> {};

VertexRankTag v_rank;

template <Descriptor VertexType> struct VertexRankImpl {

  using rank_t = index_t;

  static constexpr bool int_vertex_ = is_integral_v<VertexType>;

  rank_t &operator()(VertexRankTag, const VertexType &vertex) {
    if constexpr (int_vertex_) {
      if (vertex >= ssize(vertex_ranks_)) {
        vertex_ranks_.resize(vertex + 1);
      }
    }
    return vertex_ranks_[vertex];
  }
  const rank_t &operator()(VertexRankTag, const VertexType &vertex) const {
    return vertex_ranks_.at(vertex);
  }

  const auto &operator()(VertexRankTag) const noexcept { return vertex_ranks_; }

  auto &&operator()(VertexRankTag) noexcept { return move(vertex_ranks_); }

  conditional_t<int_vertex_, vector<rank_t>, unordered_map<VertexType, rank_t>>
      vertex_ranks_;
};

template <Descriptor VertexType>
index_t getVertexRankType(const VertexRankImpl<VertexType> &);

template <typename U>
concept HasVertexRankProperty = requires(U u) {
  getVertexRankType(u);
};

struct VertexRankProperty {
  template <Descriptor VertexType> using Impl = VertexRankImpl<VertexType>;
};

struct VertexParentTag : public VertexPropertyTag<VertexParentTag> {};

VertexParentTag v_parent;

template <Descriptor VertexType> struct VertexParentImpl {

  static constexpr bool int_vertex_ = is_integral_v<VertexType>;

  VertexType &operator()(VertexParentTag, const VertexType &vertex) {
    if constexpr (int_vertex_) {
      if (vertex >= ssize(vertex_parents_)) {
        vertex_parents_.resize(vertex + 1);
      }
    }
    return vertex_parents_[vertex];
  }
  const VertexType &operator()(VertexParentTag,
                               const VertexType &vertex) const {
    return vertex_parents_.at(vertex);
  }

  const auto &operator()(VertexParentTag) const noexcept {
    return vertex_parents_;
  }

  auto &&operator()(VertexParentTag) noexcept { return move(vertex_parents_); }

  conditional_t<int_vertex_, vector<VertexType>,
                unordered_map<VertexType, VertexType>>
      vertex_parents_;
};

template <Descriptor VertexType>
VertexType getVertexParentType(const VertexParentImpl<VertexType> &);

template <typename U>
concept HasVertexParentProperty = requires(U u) {
  getVertexParentType(u);
};

struct VertexParentProperty {
  template <Descriptor VertexType> using Impl = VertexParentImpl<VertexType>;
};

struct VertexChildTag : public VertexPropertyTag<VertexChildTag> {};

VertexChildTag v_child;

template <Descriptor VertexType> struct VertexChildImpl {

  static constexpr bool int_vertex_ = is_integral_v<VertexType>;

  VertexType &operator()(VertexChildTag, const VertexType &vertex) {
    if constexpr (int_vertex_) {
      if (vertex >= ssize(vertex_children_)) {
        vertex_children_.resize(vertex + 1);
      }
    }
    return vertex_children_[vertex];
  }
  const VertexType &operator()(VertexChildTag, const VertexType &vertex) const {
    return vertex_children_.at(vertex);
  }

  const auto &operator()(VertexChildTag) const noexcept {
    return vertex_children_;
  }

  auto &&operator()(VertexChildTag) noexcept { return move(vertex_children_); }

  conditional_t<int_vertex_, vector<VertexType>,
                unordered_map<VertexType, VertexType>>
      vertex_children_;
};

template <Descriptor VertexType>
VertexType getVertexChildType(const VertexChildImpl<VertexType> &);

template <typename U>
concept HasVertexChildProperty = requires(U u) {
  getVertexChildType(u);
};

struct VertexChildProperty {
  template <Descriptor VertexType> using Impl = VertexChildImpl<VertexType>;
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

struct VertexVisitedProperty {
  template <Descriptor VertexType> using Impl = VertexVisitedImpl<VertexType>;
};

struct EdgeWeightTag : public EdgePropertyTag<EdgeWeightTag> {};

EdgeWeightTag e_w;

template <typename WeightType, typename EdgeType> struct EdgeWeightImpl {
  WeightType &operator()(EdgeWeightTag, const EdgeType &edge) {
    return edge_weights_[edge];
  }
  const WeightType &operator()(EdgeWeightTag, const EdgeType &edge) const {
    return edge_weights_.at(edge);
  }

  const auto &operator()(EdgeWeightTag) const noexcept { return edge_weights_; }

  auto &&operator()(EdgeWeightTag) noexcept { return move(edge_weights_); }

  unordered_map<EdgeType, WeightType, Hash<EdgeType>> edge_weights_;
};

template <typename WeightType, typename EdgeType>
WeightType getEdgeWeightType(const EdgeWeightImpl<WeightType, EdgeType> &);

template <typename U>
concept HasEdgeWeightProperty = requires(U u) {
  getEdgeWeightType(u);
};

template <typename WeightType> struct EdgeWeightProperty {
  template <Descriptor VertexType>
  using Impl = EdgeWeightImpl<WeightType, EdgePair<VertexType>>;
};

struct TraversalProperty {
  template <Descriptor VertexType>
  using Impl = GraphProperties<VertexType, VertexVisitedProperty>;
};

struct UnionFindProperties {
  template <Descriptor VertexType>
  using Impl = GraphProperties<VertexType, VertexParentProperty,
                               VertexRankProperty, VertexChildProperty>;
};

template <typename DistType> struct TraversalWeightProperties {
  template <Descriptor VertexType>
  using Impl = GraphProperties<VertexType, VertexVisitedProperty,
                               VertexDistanceProperty<DistType>,
                               EdgeWeightProperty<DistType>>;
};

} // namespace frozenca

#endif //__CLRS4_GRAPH_PROPERTIES_H__
