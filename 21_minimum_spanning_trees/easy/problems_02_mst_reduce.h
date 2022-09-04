#ifndef __CLRS4_MST_REDUCE_H__
#define __CLRS4_MST_REDUCE_H__

#include "02_kruskal_prim.h"
#include <common.h>
#include <graph.h>

namespace frozenca {

using namespace std;

template <UndirGraphConcept G, Arithmetic F>
void mst_reduce(G &g, const UndirEdgeProperty<V<G>, F> &weight) {
  G g_contract;
  auto &mst = g.add_graph_property<EdgeSet<G>>(GraphPropertyTag::GraphMST);
  mst.clear();
  g.erase_property(GraphPropertyTag::VertexParent);
  auto &parent = g.add_vertex_property<V<G>>(GraphPropertyTag::VertexParent);
  auto &set_rank = g.add_vertex_property<int>(GraphPropertyTag::VertexRank);
  auto &visited =
      g.add_vertex_property<VisitMark>(GraphPropertyTag::VertexVisited);
  for (const auto &vertex : g.vertices()) {
    make_set(parent, set_rank, vertex);
    visited[vertex] = VisitMark::Unvisited;
  }
  for (const auto &u : g.vertices()) {
    if (visited[u] == VisitMark::Unvisited) {
      visited[u] = VisitMark::Visited;
      if (!g.adj(u).empty()) {
        V<G> v;
        F w = numeric_limits<F>::max();
        for (const auto &nxt : g.adj(u)) {
          if (weight[{u, nxt}] < w) {
            w = weight[{u, nxt}];
            v = nxt;
          }
        }
        link_by_rank(parent, set_rank, u, v);
        mst.emplace(u, v);
        visited[v] = VisitMark::Visited;
      }
    }
  }

  auto &edge_origin =
      g_contract.add_edge_property<E<G>>(GraphPropertyTag::EdgeOrigin);
  auto &weight_contract =
      g_contract.add_edge_property<F>(GraphPropertyTag::EdgeWeight);
  auto &mst_contract =
      g_contract.add_graph_property<EdgeSet<G>>(GraphPropertyTag::GraphMST);
  for (const auto &x : g.vertices()) {
    for (const auto &y : g.adj(x)) {
      auto u = find_set(parent, x);
      auto v = find_set(parent, y);
      if (u != v) {
        if (!g_contract.has_edge({u, v})) {
          g_contract.add_edge(u, v);
          edge_origin[{u, v}] = {x, y};
          weight_contract[{u, v}] = weight[{x, y}];
        } else if (weight[{x, y}] < weight_contract[{u, v}]) {
          edge_origin[{u, v}] = {x, y};
          weight_contract[{u, v}] = weight[{x, y}];
        }
      }
    }
  }
  mst_kruskal(g_contract, weight_contract);
  for (const auto &u : g_contract.vertices()) {
    for (const auto &v : g_contract.adj(u)) {
      if (mst_contract.contains({u, v})) {
        mst.insert(edge_origin[{u, v}]);
      }
    }
  }
}

} // namespace frozenca

#endif //__CLRS4_MST_REDUCE_H__