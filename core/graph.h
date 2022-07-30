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

template <Descriptor VertexType, typename Traits>
class Graph : private Traits::template Impl<VertexType> {
public:
  using TraitBase = Traits::template Impl<VertexType>;
  using vertex_type = TraitBase::vertex_type;
  using edge_type = TraitBase::edge_type;
  static constexpr bool directed_ = TraitBase::directed_;
  static constexpr bool is_graph_ = true;

  Graph() : TraitBase() {}

  void add_vertex(const vertex_type &v) { TraitBase::add_vertex(v); }

  void add_edge(const vertex_type &src, const vertex_type &dst) {
    TraitBase::add_edge(src, dst);
  }

  auto adj(const vertex_type &src) { return TraitBase::adj(src); }

  auto adj(const vertex_type &src) const { return TraitBase::adj(src); }

  const auto &vertices() const noexcept { return TraitBase::vertices(); }

  [[nodiscard]] auto size() const noexcept {
    return TraitBase::vertices().size();
  }

  const auto &edges() const noexcept { return TraitBase::edges(); }

  const auto &out_edges() const noexcept { return TraitBase::out_edges(); }

  bool has_vertex(const vertex_type &src) const noexcept {
    return TraitBase::has_vertex(src);
  }

  bool has_edge(const edge_type &edge) const noexcept {
    return TraitBase::has_edge(edge);
  }

  template <typename PropertyType>
  VertexProperty<PropertyType, vertex_type> &
  add_vertex_property(const GraphPropertyTag &tag) {
    properties_.emplace(
        tag, make_unique<VertexProperty<PropertyType, vertex_type>>());
    return get_vertex_property<PropertyType>(tag);
  }

  template <typename PropertyType>
  EdgeProperty<PropertyType, edge_type> &
  add_edge_property(const GraphPropertyTag &tag) {
    properties_.emplace(tag,
                        make_unique<EdgeProperty<PropertyType, edge_type>>());
    return get_edge_property<PropertyType>(tag);
  }

  template <typename PropertyType>
  GraphProperty<PropertyType> &add_graph_property(const GraphPropertyTag &tag) {
    properties_.emplace(tag, make_unique<GraphProperty<PropertyType>>());
    return get_graph_property<PropertyType>(tag);
  }

  template <typename PropertyType>
  VertexProperty<PropertyType, vertex_type> &
  get_vertex_property(const GraphPropertyTag &tag) {
    return dynamic_cast<VertexProperty<PropertyType, vertex_type> &>(
        *properties_.at(tag));
  }

  template <typename PropertyType>
  const VertexProperty<PropertyType, vertex_type> &
  get_vertex_property(const GraphPropertyTag &tag) const {
    return dynamic_cast<const VertexProperty<PropertyType, vertex_type> &>(
        *properties_.at(tag));
  }

  template <typename PropertyType>
  EdgeProperty<PropertyType, edge_type> &
  get_edge_property(const GraphPropertyTag &tag) {
    return dynamic_cast<EdgeProperty<PropertyType, edge_type> &>(
        *properties_.at(tag));
  }

  template <typename PropertyType>
  const EdgeProperty<PropertyType, edge_type> &
  get_edge_property(const GraphPropertyTag &tag) const {
    return dynamic_cast<const EdgeProperty<PropertyType, edge_type> &>(
        *properties_.at(tag));
  }

  template <typename PropertyType>
  GraphProperty<PropertyType> &get_graph_property(const GraphPropertyTag &tag) {
    return dynamic_cast<GraphProperty<PropertyType> &>(*properties_.at(tag));
  }

  template <typename PropertyType>
  const GraphProperty<PropertyType> &
  get_graph_property(const GraphPropertyTag &tag) const {
    return dynamic_cast<const GraphProperty<PropertyType> &>(
        *properties_.at(tag));
  }

private:
  unordered_map<GraphPropertyTag, unique_ptr<Property>> properties_;
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

  const auto &edges() const noexcept { return edges_; }

  const auto &out_edges() const noexcept { return out_edges_; }

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

template <Descriptor VertexType>
using DiGraph = Graph<VertexType, GraphTraits<true, AdjListTraitTag>>;

template <Descriptor VertexType>
using UndirGraph = Graph<VertexType, GraphTraits<false, AdjListTraitTag>>;

} // namespace frozenca

#endif //__CLRS4_GRAPH_H__
