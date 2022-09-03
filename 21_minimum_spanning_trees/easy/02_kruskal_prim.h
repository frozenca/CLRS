#ifndef __CLRS4_MST_H__
#define __CLRS4_MST_H__

#include <19_data_structures_for_disjoint_sets/easy/union_find.h>
#include <common.h>
#include <graph.h>
#include <optional>
#include <queue>
#include <set>

namespace frozenca {

using namespace std;

template <UndirGraphConcept G, Arithmetic F>
void mst_kruskal(G &g, const UndirEdgeProperty<V<G>, F> &weight) {
  g.erase_property(GraphPropertyTag::VertexParent);
  auto &parent = g.add_vertex_property<V<G>>(GraphPropertyTag::VertexParent);
  auto &set_rank = g.add_vertex_property<int>(GraphPropertyTag::VertexRank);
  for (const auto &vertex : g.vertices()) {
    make_set(parent, set_rank, vertex);
  }
  auto edge_weight_comp = [&](const auto &e1, const auto &e2) {
    return weight[e1] < weight[e2];
  };
  multiset<E<G>, decltype(edge_weight_comp)> sorted_edges(edge_weight_comp);
  for (const auto &u : g.vertices()) {
    for (const auto &v : g.adj(u)) {
      sorted_edges.emplace(u, v);
    }
  }
  auto &mst = g.add_graph_property<EdgeSet<G>>(GraphPropertyTag::GraphMST);
  for (const auto &[u, v] : sorted_edges) {
    if (find_set(parent, u) != find_set(parent, v)) {
      mst.emplace(u, v);
      link_by_rank(parent, set_rank, u, v);
    }
  }
}

template <UndirGraphConcept G, Arithmetic F>
void mst_prim(G &g, const UndirEdgeProperty<V<G>, F> &weight) {
  if (g.empty()) {
    return;
  }
  g.erase_property(GraphPropertyTag::VertexParent);
  auto &prev =
      g.add_vertex_property<optional<V<G>>>(GraphPropertyTag::VertexParent);
  auto &dist = g.add_vertex_property<F>(GraphPropertyTag::VertexDistance);
  for (const auto &u : g.vertices()) {
    prev[u] = nullopt;
    dist[u] = numeric_limits<F>::max();
  }
  const auto &r = *g.vertices().begin();
  dist[r] = 0;

  auto &mst = g.add_graph_property<EdgeSet<G>>(GraphPropertyTag::GraphMST);

  auto vertex_dist_comp = [&](const auto &v1, const auto &v2) {
    return dist[v1] > dist[v2];
  };
  priority_queue<V<G>, vector<V<G>>, decltype(vertex_dist_comp)> pq(
      vertex_dist_comp);
  pq.push(r);

  auto &visited =
      g.add_vertex_property<VisitMark>(GraphPropertyTag::VertexVisited);
  while (!pq.empty()) {
    auto u = pq.top();
    pq.pop();
    visited[u] = VisitMark::Visited;
    for (const auto &v : g.adj(u)) {
      if (visited[v] != VisitMark::Visited && weight[{u, v}] < dist[v]) {
        prev[v] = u;
        dist[v] = weight[{u, v}];
        pq.push(v);
      }
    }
  }
  for (const auto &u : g.vertices()) {
    if (prev[u].has_value()) {
      mst.emplace(u, *prev[u]);
    }
  }
}

} // namespace frozenca

#endif //__CLRS4_MST_H__