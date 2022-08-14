#ifndef __CLRS4_DFS_H__
#define __CLRS4_DFS_H__

#include <common.h>
#include <graph.h>

namespace frozenca {

using namespace std;

template <GraphConcept G> void init_properties_dfs(G &g) {
  auto &visited =
      g.add_vertex_property<VisitMark>(GraphPropertyTag::VertexVisited);
  g.add_vertex_property<index_t>(GraphPropertyTag::VertexTime);
  g.add_vertex_property<index_t>(GraphPropertyTag::VertexTimeFinish);
  auto &pred =
      g.add_vertex_property<optional<V<G>>>(GraphPropertyTag::VertexParent);
  for (const auto &v : g.vertices()) {
    visited[v] = VisitMark::Unvisited;
    pred[v] = nullopt;
  }
}

template <GraphConcept G>
void dfs_visit(G &g, const V<G> &u, VertexProperty<V<G>, VisitMark> &visited,
               VertexProperty<V<G>, index_t> &d,
               VertexProperty<V<G>, index_t> &f,
               VertexProperty<V<G>, optional<V<G>>> &pred, index_t &time) {
  time++;
  d[u] = time;
  visited[u] = VisitMark::Visiting;
  for (const auto &v : g.adj(u)) {
    if (visited[v] == VisitMark::Unvisited) {
      pred[v] = u;
      dfs_visit(g, v, visited, d, f, pred, time);
    }
  }
  time++;
  f[u] = time;
  visited[u] = VisitMark::Visited;
}

template <GraphConcept G> void dfs(G &g) {
  init_properties_dfs(g);
  auto &visited =
      g.get_vertex_property<VisitMark>(GraphPropertyTag::VertexVisited);
  auto &d = g.get_vertex_property<index_t>(GraphPropertyTag::VertexTime);
  auto &f = g.get_vertex_property<index_t>(GraphPropertyTag::VertexTimeFinish);
  auto &pred =
      g.get_vertex_property<optional<V<G>>>(GraphPropertyTag::VertexParent);
  index_t time = 0;
  for (const auto &u : g.vertices()) {
    if (visited[u] == VisitMark::Unvisited) {
      dfs_visit(g, u, visited, d, f, pred, time);
    }
  }
}

} // namespace frozenca

#endif //__CLRS4_DFS_H__