#ifndef __CLRS4_LONGEST_SIMPLE_PATH_DAG_H__
#define __CLRS4_LONGEST_SIMPLE_PATH_DAG_H__

#include <cassert>
#include <common.h>
#include <graph_algorithms.h>
#include <limits>
#include <vector>

namespace frozenca {

using namespace std;

template <Descriptor V>
float longest_simple_path_dag(DirGraph<V> &g, const V &src, const V &dst) {

  auto &weight = g.add_edge_property<float>(GraphPropertyTag::EdgeWeight);
  auto &dist = g.add_vertex_property<float>(GraphPropertyTag::VertexDistance);

  constexpr auto MINF = numeric_limits<float>::lowest();
  for (const auto &vertex : g.vertices()) {
    dist(vertex) = MINF;
  }
  dist(src) = 0;

  topological_sort(g);

  auto top_sort =
      g.get_graph_property<list<V>>(GraphPropertyTag::GraphTopSort).get();

  while (!top_sort.empty()) {
    auto curr = top_sort.back();
    top_sort.pop_back();

    if (dist(curr) != MINF) {
      for (const auto &next : g.adj(curr)) {
        auto alt = dist(curr) + weight({curr, next});
        if (alt > dist(next)) {
          dist(next) = alt;
        }
      }
    }
  }
  return dist(dst);
}

} // namespace frozenca

#endif //__CLRS4_LONGEST_SIMPLE_PATH_DAG_H__
