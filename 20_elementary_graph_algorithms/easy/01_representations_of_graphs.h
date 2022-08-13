#ifndef __CLRS4_REPRESENTATION_GRAPHS_H__
#define __CLRS4_REPRESENTATION_GRAPHS_H__

#include <common.h>
#include <graph.h>

namespace frozenca {

using namespace std;

template <GraphConcept GraphType> auto outdegree(GraphType &g) {
  using V = GraphType::vertex_type;
  auto &outdegrees =
      g.add_vertex_property<index_t>(GraphPropertyTag::VertexOutdegree);
  for (const auto &vertex : g.vertices()) {
    outdegrees[vertex] = ranges::ssize(g.adj(vertex));
  }
  return outdegrees.get();
}

template <GraphConcept GraphType> GraphType transpose(const GraphType &g) {
  GraphType g_t;
  for (const auto &src : g.vertices()) {
    for (const auto &dst : g.adj(src)) {
      g_t.add_edge(dst, src);
    }
  }
  return g_t;
}

template <Descriptor V> UndirGraph<V> refine(const MultiGraph<V> &g) {
  UndirGraph<V> g_rf;
  for (const auto &src : g.vertices()) {
    for (const auto &dst : g.adj(src)) {
      if (src != dst) {
        g_rf.add_edge(src, dst);
      }
    }
  }
  return g_rf;
}

template <Descriptor V> DirGraph<V> square(DirGraph<V> &g) {
  DirGraph<V> g_sq;
  for (const auto &src : g.vertices()) {
    for (const auto &dst : g.adj(src)) {
      g_sq.add_edge(src, dst);
      for (auto nxt : g.adj(dst)) {
        g_sq.add_edge(src, nxt);
      }
    }
  }
  return g_sq;
}

template <Descriptor V> AdjMatDirGraph<V> square(AdjMatDirGraph<V> &g) {
  AdjMatDirGraph<V> g_sq;

  auto &sq_edges = g_sq.edges();
  auto &orig_edges = g.edges();
  const auto n = ssize(g);
  g_sq.add_vertex(static_cast<V>(n - 1));
  for (index_t i = 0; i < n; i++) {
    for (index_t j = 0; j < n; j++) {
      sq_edges[i * n + j] = orig_edges[i * n + j];
      for (index_t k = 0; k < n; k++) {
        if (orig_edges[i * n + k] * orig_edges[k * n + j]) {
          sq_edges[i * n + j] = 1;
          break;
        }
      }
    }
  }

  return g_sq;
}

template <Descriptor V> bool has_universal_sink(AdjMatDirGraph<V> &g) {
  const auto n = ssize(g);
  V i = 0;
  V j = 0;
  while (i < n && j < n) {
    if (!g.has_edge({i, j})) {
      j++;
    } else {
      i++;
    }
  }
  if (i == n) {
    return false;
  } else {
    for (V k = 0; k < n; k++) {
      if (k != i && g.has_edge({i, k})) {
        return false;
      }
    }
  }
  return true;
}

} // namespace frozenca

#endif //__CLRS4_REPRESENTATION_GRAPHS_H__