#ifndef __CLRS4_SSSP_DAG_H__
#define __CLRS4_SSSP_DAG_H__

#include "sssp.h"
#include <20_elementary_graph_algorithms/easy/04_topological_sort.h>
#include <common.h>
#include <graph.h>

namespace frozenca {

using namespace std;

template <DirGraphConcept G, Arithmetic F>
void dag_shortest_paths(G &g, const DirEdgeProperty<V<G>, F> &w,
                        const V<G> &s) {
  // assumption: graph is acyclic
  initialize_single_source<G, F>(g, s);
  topological_sort(g);
  const auto &top_sort =
      g.get_graph_property<list<V<G>>>(GraphPropertyTag::GraphTopSort);
  auto &d = g.get_vertex_property<F>(GraphPropertyTag::VertexDistance);
  auto &pred =
      g.get_vertex_property<optional<V<G>>>(GraphPropertyTag::VertexParent);
  bool change = true;
  for (const auto &u : top_sort) {
    for (const auto &v : g.adj(u)) {
      relax(u, v, w, d, pred, change);
    }
  }
}

} // namespace frozenca

#endif //__CLRS4_SSSP_DAG_H__