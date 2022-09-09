#ifndef __CLRS4_ARBITRAGE_H__
#define __CLRS4_ARBITRAGE_H__

#include "01_bellman-ford.h"
#include <cmath>
#include <common.h>
#include <graph.h>

namespace frozenca {

using namespace std;

bool arbitrage(const vector<vector<float>> &rates) {
  const auto n = ssize(rates);
  DirGraph<index_t> g;
  auto &w = g.add_edge_property<float>(GraphPropertyTag::EdgeWeight);
  for (index_t i = 0; i < n; i++) {
    for (index_t j = i + 1; j < n; j++) {
      auto rate = rates[i][j];
      auto weight = -log(rate);
      g.add_edge(i, j);
      w[{i, j}] = weight;
      g.add_edge(j, i);
      w[{j, i}] = -weight;
    }
    g.add_edge(n, i);
    w[{n, i}] = 0;
  }
  return bellman_ford_neg_cycle(g, w, n);
}

} // namespace frozenca

#endif //__CLRS4_ARBITRAGE_H__