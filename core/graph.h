#ifndef __CLRS4_GRAPH_H__
#define __CLRS4_GRAPH_H__

#include <common.h>
#include <concepts>
#include <list>
#include <map>
#include <ranges>
#include <set>
#include <unordered_map>
#include <unordered_set>

namespace frozenca {

using namespace std;

template <typename T>
concept Descriptor = is_default_constructible_v<T> && is_assignable_v<T &, T> &&
    equality_comparable<T>;

template <typename Derived> struct EdgePropertyTag {};

struct EdgeWeightTag : public EdgePropertyTag<EdgeWeightTag> {};

EdgeWeightTag e_w;

template <typename Derived> struct VertexPropertyTag {};

struct VertexDistanceTag : public VertexPropertyTag<VertexDistanceTag> {};

VertexDistanceTag v_dist;

template <typename Derived> struct EmptyProperty {
  void operator()() const noexcept {
    // do nothing
  }
};

template <Descriptor VertexType, typename Traits, typename Properties>
class Graph : public Traits::template Impl<VertexType>,
              public Properties::template Impl<VertexType> {
public:
  using TraitBase = Traits::template Impl<VertexType>;

  using TraitBase::add_edge;
  using TraitBase::add_vertex;
  using TraitBase::adj;
  using TraitBase::has_edge;
  using TraitBase::has_vertex;
  using TraitBase::vertices;
};

template <Descriptor Vertex, typename Derived> struct AdjListTraits {
  using vertex_type = Vertex;
  using vertices_type = unordered_set<vertex_type>;
  using vertex_iterator_type = vertices_type::iterator;

  using edge_type = pair<vertex_type, vertex_type>;
  using adj_list_type = list<edge_type>;
  using edges_type = list<adj_list_type>;
  using edge_iterator_type = adj_list_type::iterator;
  using const_edge_iterator_type = adj_list_type::const_iterator;
  using out_edges_type =
      unordered_map<vertex_type, typename edges_type::iterator>;

  vertices_type vertices_;
  edges_type edges_;
  out_edges_type out_edges_;

  const auto &vertices() const noexcept { return vertices_; }

  void add_vertex(const vertex_type &vertex) {
    vertices_.insert(vertex);
    if (!out_edges_.contains(vertex)) {
      edges_.emplace_front();
      out_edges_.emplace(vertex, edges_.begin());
    }
  }

  bool has_vertex(const vertex_type &vertex) const {
    return vertices_.contains(vertex);
  }

  ranges::subrange<edge_iterator_type, edge_iterator_type>
  adj(const vertex_type &vertex) {
    return *out_edges_.at(vertex);
  }

  ranges::subrange<const_edge_iterator_type, const_edge_iterator_type>
  adj(const vertex_type &vertex) const {
    return *out_edges_.at(vertex);
  }

  bool has_edge(const edge_type &edge) const {
    auto edge_range = adj(edge.first);
    return ranges::find(edge_range, edge) != edge_range.end();
  }

  void add_edge(const vertex_type &src, const vertex_type &dst) {
    out_edges_[src]->emplace_front(src, dst);
  }
};

struct AdjListTraitTag {
  template <Descriptor VertexType, typename Derived>
  using Trait = AdjListTraits<VertexType, Derived>;
};

template <Descriptor VertexType, bool Directed, typename ContainerTraitTag>
struct GraphTraitsImpl
    : public ContainerTraitTag::template Trait<
          VertexType,
          GraphTraitsImpl<VertexType, Directed, ContainerTraitTag>> {
  using vertex_type = VertexType;
  using Base = ContainerTraitTag::template Trait<
      VertexType, GraphTraitsImpl<VertexType, Directed, ContainerTraitTag>>;
  static constexpr bool directed_ = Directed;

  using Base::add_vertex;
  using Base::adj;
  using Base::has_edge;
  using Base::has_vertex;
  using Base::vertices;

  void add_edge(const vertex_type &src, const vertex_type &dst) {
    add_vertex(src);
    add_vertex(dst);
    Base::add_edge(src, dst);
    if constexpr (!directed_) {
      Base::add_edge(dst, src);
    }
  }
};

template <bool Directed, typename ContainerTraitTag> struct GraphTraits {
  template <Descriptor VertexType>
  using Impl = GraphTraitsImpl<VertexType, Directed, ContainerTraitTag>;
};

template <typename ContainerTraitTag>
using DiGraphTraits = GraphTraits<true, ContainerTraitTag>;

template <Descriptor VertexType, typename... BaseProperties>
struct GraphProperties : public BaseProperties::template Impl<VertexType>... {
  using BaseProperties::template Impl<VertexType>::operator()...;
};

template <Arithmetic WeightType, typename EdgeType> struct EdgeWeightImpl {
  WeightType &operator()(EdgeWeightTag, const EdgeType &edge) {
    return weights_[edge];
  }
  const WeightType &operator()(EdgeWeightTag, const EdgeType &edge) const {
    return weights_.at(edge);
  }

  map<EdgeType, WeightType> weights_;
};

template <Arithmetic WeightType> struct EdgeWeightProperty {
  template <Descriptor VertexType>
  using Impl = EdgeWeightImpl<WeightType, pair<VertexType, VertexType>>;
};

template <Arithmetic DistanceType, Descriptor VertexType>
struct VertexDistanceImpl {

  DistanceType &operator()(VertexDistanceTag, const VertexType &vertex) {
    return distances_[vertex];
  }
  const DistanceType &operator()(VertexDistanceTag,
                                 const VertexType &vertex) const {
    return distances_.at(vertex);
  }

  unordered_map<VertexType, DistanceType> distances_;
};

template <Arithmetic DistanceType> struct VertexDistanceProperty {
  template <Descriptor VertexType>
  using Impl = VertexDistanceImpl<DistanceType, VertexType>;
};

template <Arithmetic DistType> struct DijkstraProperties {
  template <Descriptor VertexType>
  using Impl = GraphProperties<VertexType, VertexDistanceProperty<DistType>,
                               EdgeWeightProperty<DistType>>;
};

} // namespace frozenca

#endif //__CLRS4_GRAPH_H__
