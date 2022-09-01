#ifndef __CLRS4_REACHABILITY_H__
#define __CLRS4_REACHABILITY_H__

#include "05_strongly_connected_components.h"
#include <common.h>
#include <graph.h>

namespace frozenca {

using namespace std;

template <DirGraphConcept G, Arithmetic F>
void get_reachable_minimal(G &g, const VertexProperty<V<G>, F> &weight) {
  auto cg = get_component_graph(g);
  const auto &scc = g.get_vertex_property<index_t>(GraphPropertyTag::VertexScc);
  using iter = G::vertex_iterator_type;
  unordered_map<index_t, iter> scc_min;
  auto end_it = g.vertices().end();
  for (auto i : cg.vertices()) {
    scc_min[i] = end_it;
  }

  for (auto it = g.vertices().begin(); it != end_it; ++it) {
    auto &min_it = scc_min[scc[*it]];
    if (min_it == end_it) {
      min_it = it;
    }
    if (weight[*min_it] > weight[*it]) {
      min_it = it;
    }
  }

  topological_sort(cg);
  auto &top_sort =
      cg.get_graph_property<list<index_t>>(GraphPropertyTag::GraphTopSort);

  auto &link = g.add_vertex_property<V<G>>(GraphPropertyTag::VertexLink);

  while (!top_sort.empty()) {
    auto i = top_sort.front();
    top_sort.pop_front();

    auto min_it_i = scc_min[i];

    for (const auto &j : cg.adj(i)) {
      auto &min_it_j = scc_min[j];
      if (weight[*min_it_i] < weight[*min_it_j]) {
        min_it_j = min_it_i;
      }
    }
  }
  for (const auto &u : g.vertices()) {
    link[u] = *scc_min[scc[u]];
  }
}

} // namespace frozenca

#endif //__CLRS4_REACHABILITY_H__