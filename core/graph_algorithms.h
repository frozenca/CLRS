#ifndef __CLRS4_GRAPH_ALGORITHMS_H__
#define __CLRS4_GRAPH_ALGORITHMS_H__

#include <common.h>
#include <graph.h>
#include <list>

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

} // namespace frozenca

#endif //__CLRS4_GRAPH_ALGORITHMS_H__
