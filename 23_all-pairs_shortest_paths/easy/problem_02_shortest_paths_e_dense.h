#ifndef __CLRS4_SHORTEST_PATHS_E_DENSE_H__
#define __CLRS4_SHORTEST_PATHS_E_DENSE_H__

#include <common.h>
#include <graph.h>

namespace frozenca {

using namespace std;

template <DirGraphConcept G, Arithmetic F>
void johnson_dary(G &g, const DirEdgeProperty<V<G>, F> &w) {
  G g_;
  auto &w_ = g_.add_edge_property<F>(GraphPropertyTag::EdgeWeightAuxiliary);
  V<G> src = {};
  if constexpr (is_integral_v<V<G>>) {
    for (const auto &u : g.vertices()) {
      src = max(src, u + 1);
    }
  } else { // TODO: how to make supersource sanely?
    src = V<G>("NIL");
  }
  for (const auto &u : g.vertices()) {
    for (const auto &v : g.adj(u)) {
      g_.add_edge(u, v);
      w_[{u, v}] = w[{u, v}];
    }
    g_.add_edge(src, u);
    w_[{src, u}] = 0;
  }
  auto res = bellman_ford(g_, w_, src);
  if (!res) {
    cerr << "Has a negative cycle\n";
    return;
  }
  auto &d = g_.get_vertex_property<F>(GraphPropertyTag::VertexDistance);
  auto &w_d = g.add_edge_property<F>(GraphPropertyTag::EdgeWeightAuxiliary);
  for (const auto &u : g.vertices()) {
    for (const auto &v : g.adj(u)) {
      w_d[{u, v}] = w[{u, v}] + d[u] - d[v];
    }
  }
  auto &shortest_paths = g.add_edge_property<F>(GraphPropertyTag::EdgeAPSP);
  for (const auto &u : g.vertices()) {
    dijkstra_dary(g, w_d, u);
    auto &d_d = g.get_vertex_property<F>(GraphPropertyTag::VertexDistance);
    for (const auto &v : g.vertices()) {
      shortest_paths[{u, v}] = d_d[v] + d[v] - d[u];
    }
  }
}

} // namespace frozenca

#endif //__CLRS4_SHORTEST_PATHS_E_DENSE_H__