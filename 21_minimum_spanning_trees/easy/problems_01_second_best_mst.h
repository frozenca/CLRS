#ifndef __CLRS4_MST_SECOND_H__
#define __CLRS4_MST_SECOND_H__

#include "02_kruskal_prim.h"
#include <bit>
#include <common.h>
#include <graph.h>
#include <limits>
#include <set>
#include <unordered_map>
#include <vector>

namespace frozenca {

using namespace std;

template <UndirGraphConcept G, Arithmetic F>
void mst_traverse(G &g, const V<G> &curr, const optional<V<G>> &curr_parent,
                  index_t curr_depth, F curr_weight,
                  const UndirEdgeProperty<V<G>, F> &weight,
                  VertexProperty<V<G>, vector<pair<F, F>>> &dp,
                  VertexProperty<V<G>, vector<optional<V<G>>>> &parent,
                  VertexProperty<V<G>, index_t> &depth) {
  depth[curr] = curr_depth;
  parent[curr][0] = curr_parent;
  dp[curr][0] = {curr_weight, numeric_limits<F>::lowest()};

  for (const auto &neighbor : g.adj(curr)) {
    if (neighbor != curr_parent) {
      mst_traverse(g, neighbor, curr, curr_depth + 1, weight[{curr, neighbor}],
                   weight, dp, parent, depth);
    }
  }
}

template <Arithmetic F>
pair<F, F> top_two(const pair<F, F> &p1, const pair<F, F> &p2) {
  set<F, greater<>> s;
  s.insert(p1.first);
  s.insert(p1.second);
  s.insert(p2.first);
  s.insert(p2.second);
  return {*s.begin(), *next(s.begin())};
}

template <Descriptor V, Arithmetic F>
pair<F, F> lca_top_two(V u, V v,
                       const VertexProperty<V, vector<pair<F, F>>> &dp,
                       const VertexProperty<V, vector<optional<V>>> &parent,
                       const VertexProperty<V, index_t> &depth) {
  if (depth[u] > depth[v]) {
    swap(u, v);
  }
  const auto H = ssize(dp[u]);
  // depth[u] < depth[v]
  set<int, greater<>> pq;
  auto depth_diff = depth[v] - depth[u];
  index_t vh = 0;
  while (depth_diff) {
    if (depth_diff & 1) {
      pq.insert(dp[v][vh].first);
      pq.insert(dp[v][vh].second);
      while (ssize(pq) > 2) {
        pq.erase(*pq.rbegin());
      }
      v = *parent[v][vh];
    }
    depth_diff >>= 1;
    vh++;
  }
  if (u != v) {
    for (index_t h = H - 1; h >= 0; h--) {
      if (parent[v][h].has_value() && parent[u][h] != parent[v][h]) {
        pq.insert(dp[u][h].first);
        pq.insert(dp[u][h].second);
        pq.insert(dp[v][h].first);
        pq.insert(dp[v][h].second);
        while (ssize(pq) > 2) {
          pq.erase(*pq.rbegin());
        }
        u = *parent[u][h];
        v = *parent[v][h];
      }
    }
    pq.insert(dp[v][0].first);
    pq.insert(dp[v][0].second);
    v = *parent[v][0];
    assert(v != -1);
  }
  assert(u != -1);
  pq.insert(dp[u][0].first);
  pq.insert(dp[u][0].second);
  while (ssize(pq) > 2) {
    pq.erase(*pq.rbegin());
  }
  assert(ssize(pq) >= 2);
  return {*pq.begin(), *next(pq.begin())};
}

template <UndirGraphConcept G, Arithmetic F>
F mst_second_best(G &g, const UndirEdgeProperty<V<G>, F> &weight) {
  mst_kruskal(g, weight);
  auto &mst = g.get_graph_property<EdgeSet<G>>(GraphPropertyTag::GraphMST);
  const auto N = ssize(g);
  if (ssize(mst) < N - 1) {
    cerr << "There is no MST\n";
    return numeric_limits<F>::max();
  }
  const auto H = static_cast<index_t>(bit_width(g.size()));
  auto &dp =
      g.add_vertex_property<vector<pair<F, F>>>(GraphPropertyTag::VertexValue);
  g.erase_property(GraphPropertyTag::VertexParent);
  auto &parent = g.add_vertex_property<vector<optional<V<G>>>>(
      GraphPropertyTag::VertexParent);
  g.erase_property(GraphPropertyTag::VertexDepth);
  auto &depth = g.add_vertex_property<index_t>(GraphPropertyTag::VertexDepth);
  for (const auto &u : g.vertices()) {
    dp[u] = vector<pair<F, F>>(
        H, {numeric_limits<F>::lowest(), numeric_limits<F>::lowest()});
    parent[u] = vector<optional<V<G>>>(H, nullopt);
    depth[u] = 0;
  }
  auto r = select_root(g);
  G g_mst;
  F mst_value = 0;
  for (const auto &e : mst) {
    mst_value += weight[e];
    g_mst.add_edge(e.first, e.second);
  }
  mst_traverse(g_mst, r, nullopt, 1, numeric_limits<F>::lowest(), weight, dp,
               parent, depth);
  // fill sparse table
  for (index_t h = 1; h < H; ++h) {
    for (const auto &v : g.vertices()) {
      if (parent[v][h - 1].has_value()) {
        auto p = *parent[v][h - 1];
        parent[v][h] = parent[p][h - 1];
        dp[v][h] = top_two(dp[v][h - 1], dp[p][h - 1]);
      }
    }
  }

  F second_mst_value = numeric_limits<F>::max();
  for (const auto &u : g.vertices()) {
    for (const auto &v : g.adj(u)) {
      if (!mst.contains({u, v})) {
        auto w = weight[{u, v}];
        auto [w1, w2] = lca_top_two(u, v, dp, parent, depth);
        if (w1 > numeric_limits<F>::lowest() && w > w1) {
          second_mst_value = min(second_mst_value, mst_value + w - w1);
        } else if (w2 > numeric_limits<F>::lowest() && w > w2) {
          second_mst_value = min(second_mst_value, mst_value + w - w2);
        }
      }
    }
  }
  if (second_mst_value == numeric_limits<F>::max()) {
    cerr << "There is no second MST\n";
  }
  return second_mst_value;
}

} // namespace frozenca

#endif //__CLRS4_MST_SECOND_H__