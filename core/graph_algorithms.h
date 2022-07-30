#ifndef __CLRS4_GRAPH_ALGORITHMS_H__
#define __CLRS4_GRAPH_ALGORITHMS_H__

#include <common.h>
#include <graph.h>
#include <list>
#include <vector>

namespace frozenca {

using namespace std;

template <DiGraphConcept DiGraphType>
bool topological_sort_helper(DiGraphType &g,
                             list<typename DiGraphType::vertex_type> &top_sort,
                             const typename DiGraphType::vertex_type &vertex) {
  g(v_visited, vertex) = Visited::Visiting;

  for (const auto &[_, dst] : g.adj(vertex)) {
    auto status = g(v_visited, dst);
    if (status == Visited::Unvisited) {
      if (!topological_sort_helper(g, top_sort, dst)) {
        top_sort.clear();
        return false;
      }
    } else if (status == Visited::Visiting) {
      top_sort.clear();
      return false;
    }
  }
  g(v_visited, vertex) = Visited::Visited;
  top_sort.push_back(vertex);
  return true;
}

template <DiGraphConcept DiGraphType> auto topological_sort(DiGraphType &g) {
  using vertex_type = DiGraphType::vertex_type;
  list<vertex_type> top_sort;

  for (const auto &vertex : g.vertices()) {
    if (g(v_visited, vertex) == Visited::Unvisited) {
      topological_sort_helper(g, top_sort, vertex);
    }
  }
  return top_sort;
}

template <Descriptor VertexType>
VertexType find_set(UnionFindGraph<VertexType> &g, const VertexType &v) {
  if (g(v_parent, v) != v) {
    g(v_parent, v) = find_set(g, g(v_parent, v));
  }
  return g(v_parent, v);
}

template <Descriptor VertexType>
VertexType find_set_iterative(UnionFindGraph<VertexType> &g,
                              const VertexType &v) {
  auto r = v;
  while (g(v_parent, v) != v) {
    r = g(v_parent, v);
  }
  auto x = v;
  while (g(v_parent, x) != r) {
    auto parent = g(v_parent, x);
    g(v_parent, x) = r;
    x = parent;
  }
  return r;
}

template <Descriptor VertexType>
vector<VertexType> enumerate_set(UnionFindGraph<VertexType> &g,
                                 const VertexType &v) {
  auto r = v;
  vector<VertexType> res;
  res.push_back(v);
  auto x = v;
  while (g(v_child, x) != r) {
    x = g(v_child, x);
    res.push_back(x);
  }
  return res;
}

template <Descriptor VertexType>
void link_by_rank(UnionFindGraph<VertexType> &g, const VertexType &x,
                  const VertexType &y) {
  auto temp = g(v_child, y);
  g(v_child, y) = g(v_child, x);
  g(v_child, x) = temp;
  if (g(v_rank, x) > g(v_rank, y)) {
    g(v_parent, y) = x;
  } else {
    g(v_parent, x) = y;
    if (g(v_rank, x) == g(v_rank, y)) {
      g(v_rank, y) += 1;
    }
  }
}

template <Descriptor VertexType>
void union_find_by_rank(UnionFindGraph<VertexType> &g) {
  for (const auto &vertex : g.vertices()) {
    g(v_parent, vertex) = vertex;
    g(v_rank, vertex) = 0;
    g(v_child, vertex) = vertex;
  }

  for (const auto &v : g.vertices()) {
    for (const auto &[_, u] : g.adj(v)) {
      link_by_rank(g, find_set(g, v), find_set(g, u));
    }
  }
}

template <Descriptor VertexType>
void link_by_size(UnionFindGraph<VertexType> &g, const VertexType &x,
                  const VertexType &y) {
  auto temp = g(v_child, y);
  g(v_child, y) = g(v_child, x);
  g(v_child, x) = temp;
  if (g(v_rank, x) > g(v_rank, y)) {
    g(v_parent, y) = x;
  } else {
    g(v_parent, x) = y;
    if (g(v_rank, x) == g(v_rank, y)) {
      g(v_rank, y) += g(v_rank, x);
    }
  }
}

template <Descriptor VertexType>
void union_find_by_size(UnionFindGraph<VertexType> &g) {
  for (const auto &vertex : g.vertices()) {
    g(v_parent, vertex) = vertex;
    g(v_rank, vertex) = 0;
    g(v_child, vertex) = vertex;
  }

  for (const auto &v : g.vertices()) {
    for (const auto &[_, u] : g.adj(v)) {
      link_by_size(g, find_set(g, v), find_set(g, u));
    }
  }
}

} // namespace frozenca

#endif //__CLRS4_GRAPH_ALGORITHMS_H__
