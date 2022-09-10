#ifndef __CLRS4_GABOW_SCALING_H__
#define __CLRS4_GABOW_SCALING_H__

#include "03_dijkstra.h"
#include <bit>
#include <cassert>
#include <common.h>
#include <graph.h>

namespace frozenca {

using namespace std;

template <DirGraphConcept G, integral F>
void dijkstra_gabow(G &g, const DirEdgeProperty<V<G>, F> &w, const V<G> &s) {
  size_t W = 0;
  auto &w_aux = g.add_edge_property<F>(GraphPropertyTag::EdgeWeightAuxiliary);
  for (const auto &[e, val] : w.get()) {
    W = max(W, bit_width(static_cast<uint64_t>(val)));
  }
  assert(W > 0);
  for (const auto &[e, val] : w.get()) {
    w_aux[e] = val >> (W - 1);
  }
  dijkstra_discrete(g, w_aux, s);
  auto &dist = g.get_vertex_property<F>(GraphPropertyTag::VertexDistance);
  auto &dist_aux =
      g.add_vertex_property<F>(GraphPropertyTag::VertexDistanceAuxiliary);
  for (size_t i = 1; i < W; i++) {
    for (const auto &u : g.vertices()) {
      dist_aux[u] = dist[u];
    }
    for (const auto &[e, val] : w.get()) {
      w_aux[e] = (w[e] >> (W - i - 1)) + 2 * dist_aux[e.first] -
                 2 * dist_aux[e.second];
    }
    dijkstra_discrete(g, w_aux, s);
    for (const auto &u : g.vertices()) {
      dist_aux[u] = dist[u] + 2 * dist_aux[u];
    }
  }
  for (const auto &u : g.vertices()) {
    dist[u] = dist_aux[u];
  }
}

} // namespace frozenca

#endif //__CLRS4_GABOW_SCALING_H__