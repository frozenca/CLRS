#ifndef __CLRS4_TOPOLOGICAL_SORT_H__
#define __CLRS4_TOPOLOGICAL_SORT_H__

#include <common.h>
#include <graph.h>
#include <queue>

namespace frozenca {

using namespace std;

template <Descriptor V>
bool topological_sort_helper(DirGraph<V> &g,
                             VertexProperty<V, VisitMark> &visited,
                             list<V> &top_sort, const V &vertex) {
  visited[vertex] = VisitMark::Visiting;

  for (const auto &dst : g.adj(vertex)) {
    auto status = visited[dst];
    if (status == VisitMark::Unvisited) {
      if (!topological_sort_helper(g, visited, top_sort, dst)) {
        top_sort.clear();
        return false;
      }
    } else if (status == VisitMark::Visiting) {
      top_sort.clear();
      return false;
    }
  }
  visited[vertex] = VisitMark::Visited;
  top_sort.push_front(vertex);
  return true;
}

template <Descriptor V> bool topological_sort(DirGraph<V> &g) {
  auto &visited =
      g.add_vertex_property<VisitMark>(GraphPropertyTag::VertexVisited);
  auto &top_sort =
      g.add_graph_property<list<V>>(GraphPropertyTag::GraphTopSort);

  top_sort.clear();
  for (const auto &u : g.vertices()) {
    visited[u] = VisitMark::Unvisited;
  }

  for (const auto &vertex : g.vertices()) {
    if (visited[vertex] == VisitMark::Unvisited) {
      if (!topological_sort_helper(g, visited, top_sort, vertex)) {
        return false;
      }
    }
  }
  return true;
}

template <Descriptor V>
index_t count_simple_paths(DirGraph<V> &g, const V &u, const V &v,
                           VertexProperty<V, index_t> &num_paths) {
  if (u == v) {
    return 1;
  } else if (num_paths[u] != -1) {
    return num_paths[u];
  } else {
    num_paths[u] = 0;
    for (const auto &w : g.adj(u)) {
      num_paths[u] += count_simple_paths(g, w, v, num_paths);
    }
    return num_paths[u];
  }
}

template <Descriptor V>
index_t count_simple_paths(DirGraph<V> &g, const V &u, const V &v) {
  // unchecked assumption: DAG
  auto &num_paths =
      g.add_vertex_property<index_t>(GraphPropertyTag::VertexSize);
  for (const auto &vertex : g.vertices()) {
    num_paths[vertex] = -1;
  }
  return count_simple_paths(g, u, v, num_paths);
}

template <Descriptor V> bool topological_sort_kahn(DirGraph<V> &g) {
  auto &indegree =
      g.add_vertex_property<index_t>(GraphPropertyTag::VertexIndegree);
  auto &top_sort =
      g.add_graph_property<list<V>>(GraphPropertyTag::GraphTopSort);

  top_sort.clear();
  for (const auto &u : g.vertices()) {
    indegree[u] = 0;
  }

  for (const auto &u : g.vertices()) {
    for (const auto &v : g.adj(u)) {
      indegree[v]++;
    }
  }
  queue<V> q;
  for (const auto &u : g.vertices()) {
    if (!indegree[u]) {
      q.push(u);
    }
  }
  while (!q.empty()) {
    auto curr = q.front();
    q.pop();
    top_sort.push_front(curr);
    for (const auto &next : g.adj(curr)) {
      indegree[next]--;
      if (!indegree[next]) {
        q.push(next);
      }
    }
  }
  for (const auto &u : g.vertices()) {
    if (indegree[u]) {
      return false;
    }
  }
  return true;
}

} // namespace frozenca

#endif //__CLRS4_TOPOLOGICAL_SORT_H__