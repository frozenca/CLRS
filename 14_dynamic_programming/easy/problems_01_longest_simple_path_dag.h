#ifndef __CLRS4_LONGEST_SIMPLE_PATH_DAG_H__
#define __CLRS4_LONGEST_SIMPLE_PATH_DAG_H__

#include <cassert>
#include <common.h>
#include <graph.h>
#include <limits>
#include <vector>

namespace frozenca {

using namespace std;

void topological_sort_helper(const WeightedDiGraph<> &g,
                             vector<int> &visited, vector<index_t> &top_sort,
                             index_t i) {
  visited[i] = true;

  for (const auto &[dst, w] : g.edges(i)) {
    if (!visited[dst]) {
      topological_sort_helper(g, visited, top_sort, dst);
    }
  }
  top_sort.push_back(i);
}

float longest_simple_path_dag(const WeightedDiGraph<> &g, index_t src,
                              index_t dst) {
  vector<index_t> top_sort;
  auto V = g.size();
  vector<float> dist(V, numeric_limits<float>::lowest());
  vector<int> visited(V);
  assert(src < V);

  for (index_t i = 0; i < V; ++i) {
    if (!visited[i]) {
      topological_sort_helper(g, visited, top_sort, i);
    }
  }
  dist[src] = 0;

  while (!top_sort.empty()) {
    auto curr = top_sort.back();
    top_sort.pop_back();

    if (dist[curr] != numeric_limits<float>::lowest()) {
      for (const auto &[next, w] : g.edges(curr)) {
        dist[next] = max(dist[next], dist[curr] + w);
      }
    }
  }
  return dist[dst];
}

} // namespace frozenca

#endif //__CLRS4_LONGEST_SIMPLE_PATH_DAG_H__
