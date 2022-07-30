#ifndef __CLRS4_GRAPH_H__
#define __CLRS4_GRAPH_H__

#include <common.h>
#include <concepts>
#include <graph_properties.h>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <numeric>
#include <ranges>
#include <set>
#include <unordered_map>
#include <unordered_set>

namespace frozenca {

using namespace std;

template <Descriptor VertexType, typename Traits, typename Properties>
class Graph : private Traits::template Impl<VertexType>,
              private Properties::template Impl<VertexType> {
public:
  using TraitBase = Traits::template Impl<VertexType>;
  using PropertyBase = Properties::template Impl<VertexType>;
  using vertex_type = TraitBase::vertex_type;
  using edge_type = TraitBase::edge_type;
  static constexpr bool directed_ = TraitBase::directed_;
  static constexpr bool is_graph_ = true;

  Graph() : TraitBase(), PropertyBase() {}

  // can't convert from graph with different graph traits
  // only conversion between graph with the same graph traits
  // (but possibly different graph properties) are allowed
  template <typename OtherProperties>
  Graph(const Graph<VertexType, Traits, OtherProperties> &other_graph) {
    this->vertices_ = other_graph.vertices();
    this->edges_ = other_graph.edges();
    this->out_edges_ = other_graph.out_edges();

    using PropertyImpl = Properties::template Impl<VertexType>;
    using OtherPropertyImpl = OtherProperties::template Impl<VertexType>;
    if constexpr (HasEdgeWeightProperty<PropertyImpl> &&
                  HasEdgeWeightProperty<OtherPropertyImpl>) {
      this->edge_weights_ = other_graph(e_w);
    }
    if constexpr (HasVertexDistanceProperty<PropertyImpl> &&
                  HasVertexDistanceProperty<OtherPropertyImpl>) {
      this->vertex_distances_ = other_graph(v_dist);
    }
    if constexpr (HasVertexVisitedProperty<PropertyImpl> &&
                  HasVertexVisitedProperty<OtherPropertyImpl>) {
      this->vertex_visited_ = other_graph(v_visited);
    }
  }

  template <typename OtherProperties>
  Graph(Graph<VertexType, Traits, OtherProperties> &&other_graph) {
    this->vertices_ = other_graph.move_vertices();
    this->edges_ = other_graph.move_edges();
    this->out_edges_ = other_graph.move_out_edges();

    using PropertyImpl = Properties::template Impl<VertexType>;
    using OtherPropertyImpl = OtherProperties::template Impl<VertexType>;
    if constexpr (HasEdgeWeightProperty<PropertyImpl> &&
                  HasEdgeWeightProperty<OtherPropertyImpl>) {
      this->edge_weights_ = other_graph(e_w);
    }
    if constexpr (HasVertexDistanceProperty<PropertyImpl> &&
                  HasVertexDistanceProperty<OtherPropertyImpl>) {
      this->vertex_distances_ = other_graph(v_dist);
    }
    if constexpr (HasVertexVisitedProperty<PropertyImpl> &&
                  HasVertexVisitedProperty<OtherPropertyImpl>) {
      this->vertex_visited_ = other_graph(v_visited);
    }
  }

  void add_vertex(const vertex_type& v) {
    TraitBase::add_vertex(v);
  }

  void add_edge(const vertex_type &src, const vertex_type &dst) {
    TraitBase::add_edge(src, dst);
  }

  auto adj(const vertex_type &src) { return TraitBase::adj(src); }

  auto adj(const vertex_type &src) const { return TraitBase::adj(src); }

  const auto &vertices() const noexcept { return TraitBase::vertices(); }

  [[nodiscard]] auto size() const noexcept {
    return TraitBase::vertices().size();
  }

  auto &&move_vertices() noexcept { return move(TraitBase::move_vertices()); }

  const auto &edges() const noexcept { return TraitBase::edges(); }

  auto &&move_edges() noexcept { return move(TraitBase::move_edges()); }

  const auto &out_edges() const noexcept { return TraitBase::out_edges(); }

  auto &&move_out_edges() noexcept { return move(TraitBase::move_out_edges()); }

  bool has_vertex(const vertex_type &src) const noexcept {
    return TraitBase::has_vertex(src);
  }

  bool has_edge(const edge_type &edge) const noexcept {
    return TraitBase::has_edge(edge);
  }

  template <typename Derived>
  auto &operator()(VertexPropertyTag<Derived> tag, const vertex_type &vertex) {
    if (!has_vertex(vertex)) {
      throw invalid_argument("Vertex is not in the graph");
    }
    return PropertyBase::operator()(tag.derived(), vertex);
  }

  template <typename Derived>
  const auto &operator()(VertexPropertyTag<Derived> tag,
                         const vertex_type &vertex) const {
    if (!has_vertex(vertex)) {
      throw invalid_argument("Vertex is not in the graph");
    }
    return PropertyBase::operator()(tag.derived(), vertex);
  }

  template <typename Derived>
  const auto &operator()(VertexPropertyTag<Derived> tag) const {
    return PropertyBase::operator()(tag.derived());
  }

  template <typename Derived>
  auto &operator()(EdgePropertyTag<Derived> tag, edge_type edge) {
    if constexpr (!directed_) {
      if (edge.first > edge.second) {
        swap(edge.first, edge.second);
      }
    }
    if (!has_edge(edge)) {
      throw invalid_argument("Edge is not in the graph");
    }
    return PropertyBase::operator()(tag.derived(), edge);
  }

  template <typename Derived>
  const auto &operator()(EdgePropertyTag<Derived> tag, edge_type edge) const {
    if constexpr (!directed_) {
      if (edge.first > edge.second) {
        swap(edge.first, edge.second);
      }
    }
    if (!has_edge(edge)) {
      throw invalid_argument("Edge is not in the graph");
    }
    return PropertyBase::operator()(tag.derived(), edge);
  }

  template <typename Derived>
  const auto &operator()(EdgePropertyTag<Derived> tag) const {
    return PropertyBase::operator()(tag.derived());
  }
};

template <Descriptor Vertex, typename Derived> struct AdjListTraits {
  using vertex_type = Vertex;
  static constexpr bool int_vertex_ = is_integral_v<vertex_type>;
  using vertices_type = conditional_t<int_vertex_, vector<vertex_type>,
                                      unordered_set<vertex_type>>;
  using vertex_iterator_type = vertices_type::iterator;

  using edge_type = EdgePair<Vertex>;
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

  auto &&move_vertices() noexcept { return move(vertices_); }

  const auto &edges() const noexcept { return edges_; }

  auto &&move_edges() noexcept { return move(edges_); }

  const auto &out_edges() const noexcept { return out_edges_; }

  auto &&move_out_edges() noexcept { return move(out_edges_); }

  void add_vertex(const vertex_type &vertex) {
    if constexpr (int_vertex_) {
      if (vertex < 0) {
        throw invalid_argument("Negative integer vertex");
      }
      auto curr_size = ssize(vertices_);
      if (vertex >= curr_size) {
        vertices_.resize(vertex + 1);
        iota(vertices_.begin() + curr_size, vertices_.end(),
             static_cast<vertex_type>(curr_size));
      }
    } else {
      vertices_.insert(vertex);
    }
    if (!out_edges_.contains(vertex)) {
      edges_.emplace_front();
      out_edges_.emplace(vertex, edges_.begin());
    }
  }

  bool has_vertex(const vertex_type &vertex) const {
    if constexpr (int_vertex_) {
      return vertex >= 0 && vertex < ssize(vertices_);
    } else {
      vertices_.contains(vertex);
    }
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

template <typename ContainerTraitTag>
using UndirGraphTraits = GraphTraits<false, ContainerTraitTag>;

template <Descriptor VertexType>
using DiGraph =
    Graph<VertexType, DiGraphTraits<AdjListTraitTag>, TraversalProperty>;

template <Descriptor VertexType>
using UnionFindGraph =
    Graph<VertexType, UndirGraphTraits<AdjListTraitTag>, UnionFindProperties>;

template <Descriptor VertexType, typename WeightType>
using WeightedDiGraph = Graph<VertexType, DiGraphTraits<AdjListTraitTag>,
                              TraversalWeightProperties<WeightType>>;

} // namespace frozenca

#endif //__CLRS4_GRAPH_H__
