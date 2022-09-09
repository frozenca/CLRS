#ifndef __CLRS4_BELLMAN_FORD_YEN_H__
#define __CLRS4_BELLMAN_FORD_YEN_H__

#include "sssp.h"
#include <common.h>
#include <graph.h>

namespace frozenca {

using namespace std;

template <DirGraphConcept G, Arithmetic F>
bool bellman_ford_yen(G &g, const DirEdgeProperty<V<G>, F> &w, const V<G> &s) {
  initialize_single_source<G, F>(g, s);
  auto &d = g.get_vertex_property<F>(GraphPropertyTag::VertexDistance);
  auto &pred =
      g.get_vertex_property<optional<V<G>>>(GraphPropertyTag::VertexParent);
  bool change = true;
  for (index_t i = 0; i < (ssize(g) + 1) / 2; ++i) {
    change = false;
    for (const auto &u : g.vertices()) {
      for (const auto &v : g.adj(u)) {
        relax(u, v, w, d, pred, change);
      }
    }
    for (const auto &u : g.vertices() | views::reverse) {
      for (const auto &v : g.adj(u)) {
        relax(u, v, w, d, pred, change);
      }
    }
    if (!change) {
      break;
    }
  }
  for (const auto &u : g.vertices()) {
    for (const auto &v : g.adj(u)) {
      if (d[v] > d[u] + w[{u, v}]) {
        return false;
      }
    }
  }
  return true;
}

} // namespace frozenca

#endif //__CLRS4_BELLMAN_FORD_YEN_H__