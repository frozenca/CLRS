#ifndef __CLRS4_BFS_H__
#define __CLRS4_BFS_H__

#include <common.h>
#include <graph.h>
#include <limits>
#include <optional>
#include <queue>

namespace frozenca {

using namespace std;

template <GraphConcept GraphType>
void bfs(GraphType &g, const typename GraphType::vertex_type &src) {
  using V = GraphType::vertex_type;
  auto &visited =
      g.add_vertex_property<VisitMark>(GraphPropertyTag::VertexVisited);
  auto &depth = g.add_vertex_property<index_t>(GraphPropertyTag::VertexDepth);
  auto &pred =
      g.add_vertex_property<optional<V>>(GraphPropertyTag::VertexParent);

  for (const auto &v : g.vertices()) {
    visited[v] = VisitMark::Unvisited;
    depth[v] = numeric_limits<index_t>::max();
    pred[v] = nullopt;
  }
  visited[src] = VisitMark::Visiting;
  depth[src] = 0;

  queue<V> q;
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

template <GraphConcept GraphType>
void print_path(const GraphType &g, const typename GraphType::vertex_type &src,
                const typename GraphType::vertex_type &dst) {
  using V = typename GraphType::vertex_type;
  const auto &pred =
      g.get_vertex_property<optional<V>>(GraphPropertyTag::VertexParent);
  print_path(pred, src, dst);
}

} // namespace frozenca

#endif //__CLRS4_BFS_H__