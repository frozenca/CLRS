#ifndef __CLRS4_DIJKSTRA_H__
#define __CLRS4_DIJKSTRA_H__

#include "sssp.h"
#include <20_elementary_graph_algorithms/easy/02_breadth_first_search.h>
#include <common.h>
#include <graph.h>
#include <queue>
#include <string>

namespace frozenca {

using namespace std;

template <DirGraphConcept G, Arithmetic F>
void dijkstra(G &g, const DirEdgeProperty<V<G>, F> &w, const V<G> &s) {
  initialize_single_source<G, F>(g, s);
  auto &d = g.get_vertex_property<F>(GraphPropertyTag::VertexDistance);
  auto &pred =
      g.get_vertex_property<optional<V<G>>>(GraphPropertyTag::VertexParent);
  priority_queue<pair<F, V<G>>, vector<pair<F, V<G>>>, greater<>> pq;
  pq.emplace(0, s);
  while (!pq.empty()) {
    auto [cw, u] = pq.top();
    pq.pop();
    if (d[u] < cw) {
      continue;
    }
    for (const auto &v : g.adj(u)) {
      bool change = false;
      relax(u, v, w, d, pred, change);
      if (change) {
        pq.emplace(d[v], v);
      }
    }
  }
}

template <DirGraphConcept G, Arithmetic F>
void reliability(G &g, const DirEdgeProperty<V<G>, F> &w, const V<G> &s) {
  // assumption: 0 <= w(e) <= 1
  auto &d = g.add_vertex_property<F>(GraphPropertyTag::VertexDistance);
  g.erase_property(GraphPropertyTag::VertexParent);
  auto &pred =
      g.add_vertex_property<optional<V<G>>>(GraphPropertyTag::VertexParent);
  for (const auto &v : g.vertices()) {
    d[v] = 0;
    pred[v] = nullopt;
  }
  d[s] = 1.0f;
  priority_queue<pair<F, V<G>>> pq;
  pq.emplace(1.0f, s);
  while (!pq.empty()) {
    auto [cw, u] = pq.top();
    pq.pop();
    if (d[u] > cw) {
      continue;
    }
    for (const auto &v : g.adj(u)) {
      if (d[u] == 0) {
        continue;
      }
      auto alt = d[u] * w[{u, v}];
      if (d[v] < alt) {
        d[v] = alt;
        pred[v] = u;
        pq.emplace(alt, v);
      }
    }
  }
}

template <DirGraphConcept G, integral F>
void dijkstra_bfs_compare(G &g, const DirEdgeProperty<V<G>, F> &w,
                          const V<G> &s) {
  // assmuption: w > 0 and integers
  initialize_single_source<G, F>(g, s);
  auto &d = g.get_vertex_property<F>(GraphPropertyTag::VertexDistance);
  auto &pred =
      g.get_vertex_property<optional<V<G>>>(GraphPropertyTag::VertexParent);
  priority_queue<pair<F, V<G>>, vector<pair<F, V<G>>>, greater<>> pq;
  pq.emplace(0, s);
  while (!pq.empty()) {
    auto [cw, u] = pq.top();
    pq.pop();
    cout << "Dijkstra: visit " << u << '\n';
    if (d[u] < cw) {
      continue;
    }
    for (const auto &v : g.adj(u)) {
      bool change = false;
      relax(u, v, w, d, pred, change);
      if (change) {
        pq.emplace(d[v], v);
      }
    }
  }

  DirGraph<string> g_exp;
  for (const auto &u : g.vertices()) {
    for (const auto &v : g.adj(u)) {
      auto curr_w = w[{u, v}];
      string su;
      su += u;
      string sv;
      sv += v;
      string suv = su + v;
      for (index_t i = 0; i < curr_w; ++i) {
        string src = (i == 0) ? su : (suv + to_string(i));
        string dst = (i == curr_w - 1) ? sv : (suv + to_string(i + 1));
        g_exp.add_edge(src, dst);
      }
    }
  }
  string src;
  src += s;
  bfs(g_exp, src, true);
}

template <DirGraphConcept G, integral F>
void dijkstra_discrete(G &g, const DirEdgeProperty<V<G>, F> &w, const V<G> &s) {
  // assumption: weight is integer in {0, 1, ..., W}
  initialize_single_source<G, F>(g, s);
  auto &d = g.get_vertex_property<F>(GraphPropertyTag::VertexDistance);
  auto &pred =
      g.get_vertex_property<optional<V<G>>>(GraphPropertyTag::VertexParent);
  F W = 0;
  for (const auto &u : g.vertices()) {
    for (const auto &v : g.adj(u)) {
      W = max(W, w[{u, v}]);
    }
  }
  map<F, set<pair<F, V<G>>>> loc;
  loc[0].emplace(0, s);
  index_t pq_sz = 1;
  F curr_mod = 0;
  while (pq_sz) {
    auto it = loc.lower_bound(curr_mod);
    if (it == loc.end()) {
      it = loc.begin();
    }
    auto [cw, u] = *it->second.begin();
    pq_sz--;
    it->second.erase(it->second.begin());
    if (it->second.empty()) {
      loc.erase(it);
    }
    curr_mod = cw % (W + 1);
    if (d[u] > cw) {
      continue;
    }
    for (const auto &v : g.adj(u)) {
      bool change = false;
      relax(u, v, w, d, pred, change);
      if (change) {
        pq_sz++;
        auto next_mod = d[v] % (W + 1);
        loc[next_mod].emplace(d[v], v);
      }
    }
  }
}

template <DirGraphConcept G, Arithmetic F>
void dijkstra_bounded_ratio(G &g, const DirEdgeProperty<V<G>, F> &w,
                            const V<G> &s) {
  // assumption: max_w / min_w is bounded by k
  auto &d = g.add_vertex_property<F>(GraphPropertyTag::VertexDistance);
  g.erase_property(GraphPropertyTag::VertexParent);
  auto &pred =
      g.add_vertex_property<optional<V<G>>>(GraphPropertyTag::VertexParent);
  for (const auto &v : g.vertices()) {
    d[v] = numeric_limits<F>::max();
    pred[v] = nullopt;
  }
  auto &in_queue = g.add_edge_property<bool>(GraphPropertyTag::EdgeVisited);
  auto &aug_weight =
      g.add_edge_property<F>(GraphPropertyTag::EdgeWeightAuxiliary);
  d[s] = 0;
  F min_W = numeric_limits<F>::max();
  F max_W = 0;
  queue<E<G>> q;
  for (const auto &u : g.vertices()) {
    for (const auto &v : g.adj(u)) {
      min_W = min(min_W, w[{u, v}]);
      max_W = min(max_W, w[{u, v}]);
      in_queue[{u, v}] = false;
      aug_weight[{u, v}] = numeric_limits<F>::max();
    }
  }
  for (const auto &u : g.adj(s)) {
    q.push({s, u});
    in_queue[{s, u}] = true;
    aug_weight[{s, u}] = w[{s, u}];
  }
  while (!q.empty()) {
    auto [p, v] = q.front();
    q.pop();
    auto w_v = aug_weight[{p, v}] - min_W;
    if (w_v <= 0) {
      if (d[p] + w[{p, v}] < d[v]) {
        d[v] = d[p] + w[{p, v}];
        pred[v] = p;
      }
      for (const auto &u : g.adj(v)) {
        if (!in_queue[{v, u}]) {
          if (d[v] + w[{v, u}] < d[u]) {
            q.push({v, u});
            in_queue[{v, u}] = true;
            aug_weight[{v, u}] = w[{v, u}] + w_v;
          }
        } else {
          aug_weight[{v, u}] = min(aug_weight[{v, u}], w[{v, u}] + w_v);
        }
      }
    } else if (d[p] + w[{p, v}] < d[v]) {
      q.push({p, v});
      aug_weight[{p, v}] = w_v;
    }
  }
}

} // namespace frozenca

#endif //__CLRS4_DIJKSTRA_H__