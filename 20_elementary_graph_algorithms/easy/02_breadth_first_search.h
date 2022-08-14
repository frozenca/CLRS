#ifndef __CLRS4_BFS_H__
#define __CLRS4_BFS_H__

#include <common.h>
#include <graph.h>
#include <limits>
#include <optional>
#include <queue>

namespace frozenca {

using namespace std;

template <GraphConcept G> void init_properties_bfs(G &g) {
  auto &visited =
      g.add_vertex_property<VisitMark>(GraphPropertyTag::VertexVisited);
  auto &depth = g.add_vertex_property<index_t>(GraphPropertyTag::VertexDepth);

  auto &pred =
      g.add_vertex_property<optional<V<G>>>(GraphPropertyTag::VertexParent);
  for (const auto &v : g.vertices()) {
    visited[v] = VisitMark::Unvisited;
    depth[v] = numeric_limits<index_t>::max();
    pred[v] = nullopt;
  }
}

template <GraphConcept G> void init_properties_bipartite(G &g) {
  auto &depth = g.add_vertex_property<index_t>(GraphPropertyTag::VertexDepth);

  auto &pred =
      g.add_vertex_property<optional<V<G>>>(GraphPropertyTag::VertexParent);
  auto &color = g.add_vertex_property<index_t>(GraphPropertyTag::VertexColor);
  g.add_graph_property<pair<Vs<G>, Vs<G>>>(GraphPropertyTag::GraphBipartite);
  for (const auto &v : g.vertices()) {
    depth[v] = numeric_limits<index_t>::max();
    pred[v] = nullopt;
    color[v] = -1;
  }
}

template <GraphConcept G> void bfs(G &g, const V<G> &src) {
  auto &visited =
      g.get_vertex_property<VisitMark>(GraphPropertyTag::VertexVisited);
  auto &depth = g.get_vertex_property<index_t>(GraphPropertyTag::VertexDepth);
  auto &pred =
      g.get_vertex_property<optional<V<G>>>(GraphPropertyTag::VertexParent);

  visited[src] = VisitMark::Visiting;
  depth[src] = 0;

  queue<V<G>> q;
  q.push(src);
  while (!q.empty()) {
    auto u = q.front();
    q.pop();
    for (const auto &v : g.adj(u)) {
      if (visited[v] == VisitMark::Unvisited) {
        visited[v] = VisitMark::Visiting;
        depth[v] = depth[u] + 1;
        pred[v] = u;
        q.push(v);
      }
    }
    visited[u] = VisitMark::Visited;
  }
}

template <Descriptor V>
void print_path(const VertexProperty<V, optional<V>> &parent, const V &s,
                const V &v) {
  if (v == s) {
    cout << s << '\n';
  } else if (!parent[v].has_value()) {
    cout << "No path from " << s << " to " << v << " exists\n";
  } else {
    print_path(parent, s, *parent[v]);
    cout << v << '\n';
  }
}

template <GraphConcept G>
void print_path(const G &g, const V<G> &src, const V<G> &dst) {
  const auto &pred =
      g.get_vertex_property<optional<V<G>>>(GraphPropertyTag::VertexParent);
  print_path(pred, src, dst);
}

template <GraphConcept G> bool bfs_bipartite(G &g, const V<G> &src) {
  auto &depth = g.get_vertex_property<index_t>(GraphPropertyTag::VertexDepth);
  auto &pred =
      g.get_vertex_property<optional<V<G>>>(GraphPropertyTag::VertexParent);
  auto &color = g.get_vertex_property<index_t>(GraphPropertyTag::VertexColor);

  depth[src] = 0;
  color[src] = 0;

  queue<V<G>> q;
  q.push(src);
  while (!q.empty()) {
    auto u = q.front();
    q.pop();
    for (const auto &v : g.adj(u)) {
      if (color[v] == -1) {
        depth[v] = depth[u] + 1;
        pred[v] = u;
        color[v] = 1 - color[u];
        q.push(v);
      } else if (color[v] == color[u]) {
        return false;
      }
    }
  }
  return true;
}

template <UndirGraphConcept G> bool bipartite(G &g) {
  init_properties_bipartite(g);
  auto &color = g.get_vertex_property<index_t>(GraphPropertyTag::VertexColor);
  for (const auto &v : g.vertices()) {
    if (color[v] == -1) {
      if (!bfs_bipartite(g, v)) {
        return false;
      }
    }
  }
  auto &depth = g.get_vertex_property<index_t>(GraphPropertyTag::VertexDepth);
  auto &bipartition = g.get_graph_property<pair<Vs<G>, Vs<G>>>(
      GraphPropertyTag::GraphBipartite);
  for (const auto &v : g.vertices()) {
    if (depth[v] % 2 == 0) {
      bipartition.first.insert(v);
    } else {
      bipartition.second.insert(v);
    }
  }
  return true;
}

template <UndirGraphConcept G> index_t tree_diameter(G &g) {
  init_properties_bfs(g);
  auto start = *g.vertices().begin();
  bfs(g, start);
  auto &depth = g.get_vertex_property<index_t>(GraphPropertyTag::VertexDepth);
  index_t max_d = 0;
  V<G> pivot = start;
  for (const auto &v : g.vertices()) {
    if (max_d < depth[v]) {
      pivot = v;
      max_d = depth[v];
    }
  }
  init_properties_bfs(g);
  bfs(g, pivot);
  max_d = 0;
  for (const auto &v : g.vertices()) {
    if (max_d < depth[v]) {
      pivot = v;
      max_d = depth[v];
    }
  }
  return max_d;
}

} // namespace frozenca

#endif //__CLRS4_BFS_H__