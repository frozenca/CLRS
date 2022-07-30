#ifndef __CLRS4_LONGEST_SIMPLE_PATH_DAG_H__
#define __CLRS4_LONGEST_SIMPLE_PATH_DAG_H__

#include <cassert>
#include <common.h>
#include <graph_algorithms.h>
#include <limits>
#include <vector>

namespace frozenca {

using namespace std;

template <Descriptor V, Arithmetic W>
float longest_simple_path_dag(WeightedDiGraph<V, W> &g, const V &src,
                              const V &dst) {

  constexpr auto MINF = numeric_limits<W>::lowest();
  for (const auto &vertex : g.vertices()) {
    g(v_dist, vertex) = MINF;
  }
  g(v_dist, src) = 0;

  auto top_sort = topological_sort(g);

  while (!top_sort.empty()) {
    auto curr = top_sort.back();
    top_sort.pop_back();

    if (g(v_dist, curr) != MINF) {
      for (auto [_, next] : g.adj(curr)) {
        auto alt = g(v_dist, curr) + g(e_w, {curr, next});
        if (alt > g(v_dist, next)) {
          g(v_dist, next) = alt;
        }
      }
    }
  }
  return g(v_dist, dst);
}

} // namespace frozenca

#endif //__CLRS4_LONGEST_SIMPLE_PATH_DAG_H__
