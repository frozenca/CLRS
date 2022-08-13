#ifndef __CLRS4_GRAPH_ALGORITHMS_H__
#define __CLRS4_GRAPH_ALGORITHMS_H__

#include <19_data_structures_for_disjoint_sets/easy/union_find.h>
#include <20_elementary_graph_algorithms/easy/01_representations_of_graph.h>
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

} // namespace frozenca

#endif //__CLRS4_GRAPH_ALGORITHMS_H__
