#ifndef __CLRS4_ARTICULATION_BRIDGE_BICC_H__
#define __CLRS4_ARTICULATION_BRIDGE_BICC_H__

#include <common.h>
#include <graph.h>

namespace frozenca {

using namespace std;

template <UndirGraphConcept G>
void dfs_bcc(G &g, const V<G> &u, const optional<V<G>> &par, index_t &time,
             VertexProperty<V<G>, index_t> &d,
             VertexProperty<V<G>, index_t> &low, vector<E<G>> &stk,
             vector<vector<E<G>>> &bccs) {
  time++;
  d[u] = time;
  low[u] = time;
  for (const auto &v : g.adj(u)) {
    if (!par.has_value() || v != *par) {
      if (d[u] > d[v]) {
        stk.emplace_back(u, v);
      }
      if (d[v] > 0) {
        low[u] = min(low[u], d[v]);
      } else {
        dfs_bcc(g, v, u, time, d, low, stk, bccs);
        low[u] = min(low[u], low[v]);
        if (low[v] >= d[u]) {
          vector<E<G>> curr_bcc;
          assert(!stk.empty());
          while (stk.back() != E<G>(u, v)) {
            curr_bcc.push_back(stk.back());
            stk.pop_back();
          }
          assert(!stk.empty());
          curr_bcc.push_back(stk.back());
          stk.pop_back();
          bccs.push_back(move(curr_bcc));
        }
      }
    }
  }
}

template <UndirGraphConcept G> void biconnected_components(G &g) {
  auto &d = g.add_vertex_property<index_t>(GraphPropertyTag::VertexTime);
  auto &low = g.add_vertex_property<index_t>(GraphPropertyTag::VertexTimeLow);
  for (const auto &u : g.vertices()) {
    d[u] = 0;
    low[u] = 0;
  }
  vector<E<G>> stk;
  vector<vector<E<G>>> bccs;
  index_t time = 0;
  for (const auto &u : g.vertices()) {
    if (d[u] == 0) {
      dfs_bcc(g, u, nullopt, time, d, low, stk, bccs);
    }
  }
  auto &articulation_points = g.add_graph_property<unordered_set<V<G>>>(
      GraphPropertyTag::GraphBCCArticulationPoints);
  auto &bridges =
      g.add_graph_property<EdgeSet<G>>(GraphPropertyTag::GraphBCCBridges);
  auto &bcc = g.add_edge_property<index_t>(GraphPropertyTag::EdgeBcc);

  unordered_map<V<G>, unordered_set<index_t>> bcc_vertex;
  for (index_t i = 0; i < ssize(bccs); ++i) {
    const auto &curr_bcc = bccs[i];
    if (ssize(curr_bcc) == 1) {
      bridges.insert(curr_bcc[0]);
    }
    for (const auto &[u, v] : curr_bcc) {
      bcc_vertex[u].insert(i);
      bcc_vertex[v].insert(i);
      bcc[{u, v}] = i;
    }
  }
  for (const auto &u : g.vertices()) {
    if (ssize(bcc_vertex[u]) > 1) {
      articulation_points.insert(u);
    }
  }
}

} // namespace frozenca

#endif //__CLRS4_ARTICULATION_BRIDGE_BICC_H__