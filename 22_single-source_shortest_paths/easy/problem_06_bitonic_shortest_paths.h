#ifndef __CLRS4_BITONIC_SHORTEST_PATHS_H__
#define __CLRS4_BITONIC_SHORTEST_PATHS_H__

#include "sssp.h"
#include <common.h>
#include <graph.h>
#include <set>

namespace frozenca {

using namespace std;

template <DirGraphConcept G, Arithmetic F>
void bitonic_shortest_path(G &g, const DirEdgeProperty<V<G>, F> &w, const V<G> &s) {
  initialize_single_source<G, F>(g, s);
  auto &d = g.get_vertex_property<F>(GraphPropertyTag::VertexDistance);
  auto &pred =
      g.get_vertex_property<optional<V<G>>>(GraphPropertyTag::VertexParent);
  auto edge_comp = [&](const auto& e1, const auto& e2) {
    return w[e1] < w[e2];
  };
  set<E<G>, decltype(edge_comp)> sorted_edges(edge_comp);
  for (const auto& u : g.vertices()) {
    for (const auto& v : g.adj(u)) {
        sorted_edges.emplace(u, v);
    }
  }
  bool change = false;
  for (const auto& [u, v] : sorted_edges) {
    relax(u, v, w, d, pred, change);
  }
  for (const auto& [u, v] : sorted_edges | views::reverse) {
    relax(u, v, w, d, pred, change);
  }
  for (const auto& [u, v] : sorted_edges) {
    relax(u, v, w, d, pred, change);
  }
  for (const auto& [u, v] : sorted_edges | views::reverse) {
    relax(u, v, w, d, pred, change);
  }
  

}

} // namespace frozenca

#endif //__CLRS4_BITONIC_SHORTEST_PATHS_H__