#ifndef __CLRS4_ARTICULATION_BRIDGE_BICC_H__
#define __CLRS4_ARTICULATION_BRIDGE_BICC_H__

#include <common.h>
#include <graph.h>

namespace frozenca {

using namespace std;

template <UndirGraphConcept G>
void dfs_bcc(G &g, const V<G> &u, VertexProperty<V<G>, VisitMark> &visited,
             VertexProperty<V<G>, index_t> &d,
             VertexProperty<V<G>, index_t> &low,
             VertexProperty<V<G>, index_t> &high,
             VertexProperty<V<G>, index_t> &nd,
             VertexProperty<V<G>, optional<V<G>>> &pred,
             VertexProperty<V<G>, unordered_set<V<G>>> &children,
             index_t &time) {
  d[u] = time;
  visited[u] = VisitMark::Visiting;
  nd[u] = 1;
  low[u] = time;
  high[u] = time;
  for (const auto &v : g.adj(u)) {
    if (visited[v] == VisitMark::Unvisited) {
      time++;
      pred[v] = u;
      children[u].insert(v);
      dfs_bcc(g, v, visited, d, low, high, nd, pred, children, time);
      nd[u] += nd[v];
      low[u] = min(low[u], low[v]);
      high[u] = min(high[u], high[v]);
    } else if (pred[u] != v) {
      low[u] = min(low[u], low[v]);
      high[u] = min(high[u], high[v]);
    }
  }
  visited[u] = VisitMark::Visited;
}

template <UndirGraphConcept G>
void dfs_bcc_cloned(
    G &g, VertexProperty<V<G>, VisitMark> &visited,
    unordered_set<EdgePair<V<G>>, Hash<EdgePair<V<G>>>> &curr_bcc,
    const V<G> &origin, const V<G> &source, bool &quit) {
  visited[source] = VisitMark::Visiting;
  while (true) {
    if (g.adj(source).empty() || (source == origin && quit)) {
      break;
    }
    const auto &adj = g.adj(source);
    const auto &dest = *adj.begin();
    curr_bcc.emplace(source, dest);
    g.remove_edge(source, dest);
    if (visited[dest] == VisitMark::Unvisited) {
      dfs_bcc_cloned(g, visited, curr_bcc, origin, dest, quit);
    }
    if (dest == origin) {
      quit = true;
    }
  }
}

template <UndirGraphConcept G> decltype(auto) biconnected_components(G &g) {
  auto &visited =
      g.add_vertex_property<VisitMark>(GraphPropertyTag::VertexVisited);
  auto &d = g.add_vertex_property<index_t>(GraphPropertyTag::VertexTime);
  auto &low = g.add_vertex_property<index_t>(GraphPropertyTag::VertexTimeLow);
  auto &high = g.add_vertex_property<index_t>(GraphPropertyTag::VertexTimeHigh);
  auto &nd = g.add_vertex_property<index_t>(GraphPropertyTag::VertexSize);
  auto &pred =
      g.add_vertex_property<optional<V<G>>>(GraphPropertyTag::VertexParent);
  auto &children = g.add_vertex_property<unordered_set<V<G>>>(
      GraphPropertyTag::VertexChildren);
  for (const auto &v : g.vertices()) {
    visited[v] = VisitMark::Unvisited;
    d[v] = 0;
    low[v] = numeric_limits<index_t>::max();
    high[v] = 0;
    nd[v] = 0;
    pred[v] = nullopt;
    children[v].clear();
  }
  index_t time = 0;
  for (const auto &u : g.vertices()) {
    if (visited[u] == VisitMark::Unvisited) {
      dfs_bcc(g, u, visited, d, low, high, nd, pred, children, time);
    }
  }

  auto &articulation_points = g.add_graph_property<unordered_set<V<G>>>(
      GraphPropertyTag::GraphBCCArticulationPoints);

  auto &bridges =
      g.add_graph_property<unordered_set<EdgePair<V<G>>, Hash<EdgePair<V<G>>>>>(
          GraphPropertyTag::GraphBCCBridges);

  auto &bcc = g.add_edge_property<index_t>(GraphPropertyTag::EdgeBcc);

  auto is_articulation_point = [&](const auto &v) {
    if (!pred[v].has_value()) {
      return ssize(children[v]) >= 2;
    } else if (d[v] == low[v] && !children[v].empty()) {
      return true;
    } else {
      return ranges::any_of(children[v],
                            [&](const auto &ch) { return low[ch] >= d[v]; });
    }
  };

  G g_clone;
  for (const auto &u : g.vertices()) {
    g_clone.add_vertex(u);
    for (const auto &v : g.adj(u)) {
      g_clone.add_edge(u, v);
    }
  }

  index_t bcc_index = 0;

  for (const auto &v : g.vertices()) {
    if (is_articulation_point(v)) {
      articulation_points.insert(v);
    }
    if (pred[v].has_value() && d[v] == low[v] && high[v] < d[v] + nd[v]) {
      bcc_index++;
      bridges.emplace(*pred[v], v);
      g_clone.remove_edge(*pred[v], v);
      bcc[{*pred[v], v}] = bcc_index;
    }
  }

  for (const auto &v : g.vertices()) {
    visited[v] = VisitMark::Unvisited;
  }

  for (const auto &u : g.vertices()) {
    if (visited[u] == VisitMark::Unvisited) {
      unordered_set<EdgePair<V<G>>, Hash<EdgePair<V<G>>>> curr_bcc;
      bool quit = false;
      dfs_bcc_cloned(g_clone, visited, curr_bcc, u, u, quit);
      if (!curr_bcc.empty()) {
        bcc_index++;
        for (const auto &bcc_elem : curr_bcc) {
          bcc[bcc_elem] = bcc_index;
        }
      }
      if (articulation_points.contains(u)) {
        visited[u] = VisitMark::Unvisited;
      }
    }
  }
}

} // namespace frozenca

#endif //__CLRS4_ARTICULATION_BRIDGE_BICC_H__