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
bool topological_sort_helper(DirGraph<V> &g,
                             VertexProperty<V, VisitMark> &visited,
                             GraphProperty<list<V>> &top_sort,
                             const V &vertex) {
  visited[vertex] = VisitMark::Visiting;

  for (const auto &dst : g.adj(vertex)) {
    auto status = visited[dst];
    if (status == VisitMark::Unvisited) {
      if (!topological_sort_helper(g, visited, top_sort, dst)) {
        top_sort.get().clear();
        return false;
      }
    } else if (status == VisitMark::Visiting) {
      cerr << "Not a DAG, can't topological sort\n";
      top_sort.get().clear();
      return false;
    }
  }
  visited[vertex] = VisitMark::Visited;
  top_sort.get().push_back(vertex);
  return true;
}

template <Descriptor V> void topological_sort(DirGraph<V> &g) {
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
              const V &vertex) {
  parent[vertex] = vertex;
  set_rank[vertex] = 0;
}

template <Descriptor V>
void make_set_with_link(VertexProperty<V, V> &parent,
                        VertexProperty<V, int> &set_rank,
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
                  VertexProperty<V, int> &set_rank, const V &x, const V &y) {
  auto xr = find_set(parent, x);
  auto yr = find_set(parent, y);
  if (set_rank[xr] > set_rank[yr]) {
    parent[yr] = xr;
  } else {
    parent[xr] = yr;
    if (set_rank[xr] == set_rank[yr]) {
      set_rank[yr] += 1;
    }
  }
}

template <GraphConcept GraphType> void union_find_by_rank(GraphType &g) {
  using V = GraphType::vertex_type;
  auto &parent = g.add_vertex_property<V>(GraphPropertyTag::VertexParent);
  auto &set_rank = g.add_vertex_property<int>(GraphPropertyTag::VertexRank);
  for (const auto &vertex : g.vertices()) {
    make_set(parent, set_rank, vertex);
  }

  for (const auto &v : g.vertices()) {
    for (const auto &u : g.adj(v)) {
      link_by_rank(parent, set_rank, v, u);
    }
  }
}

template <Descriptor V>
void link_by_size(VertexProperty<V, V> &parent,
                  VertexProperty<V, int> &set_rank, const V &x, const V &y) {
  auto xr = find_set(parent, x);
  auto yr = find_set(parent, y);
  if (set_rank[xr] > set_rank[yr]) {
    parent[yr] = xr;
  } else {
    parent[xr] = yr;
    if (set_rank[xr] == set_rank[yr]) {
      set_rank[yr] += set_rank[xr];
    }
  }
}

template <Descriptor V>
void link_by_size_with_link(VertexProperty<V, V> &parent,
                            VertexProperty<V, int> &set_rank,
                            VertexProperty<V, V> &link, const V &x,
                            const V &y) {
  auto xr = find_set(parent, x);
  auto yr = find_set(parent, y);
  swap(link[xr], link[yr]);
  if (set_rank[xr] > set_rank[yr]) {
    parent[yr] = xr;
  } else {
    parent[xr] = yr;
    if (set_rank[xr] == set_rank[yr]) {
      set_rank[yr] += set_rank[xr];
    }
  }
}

template <GraphConcept GraphType> void union_find_by_size(GraphType &g) {
  using V = GraphType::vertex_type;
  auto &parent = g.add_vertex_property<V>(GraphPropertyTag::VertexParent);
  auto &set_rank = g.add_vertex_property<int>(GraphPropertyTag::VertexRank);
  for (const auto &vertex : g.vertices()) {
    make_set(parent, set_rank, vertex);
  }

  for (const auto &v : g.vertices()) {
    for (const auto &u : g.adj(v)) {
      link_by_size(parent, set_rank, v, u);
    }
  }
}

template <GraphConcept GraphType> void union_find_with_link(GraphType &g) {
  using V = GraphType::vertex_type;
  auto &parent = g.add_vertex_property<V>(GraphPropertyTag::VertexParent);
  auto &set_rank = g.add_vertex_property<int>(GraphPropertyTag::VertexRank);
  auto &link = g.add_vertex_property<V>(GraphPropertyTag::VertexLink);
  for (const auto &vertex : g.vertices()) {
    make_set_with_link(parent, set_rank, link, vertex);
  }

  for (const auto &v : g.vertices()) {
    for (const auto &u : g.adj(v)) {
      link_by_size_with_link(parent, set_rank, link, v, u);
    }
  }
}

} // namespace frozenca

#endif //__CLRS4_GRAPH_ALGORITHMS_H__
