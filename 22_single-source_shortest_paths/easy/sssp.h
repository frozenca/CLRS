#ifndef __CLRS4_SSSP_H__
#define __CLRS4_SSSP_H__

#include <common.h>
#include <graph.h>
#include <limits>

namespace frozenca {

using namespace std;

template <DirGraphConcept G, Arithmetic F>
void initialize_single_source(G &g, const V<G> &s) {
  auto &d = g.add_vertex_property<F>(GraphPropertyTag::VertexDistance);
  g.erase_property(GraphPropertyTag::VertexParent);
  auto &pred =
      g.add_vertex_property<optional<V<G>>>(GraphPropertyTag::VertexParent);
  for (const auto &v : g.vertices()) {
    d[v] = numeric_limits<F>::max();
    pred[v] = nullopt;
  }
  d[s] = 0;
}

template <DirGraphConcept G, Arithmetic F>
void initialize_single_source_zero(G &g, const V<G> &s) {
  auto &d = g.add_vertex_property<F>(GraphPropertyTag::VertexDistance);
  g.erase_property(GraphPropertyTag::VertexParent);
  auto &pred =
      g.add_vertex_property<optional<V<G>>>(GraphPropertyTag::VertexParent);
  for (const auto &v : g.vertices()) {
    d[v] = 0;
    pred[v] = nullopt;
  }
  d[s] = 0;
}

template <Descriptor V, Arithmetic F>
void relax(const V &u, const V &v, const DirEdgeProperty<V, F> &w,
           VertexProperty<V, F> &d, VertexProperty<V, optional<V>> &pred,
           bool &change) {
  if (d[u] == numeric_limits<F>::max()) {
    return;
  }
  auto alt = d[u] + w[{u, v}];
  if (d[v] > alt) {
    d[v] = alt;
    pred[v] = u;
    change = true;
  }
}

template <Descriptor V, Arithmetic F>
void relax_floor(const V &u, const V &v, const DirEdgeProperty<V, F> &w,
                 VertexProperty<V, F> &d, VertexProperty<V, optional<V>> &pred,
                 bool &change) {
  if (d[u] == numeric_limits<F>::max()) {
    return;
  }
  auto alt = floor(d[u] + w[{u, v}]);
  if (d[v] > alt) {
    d[v] = alt;
    pred[v] = u;
    change = true;
  }
}

template <Descriptor V, Arithmetic F>
void relax_inf(const V &u, const V &v, const DirEdgeProperty<V, F> &w,
               VertexProperty<V, F> &d, VertexProperty<V, optional<V>> &pred,
               bool &change) {
  auto alt = d[u] + w[{u, v}];
  if (d[v] > alt) {
    d[v] = numeric_limits<F>::lowest();
    pred[v] = u;
    change = true;
  }
}

} // namespace frozenca

#endif //__CLRS4_SSSP_H__