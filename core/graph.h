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
  using vertices_type = TraitBase::vertices_type;
  using vertex_iterator_type = TraitBase::vertex_iterator_type;
  using edge_type = TraitBase::edge_type;
  static constexpr bool directed_ = TraitBase::directed_;
  static constexpr bool multi_ = TraitBase::multi_;
  static constexpr bool is_graph_ = true;

  Graph() : TraitBase() {}

  void add_vertex(const vertex_type &v) { TraitBase::add_vertex(v); }

  void add_edge(const vertex_type &src, const vertex_type &dst) {
    TraitBase::add_edge(src, dst);
  }

  void remove_edge(const vertex_type &src, const vertex_type &dst) {
    TraitBase::remove_edge(src, dst);
  }

  [[nodiscard]] auto adj(const vertex_type &src) { return TraitBase::adj(src); }

  [[nodiscard]] auto adj(const vertex_type &src) const {
    return TraitBase::adj(src);
  }

  [[nodiscard]] const auto &vertices() const noexcept {
    return TraitBase::vertices();
  }

  [[nodiscard]] auto size() const noexcept {
    return TraitBase::vertices().size();
  }

  [[nodiscard]] bool empty() const noexcept { return size() == 0; }

  [[nodiscard]] auto &edges() noexcept { return TraitBase::edges(); }

  [[nodiscard]] const auto &edges() const noexcept {
    return TraitBase::edges();
  }

  [[nodiscard]] bool has_vertex(const vertex_type &src) const noexcept {
    return TraitBase::has_vertex(src);
  }

  [[nodiscard]] bool has_edge(const edge_type &edge) const noexcept {
    return TraitBase::has_edge(edge);
  }

  template <typename PropertyType>
  VertexProperty<vertex_type, PropertyType> &
  add_vertex_property(const GraphPropertyTag &tag) {
    properties_.emplace(
        tag, make_unique<VertexProperty<vertex_type, PropertyType>>());
    return get_vertex_property<PropertyType>(tag);
  }

  template <typename PropertyType>
  EdgeProperty<vertex_type, PropertyType> &
  add_edge_property(const GraphPropertyTag &tag) {
    properties_.emplace(tag,
                        make_unique<EdgeProperty<vertex_type, PropertyType>>());
    return get_edge_property<PropertyType>(tag);
  }

  template <typename PropertyType>
  PropertyType &add_graph_property(const GraphPropertyTag &tag) {
    properties_.emplace(tag, make_unique<GraphProperty<PropertyType>>());
    return get_graph_property<PropertyType>(tag);
  }

  template <typename PropertyType>
  VertexProperty<vertex_type, PropertyType> &
  get_vertex_property(const GraphPropertyTag &tag) {
    return dynamic_cast<VertexProperty<vertex_type, PropertyType> &>(
        *properties_.at(tag));
  }

  template <typename PropertyType>
  const VertexProperty<vertex_type, PropertyType> &
  get_vertex_property(const GraphPropertyTag &tag) const {
    return dynamic_cast<const VertexProperty<vertex_type, PropertyType> &>(
        *properties_.at(tag));
  }

  template <typename PropertyType>
  EdgeProperty<vertex_type, PropertyType> &
  get_edge_property(const GraphPropertyTag &tag) {
    return dynamic_cast<EdgeProperty<vertex_type, PropertyType> &>(
        *properties_.at(tag));
  }

  template <typename PropertyType>
  const EdgeProperty<vertex_type, PropertyType> &
  get_edge_property(const GraphPropertyTag &tag) const {
    return dynamic_cast<const EdgeProperty<vertex_type, PropertyType> &>(
        *properties_.at(tag));
  }

  template <typename PropertyType>
  PropertyType &get_graph_property(const GraphPropertyTag &tag) {
    return dynamic_cast<GraphProperty<PropertyType> &>(*properties_.at(tag))
        .get();
  }

  template <typename PropertyType>
  const PropertyType &get_graph_property(const GraphPropertyTag &tag) const {
    return dynamic_cast<const GraphProperty<PropertyType> &>(
               *properties_.at(tag))
        .get();
  }

private:
  unordered_map<GraphPropertyTag, unique_ptr<Property>> properties_;
};

template <Descriptor VertexType, bool Directed, bool Multi,
          typename ContainerTraitTag>
struct GraphTraitsImpl
    : public ContainerTraitTag::template Trait<
          VertexType, Multi,
          GraphTraitsImpl<VertexType, Directed, Multi, ContainerTraitTag>> {
  using vertex_type = VertexType;
  using Base = ContainerTraitTag::template Trait<
      VertexType, Multi,
      GraphTraitsImpl<VertexType, Directed, Multi, ContainerTraitTag>>;
  inline static constexpr bool directed_ = Directed;
  inline static constexpr bool multi_ = Multi;

  using Base::add_vertex;
  using Base::adj;
  using Base::has_edge;
  using Base::has_vertex;
  using Base::vertices;
  using vertices_type = Base::vertices_type;
  using vertex_iterator_type = Base::vertex_iterator_type;

  void add_edge(const vertex_type &src, const vertex_type &dst) {
    add_vertex(src);
    add_vertex(dst);
    Base::add_edge(src, dst);
    if constexpr (!directed_) {
      Base::add_edge(dst, src);
    }
  }

  void remove_edge(const vertex_type &src, const vertex_type &dst) {
    add_vertex(src);
    add_vertex(dst);
    Base::remove_edge(src, dst);
    if constexpr (!directed_) {
      Base::remove_edge(dst, src);
    }
  }
};

template <bool Directed, bool Multi, typename ContainerTraitTag>
struct GraphTraits {
  template <Descriptor VertexType>
  using Impl = GraphTraitsImpl<VertexType, Directed, Multi, ContainerTraitTag>;
};

template <Descriptor Vertex, bool Multi, typename Derived>
struct AdjListTraits {
  using vertex_type = Vertex;
  using vertices_type = unordered_set<vertex_type>;
  using vertex_iterator_type = vertices_type::iterator;

  using edge_type = EdgePair<Vertex>;
  using adj_list_type = conditional_t<Multi, unordered_multiset<vertex_type>,
                                      unordered_set<vertex_type>>;
  using adj_iterator_type = adj_list_type::iterator;
  using const_adj_iterator_type = adj_list_type::const_iterator;
  using edges_type = unordered_map<vertex_type, adj_list_type>;

  vertices_type vertices_;
  edges_type edges_;

  [[nodiscard]] const auto &vertices() const noexcept { return vertices_; }

  [[nodiscard]] auto &edges() noexcept { return edges_; }

  [[nodiscard]] const auto &edges() const noexcept { return edges_; }

  void add_vertex(const vertex_type &vertex) {
    vertices_.insert(vertex);
    if (!edges_.contains(vertex)) {
      edges_[vertex] = {};
    }
  }

  [[nodiscard]] bool has_vertex(const vertex_type &vertex) const {
    vertices_.contains(vertex);
  }

  [[nodiscard]] auto &adj(const vertex_type &vertex) {
    return edges_.at(vertex);
  }

  [[nodiscard]] const auto &adj(const vertex_type &vertex) const {
    return edges_.at(vertex);
  }

  [[nodiscard]] bool has_edge(const edge_type &edge) const {
    auto edge_range = adj(edge.first);
    return ranges::find(edge_range, edge.second) != edge_range.end();
  }

  void add_edge(const vertex_type &src, const vertex_type &dst) {
    if constexpr (!Multi) {
      if (src == dst) {
        return;
      }
    }
    edges_[src].insert(dst);
  }

  void remove_edge(const vertex_type &src, const vertex_type &dst) {
    if constexpr (!Multi) {
      if (src == dst) {
        return;
      }
    }
    edges_[src].erase(dst);
  }
};

struct AdjListTraitTag {
  template <Descriptor VertexType, bool Multi, typename Derived>
  using Trait = AdjListTraits<VertexType, Multi, Derived>;
};

template <Descriptor V, bool Const>
requires(is_integral_v<V>) struct AdjMatIterator {
  using difference_type = ptrdiff_t;
  using value_type = V;
  using pointer = conditional_t<Const, const V *, V *>;
  using reference = conditional_t<Const, const V &, V &>;
  using iterator_category = bidirectional_iterator_tag;
  using iterator_concept = iterator_category;
  using orig_iter_type = conditional_t<Const, vector<char>::const_iterator,
                                       vector<char>::iterator>;

  orig_iter_type first_;
  orig_iter_type curr_;
  V val_;
  orig_iter_type last_;

  AdjMatIterator() noexcept = default;

  AdjMatIterator(orig_iter_type first, orig_iter_type curr,
                 orig_iter_type last) noexcept
      : first_{first}, curr_{curr}, last_{last} {
    set_pos();
  }

  AdjMatIterator(const AdjMatIterator &other) noexcept
      : AdjMatIterator(other.first_, other.curr_, other.last_) {}

  AdjMatIterator(const AdjMatIterator<V, false> &other) noexcept requires(Const)
      : AdjMatIterator(other.first_, other.curr_, other.last_) {}

  reference operator*() noexcept { return val_; }

  pointer operator->() noexcept { return &val_; }

  void set_pos() noexcept {
    while (curr_ != last_ && !(*curr_)) {
      ++curr_;
    }
    val_ = static_cast<V>(curr_ - first_);
  }

  void inc_pos() noexcept {
    if (curr_ != last_) {
      ++curr_;
    }
    while (curr_ != last_ && !(*curr_)) {
      ++curr_;
    }
    val_ = static_cast<V>(curr_ - first_);
  }

  void dec_pos() noexcept {
    if (curr_ != first_) {
      --curr_;
    }
    while (curr_ != first_ && !(*curr_)) {
      --curr_;
    }
    val_ = static_cast<V>(curr_ - first_);
  }

  AdjMatIterator &operator++() noexcept {
    inc_pos();
    return *this;
  }

  AdjMatIterator operator++(int) noexcept {
    AdjMatIterator temp = *this;
    inc_pos();
    return temp;
  }

  AdjMatIterator &operator--() noexcept {
    dec_pos();
    return *this;
  }

  AdjMatIterator operator--(int) noexcept {
    AdjMatIterator temp = *this;
    dec_pos();
    return temp;
  }

  friend bool operator==(const AdjMatIterator &x,
                         const AdjMatIterator &y) noexcept {
    return x.first_ == y.first_ && x.curr_ == y.curr_ && x.last_ == y.last_;
  }

  friend bool operator!=(const AdjMatIterator &x,
                         const AdjMatIterator &y) noexcept {
    return !(x == y);
  }

  friend difference_type operator-(const AdjMatIterator &x,
                                   const AdjMatIterator &y) noexcept {
    auto it = y;
    difference_type cnt = 0;
    while (it != x) {
      ++it;
      ++cnt;
    }
    return cnt;
  }
};

template <Descriptor Vertex, typename Derived>
requires(is_integral_v<Vertex>) struct AdjMatTraits {
  using vertex_type = Vertex;
  using vertices_type = vector<Vertex>;
  using vertex_iterator_type = vertices_type::iterator;

  using edge_type = EdgePair<Vertex>;
  using edges_type = vector<char>;
  using adj_iterator_type = AdjMatIterator<Vertex, false>;
  using const_adj_iterator_type = AdjMatIterator<Vertex, true>;

  vertices_type vertices_;
  edges_type edges_;

  [[nodiscard]] const auto &vertices() const noexcept { return vertices_; }

  [[nodiscard]] const auto num_vertices() const noexcept {
    return ssize(vertices_);
  }

  [[nodiscard]] auto &edges() noexcept { return edges_; }

  [[nodiscard]] const auto &edges() const noexcept { return edges_; }

  void add_vertex(const vertex_type &vertex) {
    if (!has_vertex(vertex) && vertex >= 0) {
      auto old_size = num_vertices();
      auto new_size = static_cast<index_t>(vertex + 1);
      vertices_.resize(new_size);
      iota(vertices_.begin() + old_size, vertices_.end(),
           static_cast<vertex_type>(old_size));

      if (new_size > old_size) {
        vector<char> new_edges(new_size * new_size);
        for (index_t i = old_size - 1; i >= 0; --i) {
          for (index_t j = old_size - 1; j >= 0; --j) {
            new_edges[i * new_size + j] = edges_[i * old_size + j];
          }
        }
        swap(new_edges, edges_);
      }
    }
  }

  [[nodiscard]] bool has_vertex(const vertex_type &vertex) const {
    return vertex >= 0 && vertex < num_vertices();
  }

  [[nodiscard]] ranges::subrange<adj_iterator_type, adj_iterator_type>
  adj(const vertex_type &vertex) {
    if (!has_vertex(vertex)) {
      throw invalid_argument("in adj() the graph has no vertex");
    }
    auto first = edges_.begin() + vertex * num_vertices();
    auto last = edges_.begin() + (vertex + 1) * num_vertices();
    return {adj_iterator_type(first, first, last),
            adj_iterator_type(first, last, last)};
  }

  [[nodiscard]] ranges::subrange<const_adj_iterator_type,
                                 const_adj_iterator_type>
  adj(const vertex_type &vertex) const {
    if (!has_vertex(vertex)) {
      throw invalid_argument("in adj() the graph has no vertex");
    }
    auto first = edges_.begin() + vertex * num_vertices();
    auto last = edges_.begin() + (vertex + 1) * num_vertices();
    return {const_adj_iterator_type(first, first, last),
            const_adj_iterator_type(first, last, last)};
  }

  [[nodiscard]] bool has_edge(const edge_type &edge) const {
    return has_vertex(edge.first) && has_vertex(edge.second) &&
           edges_[edge.first * num_vertices() + edge.second];
  }

  void add_edge(const vertex_type &src, const vertex_type &dst) {
    edges_[src * num_vertices() + dst] = 1;
  }

  void remove_edge(const vertex_type &src, const vertex_type &dst) {
    edges_[src * num_vertices() + dst] = 0;
  }
};

struct AdjMatTraitTag {
  template <Descriptor VertexType, bool Multi, typename Derived>
  requires(!Multi) using Trait = AdjMatTraits<VertexType, Derived>;
};

template <Descriptor VertexType>
using DirGraph = Graph<VertexType, GraphTraits<true, false, AdjListTraitTag>>;

template <Descriptor VertexType>
using UndirGraph =
    Graph<VertexType, GraphTraits<false, false, AdjListTraitTag>>;

template <Descriptor VertexType>
using AdjMatDirGraph =
    Graph<VertexType, GraphTraits<true, false, AdjMatTraitTag>>;

template <Descriptor VertexType>
using AdjMatUndirGraph =
    Graph<VertexType, GraphTraits<false, false, AdjMatTraitTag>>;

template <Descriptor VertexType>
using MultiGraph = Graph<VertexType, GraphTraits<true, true, AdjListTraitTag>>;

} // namespace frozenca

#endif //__CLRS4_GRAPH_H__
