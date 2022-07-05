#ifndef __CLRS4_GRAPH_H__
#define __CLRS4_GRAPH_H__

#include <algorithm>
#include <cassert>
#include <common.h>
#include <list>
#include <ranges>
#include <stdexcept>
#include <vector>

namespace frozenca::hard {

using namespace std;

namespace detail {

template <typename Weight> struct AdjListDescTrait {
  using desc_type = pair<index_t, Weight>;
  using edges_type = vector<list<desc_type>>;

  static void add_edge(edges_type &edges, index_t src, index_t dst,
                       float weight) {
    edges[src].emplace_back(dst, weight);
  }

  static index_t proj(const desc_type &desc) noexcept { return desc.first; }
};

template <> struct AdjListDescTrait<void> {
  using desc_type = index_t;
  using edges_type = vector<list<desc_type>>;

  static void add_edge(edges_type &edges, index_t src, index_t dst) {
    edges[src].emplace_back(dst);
  }

  static index_t proj(const desc_type &desc) noexcept { return desc; }
};

template <typename Weight> struct AdjListTrait {
  using desc_trait = AdjListDescTrait<Weight>;
  using desc_type = desc_trait::desc_type;
  using edges_type = desc_trait::edges_type;
  using edge_iter_type = list<desc_type>::iterator;
  using edge_const_iter_type = list<desc_type>::const_iterator;
  using edge_range_type = ranges::subrange<edge_iter_type>;
  using const_edge_range_type = ranges::subrange<edge_const_iter_type>;

  static void resize(edges_type &edges, index_t new_size) {
    auto old_size = curr_size(edges);
    if (new_size < old_size) {
      for (index_t i = 0; i < new_size; ++i) {
        erase_if(edges[i], [&new_size](const auto &edge_desc) {
          return desc_trait::proj(edge_desc) >= new_size;
        });
      }
    }
    edges.resize(new_size);
  }

  static index_t curr_size(const edges_type &edges) { return ssize(edges); }

  static edge_range_type get_edges(edges_type &edges, index_t index) {
    return edges[index];
  }

  static const_edge_range_type get_edges(const edges_type &edges,
                                         index_t index) {
    return edges[index];
  }

  static edge_iter_type get_edge(edges_type &edges, index_t src, index_t dst) {
    auto it = edges[src].begin();
    for (; it != edges[src].end(); ++it) {
      if (desc_trait::proj(*it) == dst) {
        break;
      }
    }
    return it;
  }

  static edge_const_iter_type get_edge(const edges_type &edges, index_t src,
                                       index_t dst) {
    auto it = edges[src].begin();
    for (; it != edges[src].end(); ++it) {
      if (desc_trait::proj(*it) == dst) {
        break;
      }
    }
    return it;
  }
};

template <typename Weight> struct AdjMatDescTrait {
  using desc_type = Weight;
  using edges_type = vector<desc_type>;

  static void add_edge(edges_type &edges, index_t src, index_t dst,
                       float weight) {
    const auto curr_size = static_cast<index_t>(sqrt(ssize(edges)));
    edges[src * curr_size + dst] = weight;
  }
};

template <> struct AdjMatDescTrait<void> {
  using desc_type = int;
  using edges_type = vector<desc_type>;

  static void add_edge(edges_type &edges, index_t src, index_t dst) {
    const auto curr_size = static_cast<index_t>(sqrt(ssize(edges)));
    edges[src * curr_size + dst] = 1;
  }
};

template <typename Weight> struct AdjMatTrait {
  using desc_trait = AdjMatDescTrait<Weight>;
  using desc_type = desc_trait::desc_type;
  using edges_type = desc_trait::edges_type;
  using edge_iter_type = vector<desc_type>::iterator;
  using edge_const_iter_type = vector<desc_type>::const_iterator;
  using edge_range_type = ranges::subrange<edge_iter_type>;
  using const_edge_range_type = ranges::subrange<edge_const_iter_type>;

  static void resize(const edges_type &edges, index_t new_size) {
    auto old_size = curr_size(edges);
    if (new_size < old_size) {
      for (index_t i = 0; i < new_size; ++i) {
        for (index_t j = 0; j < new_size; ++j) {
          edges[i * new_size + j] = edges[i * old_size + j];
        }
      }
    }
    edges.resize(new_size * new_size);
    if (new_size > old_size) {
      for (index_t i = old_size - 1; i >= 0; --i) {
        for (index_t j = old_size - 1; j >= 0; --j) {
          edges[i * new_size + j] = edges[i * old_size + j];
          edges[i * old_size + j] = 0;
        }
      }
    }
  }

  static index_t curr_size(const edges_type &edges) {
    return static_cast<index_t>(sqrt(ssize(edges)));
  }

  static edge_range_type get_edges(edges_type &edges, index_t index) {
    auto sz = curr_size(edges);
    return {edges.begin() + index * sz, edges.begin() + (index + 1) * sz};
  }

  static const_edge_range_type get_edges(const edges_type &edges,
                                         index_t index) {
    auto sz = curr_size(edges);
    return {edges.begin() + index * sz, edges.begin() + (index + 1) * sz};
  }

  static edge_iter_type get_edge(edges_type &edges, index_t src, index_t dst) {
    auto sz = curr_size(edges);
    return edges.begin() + src * sz + dst;
  }

  static edge_const_iter_type get_edge(const edges_type &edges, index_t src,
                                       index_t dst) {
    auto sz = curr_size(edges);
    return edges.begin() + src * sz + dst;
  }
};

struct AdjListTraitTag {
  template <bool Weighted, typename WeightType>
  using edge_trait_type =
      AdjListTrait<conditional_t<Weighted, WeightType, void>>;
};

struct AdjMatTraitTag {
  template <bool Weighted, typename WeightType>
  using edge_trait_type =
      AdjMatTrait<conditional_t<Weighted, WeightType, void>>;
};

template <bool Directed, bool Weighted, typename WeightType,
          typename EdgeTraitTag>
class GraphBase {
public:
  using edge_trait =
      EdgeTraitTag::template edge_trait_type<Weighted, WeightType>;
  using edges_type = edge_trait::edges_type;
  using edge_iter_type = edge_trait::edge_iter_type;
  using edge_const_iter_type = edge_trait::edge_const_iter_type;

private:
  edges_type edges_;

  [[nodiscard]] bool vertex_large(index_t index) const noexcept {
    return index >= size();
  }

  [[nodiscard]] bool vertex_small(index_t index) const noexcept {
    return index < 0;
  }

  [[nodiscard]] bool vertex_invalid(index_t index) const noexcept {
    return vertex_small(index) || vertex_large(index);
  }

public:
  void index_check(index_t src, index_t dst) {
    auto max_index = max(src, dst);
    if (src < 0 || dst < 0) {
      throw invalid_argument("vertex index is negative");
    } else if (vertex_large(max_index)) {
      edge_trait::resize(edges_, max_index + 1);
    }
  }

  void add_edge(index_t src, index_t dst, WeightType w) requires(Weighted) {
    index_check(src, dst);
    edge_trait::desc_trait::add_edge(edges_, src, dst, w);
    if constexpr (!Directed) {
      edge_trait::desc_trait::add_edge(edges_, dst, src, w);
    }
  }

  void add_edge(index_t src, index_t dst) requires(!Weighted) {
    index_check(src, dst);
    edge_trait::desc_trait::add_edge(edges_, src, dst);
    if constexpr (!Directed) {
      edge_trait::desc_trait::add_edge(edges_, dst, src);
    }
  }

  auto edges(index_t src) {
    if (vertex_invalid(src)) {
      throw invalid_argument("vertex does not exist");
    }
    return edge_trait::get_edges(edges_, src);
  }

  auto edges(index_t src) const {
    if (vertex_invalid(src)) {
      throw invalid_argument("vertex does not exist");
    }
    return edge_trait::get_edges(edges_, src);
  }

  auto edge(index_t src, index_t dst) {
    if (vertex_invalid(src) || vertex_invalid(dst)) {
      throw invalid_argument("vertex does not exist");
    }
    return edge_trait::get_edge(edges_, src, dst);
  }

  auto edge(index_t src, index_t dst) const {
    if (vertex_invalid(src) || vertex_invalid(dst)) {
      throw invalid_argument("vertex does not exist");
    }
    return edge_trait::get_edge(edges_, src, dst);
  }

  [[nodiscard]] index_t size() const noexcept {
    return edge_trait::curr_size(edges_);
  }
};

} // namespace detail

template <typename EdgeTraitTag = detail::AdjListTraitTag>
using Graph = detail::GraphBase<false, false, void, EdgeTraitTag>;

template <typename EdgeTraitTag = detail::AdjListTraitTag>
using DiGraph = detail::GraphBase<true, false, void, EdgeTraitTag>;

template <typename W = float, typename EdgeTraitTag = detail::AdjListTraitTag>
using WeightedGraph = detail::GraphBase<false, true, W, EdgeTraitTag>;

template <typename W = float, typename EdgeTraitTag = detail::AdjListTraitTag>
using WeightedDiGraph = detail::GraphBase<true, true, W, EdgeTraitTag>;

} // namespace frozenca::hard

#endif //__CLRS4_GRAPH_H__
