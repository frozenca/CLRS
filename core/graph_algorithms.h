#ifndef __CLRS4_GRAPH_ALGORITHMS_H__
#define __CLRS4_GRAPH_ALGORITHMS_H__

#include <common.h>
#include <graph.h>
#include <iostream>
#include <list>
#include <vector>

namespace frozenca {

using namespace std;

template <Descriptor VertexType>
bool topological_sort_helper(DiGraph<VertexType> &g,
                             VertexProperty<VisitMark, VertexType> &visited,
                             GraphProperty<list<VertexType>> &top_sort,
                             const VertexType &vertex) {
  visited(vertex) = VisitMark::Visiting;

  for (const auto &[_, dst] : g.adj(vertex)) {
    auto status = visited(dst);
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
  visited(vertex) = VisitMark::Visited;
  top_sort().push_back(vertex);
  return true;
}

template <Descriptor VertexType> void topological_sort(DiGraph<VertexType> &g) {
  auto &visited =
      g.add_vertex_property<VisitMark>(GraphPropertyTag::VertexVisited);
  auto &top_sort =
      g.add_graph_property<list<VertexType>>(GraphPropertyTag::GraphTopSort);

  for (const auto &vertex : g.vertices()) {
    if (visited(vertex) == VisitMark::Unvisited) {
      topological_sort_helper(g, visited, top_sort, vertex);
    }
  }
}

template <Descriptor VertexType>
void make_set(VertexProperty<VertexType, VertexType> &parent,
              VertexProperty<int, VertexType> &rank,
              VertexProperty<VertexType, VertexType> &link,
              const VertexType &vertex) {
  parent(vertex) = vertex;
  rank(vertex) = 0;
  link(vertex) = vertex;
}

template <Descriptor VertexType>
VertexType find_set(UndirGraph<VertexType> &g,
                    VertexProperty<VertexType, VertexType> &parent,
                    const VertexType &v) {
  if (parent(v) != v) {
    parent(v) = find_set(g, parent, parent(v));
  }
  return parent(v);
}

template <Descriptor VertexType>
VertexType find_set_iterative(UndirGraph<VertexType> &g,
                              VertexProperty<VertexType, VertexType> &parent,
                              const VertexType &v) {
  auto r = v;
  while (parent(v) != v) {
    r = parent(v);
  }
  auto x = v;
  while (parent(x) != r) {
    auto parent = parent(x);
    parent(x) = r;
    x = parent;
  }
  return r;
}

template <Descriptor VertexType>
vector<VertexType> enumerate_set(UndirGraph<VertexType> &g,
                                 const VertexType &v) {
  const auto &link =
      g.get_vertex_property<VertexType>(GraphPropertyTag::VertexLink);
  auto r = v;
  vector<VertexType> res;
  res.push_back(v);
  auto x = v;
  while (link(x) != r) {
    x = link(x);
    res.push_back(x);
  }
  return res;
}

template <Descriptor VertexType>
void link_by_rank(VertexProperty<VertexType, VertexType> &parent,
                  VertexProperty<int, VertexType> &rank,
                  VertexProperty<VertexType, VertexType> &link,

                  const VertexType &x, const VertexType &y) {
  auto temp = link(y);
  link(y) = link(x);
  link(x) = temp;
  if (rank(x) > rank(y)) {
    parent(y) = x;
  } else {
    parent(x) = y;
    if (rank(x) == rank(y)) {
      rank(y) += 1;
    }
  }
}

template <Descriptor VertexType>
void union_find_by_rank(UndirGraph<VertexType> &g) {
  auto &parent =
      g.add_vertex_property<VertexType>(GraphPropertyTag::VertexParent);
  auto &rank = g.add_vertex_property<int>(GraphPropertyTag::VertexRank);
  auto &link = g.add_vertex_property<VertexType>(GraphPropertyTag::VertexLink);
  for (const auto &vertex : g.vertices()) {
    make_set(parent, rank, link, vertex);
  }

  for (const auto &v : g.vertices()) {
    for (const auto &[_, u] : g.adj(v)) {
      auto vr = find_set(g, parent, v);
      auto ur = find_set(g, parent, u);
      link_by_rank(parent, rank, link, vr, ur);
    }
  }
}

template <Descriptor VertexType>
void link_by_size(VertexProperty<VertexType, VertexType> &parent,
                  VertexProperty<int, VertexType> &rank,
                  VertexProperty<VertexType, VertexType> &link,
                  const VertexType &x, const VertexType &y) {
  auto temp = link(y);
  link(y) = link(x);
  link(x) = temp;
  if (rank(x) > rank(y)) {
    parent(y) = x;
  } else {
    parent(x) = y;
    if (rank(x) == rank(y)) {
      rank(y) += rank(x);
    }
  }
}

template <Descriptor VertexType>
void union_find_by_size(UndirGraph<VertexType> &g) {
  auto &parent =
      g.add_vertex_property<VertexType>(GraphPropertyTag::VertexParent);
  auto &rank = g.add_vertex_property<int>(GraphPropertyTag::VertexRank);
  auto &link = g.add_vertex_property<VertexType>(GraphPropertyTag::VertexLink);
  for (const auto &vertex : g.vertices()) {
    make_set(parent, rank, link, vertex);
  }

  for (const auto &v : g.vertices()) {
    for (const auto &[_, u] : g.adj(v)) {
      auto vr = find_set(g, parent, v);
      auto ur = find_set(g, parent, u);
      link_by_size(parent, rank, link, vr, ur);
    }
  }
}

} // namespace frozenca

#endif //__CLRS4_GRAPH_ALGORITHMS_H__
