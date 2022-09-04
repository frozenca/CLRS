#ifndef __CLRS4_MST_MAYBE_H__
#define __CLRS4_MST_MAYBE_H__

#include <20_elementary_graph_algorithms/easy/03_depth_first_search.h>
#include <common.h>
#include <graph.h>
#include <set>

namespace frozenca {

using namespace std;

template <UndirGraphConcept G, Arithmetic F>
EdgeSet<G> mst_maybe_A(const G &g, const UndirEdgeProperty<V<G>, F> &weight) {
  auto edge_weight_comp = [&](const auto &e1, const auto &e2) {
    return weight[e1] > weight[e2];
  };
  multiset<E<G>, decltype(edge_weight_comp)> sorted_edges(edge_weight_comp);
  EdgeSet<G> maybe_mst;
  G g_cloned;
  for (const auto &u : g.vertices()) {
    for (const auto &v : g.adj(u)) {
      sorted_edges.emplace(u, v);
      g_cloned.add_edge(u, v);
      maybe_mst.emplace(u, v);
    }
  }
  for (const auto &[u, v] : sorted_edges) {
    g_cloned.remove_edge(u, v);
    if (!is_connected(g_cloned)) {
      g_cloned.add_edge(u, v);
      maybe_mst.erase({u, v});
    }
  }
  return maybe_mst;
}

template <UndirGraphConcept G> EdgeSet<G> mst_maybe_B(const G &g) {
  EdgeSet<G> maybe_mst;
  G g_cloned;
  for (const auto &u : g.vertices()) {
    for (const auto &v : g.adj(u)) {
      g_cloned.add_edge(u, v);
      if (has_cycle(g_cloned)) {
        g_cloned.remove_edge(u, v);
      } else {
        maybe_mst.emplace(u, v);
      }
    }
  }
  return maybe_mst;
}

template <UndirGraphConcept G, Arithmetic F>
EdgeSet<G> mst_maybe_C(const G &g, const UndirEdgeProperty<V<G>, F> &weight) {
  EdgeSet<G> maybe_mst;
  G g_cloned;
  for (const auto &u : g.vertices()) {
    for (const auto &v : g.adj(u)) {
      g_cloned.add_edge(u, v);
      maybe_mst.emplace(u, v);
      auto cycle = get_cycle(g_cloned);
      if (!cycle.empty()) {
        auto e =
            *ranges::max_element(cycle, ranges::less{}, [&](const auto &edge) {
              return weight[edge];
            });
        g_cloned.remove_edge(e.first, e.second);
        maybe_mst.erase(e);
      }
    }
  }
  return maybe_mst;
}

} // namespace frozenca

#endif //__CLRS4_MST_MAYBE_H__