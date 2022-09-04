#ifndef __CLRS4_MST_H__
#define __CLRS4_MST_H__

#include <02_getting_started/hard/01_insertion_sort_hard.h>
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

template <UndirGraphConcept G>
void mst_kruskal_discretized(G &g,
                             const UndirEdgeProperty<V<G>, index_t> &weight) {
  g.erase_property(GraphPropertyTag::VertexParent);
  auto &parent = g.add_vertex_property<V<G>>(GraphPropertyTag::VertexParent);
  auto &set_rank = g.add_vertex_property<int>(GraphPropertyTag::VertexRank);
  for (const auto &vertex : g.vertices()) {
    make_set(parent, set_rank, vertex);
  }
  unordered_map<index_t, EdgeSet<G>> weighted_edges;
  for (const auto &u : g.vertices()) {
    for (const auto &v : g.adj(u)) {
      weighted_edges[weight[{u, v}]].emplace(u, v);
    }
  }
  // assumption: min_weight >= 1 && max_weight <= V
  vector<E<G>> sorted_edges;
  for (index_t i = 1; i <= ssize(g); ++i) {
    for (const auto &e : weighted_edges[i]) {
      sorted_edges.push_back(e);
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

template <UndirGraphConcept G>
void mst_kruskal_bucket(G &g, const UndirEdgeProperty<V<G>, float> &weight) {
  g.erase_property(GraphPropertyTag::VertexParent);
  auto &parent = g.add_vertex_property<V<G>>(GraphPropertyTag::VertexParent);
  auto &set_rank = g.add_vertex_property<int>(GraphPropertyTag::VertexRank);
  for (const auto &vertex : g.vertices()) {
    make_set(parent, set_rank, vertex);
  }

  index_t n = 0;
  for (const auto &u : g.vertices()) {
    n += ssize(g.adj(u));
  }
  n /= 2;
  // bucket sort
  vector<E<G>> sorted_edges;
  vector<vector<E<G>>> B(n + 1);
  auto edge_comp = [&](const auto &e1, const auto &e2) {
    return weight[e1] < weight[e2];
  };
  for (const auto &u : g.vertices()) {
    for (const auto &v : g.adj(u)) {
      if (u < v) {
        B[static_cast<index_t>(floor(n * weight[{u, v}]))].emplace_back(u, v);
      }
    }
  }
  for (auto &b : B) {
    ranges::sort(b, edge_comp);
  }
  for (auto &b : B) {
    ranges::move(b, back_inserter(sorted_edges));
  }

  auto &mst = g.add_graph_property<EdgeSet<G>>(GraphPropertyTag::GraphMST);
  for (const auto &[u, v] : sorted_edges) {
    if (find_set(parent, u) != find_set(parent, v)) {
      mst.emplace(u, v);
      link_by_rank(parent, set_rank, u, v);
    }
  }
}

template <UndirGraphConcept G> V<G> select_root(G &g) {
  V<G> r = {};
  for (const auto &v : g.vertices()) {
    if (!g.adj(v).empty()) {
      r = v;
      break;
    }
  }
  return r;
}

template <UndirGraphConcept G, Arithmetic F>
void mst_prim(G &g, const UndirEdgeProperty<V<G>, F> &weight) {
  if (g.empty()) {
    return;
  }
  // assumption: g is connected
  g.erase_property(GraphPropertyTag::VertexParent);
  auto &prev =
      g.add_vertex_property<optional<V<G>>>(GraphPropertyTag::VertexParent);
  auto &dist = g.add_vertex_property<F>(GraphPropertyTag::VertexDistance);
  for (const auto &u : g.vertices()) {
    prev[u] = nullopt;
    dist[u] = numeric_limits<F>::max();
  }
  auto r = select_root(g);
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

template <UndirGraphConcept G>
void mst_prim_discretized(G &g,
                          const UndirEdgeProperty<V<G>, index_t> &weight) {
  if (g.empty()) {
    return;
  }
  // assumption: g is connected
  g.erase_property(GraphPropertyTag::VertexParent);
  auto &prev =
      g.add_vertex_property<optional<V<G>>>(GraphPropertyTag::VertexParent);
  auto &dist = g.add_vertex_property<index_t>(GraphPropertyTag::VertexDistance);
  for (const auto &u : g.vertices()) {
    prev[u] = nullopt;
    dist[u] = numeric_limits<index_t>::max();
  }
  V<G> r;
  for (const auto &v : g.vertices()) {
    if (!g.adj(v).empty()) {
      r = v;
      break;
    }
  }
  dist[r] = 0;

  auto &mst = g.add_graph_property<EdgeSet<G>>(GraphPropertyTag::GraphMST);

  map<index_t, set<V<G>>> dist_map;
  using dist_iter = map<index_t, set<V<G>>>::iterator;

  auto vertex_dist_comp = [&](const auto &it1, const auto &it2) {
    return it1->first > it2->first;
  };
  priority_queue<dist_iter, vector<dist_iter>, decltype(vertex_dist_comp)> pq(
      vertex_dist_comp);
  dist_map[0].insert(r);
  pq.push(dist_map.begin());

  auto &visited =
      g.add_vertex_property<VisitMark>(GraphPropertyTag::VertexVisited);
  while (!pq.empty()) {
    auto &s = pq.top()->second;
    auto u = *s.begin();
    s.erase(u);
    if (s.empty()) {
      pq.pop();
    }
    visited[u] = VisitMark::Visited;
    for (const auto &v : g.adj(u)) {
      auto w = weight[{u, v}];
      if (visited[v] != VisitMark::Visited && w < dist[v]) {
        prev[v] = u;
        dist[v] = w;
        bool to_push = false;
        if (!dist_map.contains(w)) {
          to_push = true;
        }
        dist_map[w].insert(v);
        if (to_push) {
          pq.push(dist_map.find(w));
        }
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