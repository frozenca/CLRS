#ifndef __CLRS4_BOTTLENECK_SPANNING_TREE_H__
#define __CLRS4_BOTTLENECK_SPANNING_TREE_H__

#include <20_elementary_graph_algorithms/easy/03_depth_first_search.h>
#include <common.h>
#include <graph.h>
#include <vector>

namespace frozenca {

using namespace std;

template <UndirGraphConcept G, Arithmetic F, ranges::random_access_range R>
pair<ranges::subrange<ranges::iterator_t<R>>,
     ranges::subrange<ranges::iterator_t<R>>>
divide_by_median([[maybe_unused]] const G &g, R &&r,
                 const UndirEdgeProperty<V<G>, F> &weight) {
  auto edge_comp = [&](const auto &e1, const auto &e2) {
    return weight[e1] > weight[e2];
  };
  auto half = ranges::ssize(r) / 2;
  ranges::nth_element(r, r.begin() + half, edge_comp);
  ranges::subrange<ranges::iterator_t<R>> A(r.begin(), r.begin() + half);
  ranges::subrange<ranges::iterator_t<R>> B(r.begin() + half, r.end());
  return {A, B};
}

template <UndirGraphConcept G, Arithmetic F, ranges::random_access_range R>
EdgeSet<G> mbst_impl([[maybe_unused]] const G &g, R &&edges,
                     const UndirEdgeProperty<V<G>, F> &weight) {
  if (ranges::ssize(edges) <= 1) {
    return EdgeSet<G>(edges.begin(), edges.end());
  }
  auto [A, B] = divide_by_median(g, edges, weight);
  G g_b;
  for (const auto &u : g.vertices()) {
    g_b.add_vertex(u);
  }
  for (const auto &e : B) {
    g_b.add_edge(e.first, e.second);
  }
  if (get_spanning_forest(g_b)) {
    return mbst_impl(g, B, weight);
  } else {
    auto &forest = g_b.get_graph_property<EdgeSet<G>>(
        GraphPropertyTag::GraphSpanningForest);
    auto &cc_repr =
        g_b.get_vertex_property<V<G>>(GraphPropertyTag::VertexComponent);
    G g_a;
    auto &weight_a = g_a.add_edge_property<F>(GraphPropertyTag::EdgeWeight);
    auto &edge_origin =
        g_a.add_edge_property<E<G>>(GraphPropertyTag::EdgeOrigin);
    for (const auto &u : g.vertices()) {
      g_a.add_vertex(cc_repr[u]);
    }
    for (const auto &[x, y] : A) {
      auto u = cc_repr[x];
      auto v = cc_repr[y];
      if (u != v &&
          (!g_a.has_edge({u, v}) || weight_a[{u, v}] > weight[{x, y}])) {
        g_a.add_edge(u, v);
        weight_a[{u, v}] = weight[{x, y}];
        edge_origin[{u, v}] = {x, y};
      }
    }
    vector<E<G>> g_a_edges;
    for (const auto &u : g_a.vertices()) {
      for (const auto &v : g_a.adj(u)) {
        if (u < v) {
          g_a_edges.emplace_back(u, v);
        }
      }
    }
    auto a_mbst = mbst_impl(g, g_a_edges, weight_a);
    for (const auto &e : a_mbst) {
      forest.insert(edge_origin[e]);
    }
    return forest;
  }
}

template <UndirGraphConcept G, Arithmetic F>
void mbst(G &g, const UndirEdgeProperty<V<G>, F> &weight) {
  vector<E<G>> edges;
  for (const auto &u : g.vertices()) {
    for (const auto &v : g.adj(u)) {
      if (u < v) {
        edges.emplace_back(u, v);
      }
    }
  }
  auto &bottleneck_mst =
      g.add_graph_property<EdgeSet<G>>(GraphPropertyTag::GraphMBST);
  bottleneck_mst = mbst_impl(g, edges, weight);
}

} // namespace frozenca

#endif //__CLRS4_BOTTLENECK_SPANNING_TREE_H__