#ifndef __CLRS4_MST_DYNAMIC_H__
#define __CLRS4_MST_DYNAMIC_H__

#include <20_elementary_graph_algorithms/easy/03_depth_first_search.h>
#include <common.h>
#include <graph.h>

namespace frozenca {

using namespace std;

template <GraphConcept G, Arithmetic F>
bool mst_relax(G &g, const V<G> &u, VertexProperty<V<G>, VisitMark> &visited,
               VertexProperty<V<G>, optional<V<G>>> &pred,
               UndirEdgeProperty<V<G>, F> &weight, EdgeSet<G> &mst) {
  visited[u] = VisitMark::Visiting;
  for (const auto &v : g.adj(u)) {
    if (!mst.contains({u, v})) {
      continue;
    }
    if (visited[v] == VisitMark::Unvisited) {
      pred[v] = u;
      if (mst_relax(g, v, visited, pred, weight, mst)) {
        return true;
      }
    } else if (visited[v] == VisitMark::Visiting && *pred[u] != v) {
      auto curr = *pred[u];

      E<G> max_edge{curr, u};
      while (curr != u && pred[curr].has_value()) {
        if (weight[max_edge] < weight[{*pred[curr], curr}]) {
          max_edge = {*pred[curr], curr};
        }
        curr = *pred[curr];
      }
      if (weight[max_edge] < weight[{u, v}]) {
        max_edge = {u, v};
      }
      mst.erase(max_edge);
      return true;
    }
  }
  visited[u] = VisitMark::Visited;
  return false;
}

template <UndirGraphConcept G, Arithmetic F>
void mst_update_edge(G &g, UndirEdgeProperty<V<G>, F> &weight, const E<G> &e,
                     F w) {
  // Currently, only updates only if e is not in the MST
  // and weight of e is decreasing
  
  auto &mst = g.get_graph_property<EdgeSet<G>>(GraphPropertyTag::GraphMST);

  if (!mst.contains(e) && weight[e] > w) {
    weight[e] = w;
    init_properties_dfs(g);
    auto &visited =
        g.get_vertex_property<VisitMark>(GraphPropertyTag::VertexVisited);
    auto &pred =
        g.get_vertex_property<optional<V<G>>>(GraphPropertyTag::VertexParent);
    mst.insert(e);
    mst_relax(g, e.first, visited, pred, weight, mst);
  } else {
    weight[e] = w;
  }
}

} // namespace frozenca

#endif //__CLRS4_MST_DYNAMIC_H__