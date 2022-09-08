#ifndef __CLRS4_BELLMAN_FORD_H__
#define __CLRS4_BELLMAN_FORD_H__

#include "sssp.h"
#include <common.h>
#include <graph.h>

namespace frozenca {

using namespace std;

template <DirGraphConcept G, Arithmetic F>
bool bellman_ford(G &g, const DirEdgeProperty<V<G>, F> &w, const V<G> &s) {
  initialize_single_source<G, F>(g, s);
  auto &d = g.get_vertex_property<F>(GraphPropertyTag::VertexDistance);
  auto &pred =
      g.get_vertex_property<optional<V<G>>>(GraphPropertyTag::VertexParent);
  bool change = true;
  for (index_t i = 0; i < ssize(g) - 1; ++i) {
    change = false;
    for (const auto &u : g.vertices()) {
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

template <DirGraphConcept G, Arithmetic F>
bool bellman_ford_relax_inf(G &g, const DirEdgeProperty<V<G>, F> &w,
                            const V<G> &s) {
  initialize_single_source<G, F>(g, s);
  auto &d = g.get_vertex_property<F>(GraphPropertyTag::VertexDistance);
  auto &pred =
      g.get_vertex_property<optional<V<G>>>(GraphPropertyTag::VertexParent);
  bool change = true;
  for (index_t i = 0; i < ssize(g) - 1; ++i) {
    change = false;
    for (const auto &u : g.vertices()) {
      for (const auto &v : g.adj(u)) {
        relax(u, v, w, d, pred, change);
      }
    }
    if (!change) {
      break;
    }
  }
  bool ok = true;
  change = true;
  while (change) {
    change = false;
    for (const auto &u : g.vertices()) {
      for (const auto &v : g.adj(u)) {
        relax_inf(u, v, w, d, pred, change);
        if (change) {
          ok = false;
        }
      }
    }
  }
  return ok;
}

template <DirGraphConcept G, Arithmetic F>
bool bellman_ford_neg_cycle(G &g, const DirEdgeProperty<V<G>, F> &w,
                            const V<G> &s) {
  initialize_single_source<G, F>(g, s);
  auto &d = g.get_vertex_property<F>(GraphPropertyTag::VertexDistance);
  auto &pred =
      g.get_vertex_property<optional<V<G>>>(GraphPropertyTag::VertexParent);
  bool change = true;
  for (index_t i = 0; i < ssize(g) - 1; ++i) {
    change = false;
    for (const auto &u : g.vertices()) {
      for (const auto &v : g.adj(u)) {
        relax(u, v, w, d, pred, change);
      }
    }
    if (!change) {
      break;
    }
  }
  bool ok = true;
  V<G> neg_start = {};
  for (const auto &u : g.vertices()) {
    for (const auto &v : g.adj(u)) {
      if (d[v] > d[u] + w[{u, v}]) {
        ok = false;
        neg_start = v;
        break;
      }
    }
  }
  if (!ok) {
    for (index_t i = 0; i < ssize(g); ++i) {
      neg_start = *pred[neg_start];
    }
    cout << "Detected negative cycle...\n";
    index_t length = 0;
    for (V<G> v = neg_start;; v = *pred[v]) {
      cout << v << ' ';
      length++;
      if (v == neg_start && length > 1) {
        cout << '\n';
        break;
      }
    }
  }
  return ok;
}

} // namespace frozenca

#endif //__CLRS4_BELLMAN_FORD_H__