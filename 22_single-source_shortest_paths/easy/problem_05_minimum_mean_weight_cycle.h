#ifndef __CLRS4_MINIMUM_MEAN_WEIGHT_CYCLE_H__
#define __CLRS4_MINIMUM_MEAN_WEIGHT_CYCLE_H__

#include <20_elementary_graph_algorithms/easy/03_depth_first_search.h>
#include <common.h>
#include <graph.h>
#include <optional>
#include <queue>
#include <unordered_map>

namespace frozenca {

using namespace std;

template <DirGraphConcept G, Arithmetic F>
optional<long double> minimum_mean_weight_cycle(G &g_,
                                                DirEdgeProperty<V<G>, F> &w_) {
  G g;
  for (const auto &u : g_.vertices()) {
    for (const auto &v : g_.adj(u)) {
      g.add_edge(u, v);
    }
  }
  V<G> src = {};
  if constexpr (is_integral_v<V<G>>) {
    for (const auto &u : g_.vertices()) {
      src = max(src, u + 1);
    }
  } else { // TODO: how to make supersource sanely?
    src = V<G>("NIL");
  }
  auto &w = g.add_edge_property<F>(GraphPropertyTag::EdgeWeight);
  w.get() = w_.get();
  for (const auto &u : g_.vertices()) {
    g.add_edge(src, u);
    w[{src, u}] = 0;
  }
  auto cycle = has_cycle(g);
  if (!cycle) {
    return nullopt;
  }
  unordered_map<V<G>, vector<F>> dp;
  const auto N = ssize(g_);
  for (const auto &u : g.vertices()) {
    dp[u] = vector<F>(N + 1, numeric_limits<F>::max());
  }
  dp[src][0] = 0;
  priority_queue<tuple<F, index_t, V<G>>, vector<tuple<F, index_t, V<G>>>,
                 greater<>>
      pq;
  pq.emplace(0, 0, src);
  while (!pq.empty()) {
    auto [cd, cc, u] = pq.top();
    pq.pop();
    if (cc >= N || dp[u][cc] < cd) {
      continue;
    }
    for (const auto &v : g.adj(u)) {
      auto alt = cd + w[{u, v}];
      if (alt < dp[v][cc + 1]) {
        dp[v][cc + 1] = alt;
        pq.emplace(alt, cc + 1, v);
      }
    }
  }
  long double mu = numeric_limits<long double>::max();
  for (const auto &u : g_.vertices()) { // excluding src
    long double curr_mu = 0.0;
    auto delta_n = dp[u][N];
    for (index_t k = 0; k < N; ++k) {
      auto delta_k = dp[u][k];
      long double r = 1.0l * (delta_n - delta_k) / (N - k);
      curr_mu = max(curr_mu, r);
    }
    mu = min(curr_mu, mu);
  }
  return mu;
}

} // namespace frozenca

#endif //__CLRS4_MINIMUM_MEAN_WEIGHT_CYCLE_H__