#ifndef __CLRS4_GRAPH_ALGORITHMS_H__
#define __CLRS4_GRAPH_ALGORITHMS_H__

#include <common.h>
#include <graph.h>
#include <iostream>
#include <list>
#include <vector>

namespace frozenca {

using namespace std;

template <Descriptor V>
bool topological_sort_helper(DiGraph<V> &g,
                             VertexProperty<V, VisitMark> &visited,
                             GraphProperty<list<V>> &top_sort,
                             const V &vertex) {
  visited[vertex] = VisitMark::Visiting;

  for (const auto &[_, dst] : g.adj(vertex)) {
    auto status = visited[dst];
    if (status == VisitMark::Unvisited) {
      if (!topological_sort_helper(g, visited, top_sort, dst)) {
        top_sort().clear();
        return false;
      }
    } else if (status == VisitMark::Visiting) {
      cerr << "Not a DAG, can't topological sort\n";
      top_sort().clear();
      return false;
    }
  }
  visited[vertex] = VisitMark::Visited;
  top_sort().push_back(vertex);
  return true;
}

template <Descriptor V> void topological_sort(DiGraph<V> &g) {
  auto &visited =
      g.add_vertex_property<VisitMark>(GraphPropertyTag::VertexVisited);
  auto &top_sort =
      g.add_graph_property<list<V>>(GraphPropertyTag::GraphTopSort);

  for (const auto &vertex : g.vertices()) {
    if (visited[vertex] == VisitMark::Unvisited) {
      topological_sort_helper(g, visited, top_sort, vertex);
    }
  }
}

template <Descriptor V>
void make_set(VertexProperty<V, V> &parent, VertexProperty<V, int> &set_rank,
              VertexProperty<V, V> &link, const V &vertex) {
  parent[vertex] = vertex;
  set_rank[vertex] = 0;
  link[vertex] = vertex;
}

template <Descriptor V> V find_set(VertexProperty<V, V> &parent, const V &v) {
  if (parent[v] != v) {
    parent[v] = find_set(parent, parent[v]);
  }
  return parent[v];
}

template <Descriptor V>
V find_set_iterative(VertexProperty<V, V> &parent, const V &v) {
  auto r = v;
  while (parent[v] != v) {
    r = parent[v];
  }
  auto x = v;
  while (parent[x] != r) {
    auto parent = parent[x];
    parent[x] = r;
    x = parent;
  }
  return r;
}

template <GraphConcept GraphType>
auto enumerate_set(const GraphType &g,
                   const typename GraphType::vertex_type &v) {
  using V = GraphType::vertex_type;
  const auto &link = g.get_vertex_property<V>(GraphPropertyTag::VertexLink);
  auto r = v;
  vector<V> res;
  res.push_back(v);
  auto x = v;
  while (link[x] != r) {
    x = link[x];
    res.push_back(x);
  }
  return res;
}

template <Descriptor V>
void link_by_rank(VertexProperty<V, V> &parent,
                  VertexProperty<V, int> &set_rank,
                  VertexProperty<V, V> &link, const V &x, const V &y) {
  swap(link[x], link[y]);
  if (set_rank[x] > set_rank[y]) {
    parent[y] = x;
  } else {
    parent[x] = y;
    if (set_rank[x] == set_rank[y]) {
      set_rank[y] += 1;
    }
  }
}

template <GraphConcept GraphType> void union_find_by_rank(GraphType &g) {
  using V = GraphType::vertex_type;
  auto &parent = g.add_vertex_property<V>(GraphPropertyTag::VertexParent);
  auto &set_rank = g.add_vertex_property<int>(GraphPropertyTag::VertexRank);
  auto &link = g.add_vertex_property<V>(GraphPropertyTag::VertexLink);
  for (const auto &vertex : g.vertices()) {
    make_set(parent, set_rank, link, vertex);
  }

  for (const auto &v : g.vertices()) {
    for (const auto &[_, u] : g.adj(v)) {
      auto vr = find_set(parent, v);
      auto ur = find_set(parent, u);
      link_by_rank(parent, set_rank, link, vr, ur);
    }
  }
}

template <Descriptor V>
void link_by_size(VertexProperty<V, V> &parent,
                  VertexProperty<V, int> &set_rank,
                  VertexProperty<V, V> &link, const V &x, const V &y) {
  swap(link[x], link[y]);
  if (set_rank[x] > set_rank[y]) {
    parent[y] = x;
  } else {
    parent[x] = y;
    if (set_rank[x] == set_rank[y]) {
      set_rank[y] += set_rank[x];
    }
  }
}

template <GraphConcept GraphType> void union_find_by_size(GraphType &g) {
  using V = GraphType::vertex_type;
  auto &parent = g.add_vertex_property<V>(GraphPropertyTag::VertexParent);
  auto &set_rank = g.add_vertex_property<int>(GraphPropertyTag::VertexRank);
  auto &link = g.add_vertex_property<V>(GraphPropertyTag::VertexLink);
  for (const auto &vertex : g.vertices()) {
    make_set(parent, set_rank, link, vertex);
  }

  for (const auto &v : g.vertices()) {
    for (const auto &[_, u] : g.adj(v)) {
      auto vr = find_set(parent, v);
      auto ur = find_set(parent, u);
      link_by_size(parent, set_rank, link, vr, ur);
    }
  }
}

} // namespace frozenca

#endif //__CLRS4_GRAPH_ALGORITHMS_H__
