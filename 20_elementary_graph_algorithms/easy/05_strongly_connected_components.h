#ifndef __CLRS4_SCC_H__
#define __CLRS4_SCC_H__

#include "01_representations_of_graphs.h"
#include "03_depth_first_search.h"
#include "04_topological_sort.h"
#include <common.h>
#include <graph.h>

namespace frozenca {

using namespace std;

template <DirGraphConcept G> void strongly_connected_components(G &g) {
  auto &scc = g.add_vertex_property<index_t>(GraphPropertyTag::VertexScc);
  dfs(g);
  auto g_t = transpose(g);
  const auto &finish_order =
      g.get_graph_property<vector<V<G>>>(GraphPropertyTag::GraphVisitSort);
  dfs(g_t, false, finish_order);
  const auto &cc =
      g_t.get_vertex_property<index_t>(GraphPropertyTag::VertexComponent);
  scc = cc;
}

template <DirGraphConcept G> DirGraph<index_t> get_component_graph(G &g) {
  strongly_connected_components(g);
  const auto &scc = g.get_vertex_property<index_t>(GraphPropertyTag::VertexScc);
  DirGraph<index_t> g_component;
  for (const auto &u : g.vertices()) {
    for (const auto &v : g.adj(u)) {
      if (scc[u] != scc[v]) {
        g_component.add_edge(scc[u], scc[v]);
      }
    }
  }
  return g_component;
}

template <DirGraphConcept G> G scc_preserving_contract(G &g) {
  strongly_connected_components(g);
  const auto &scc = g.get_vertex_property<index_t>(GraphPropertyTag::VertexScc);
  unordered_map<index_t, unordered_set<V<G>>> scc_clusters;
  for (const auto &u : g.vertices()) {
    scc_clusters[scc[u]].insert(u);
  }
  G g_contract;
  for (const auto &[index, cluster] : scc_clusters) {
    for (auto it = cluster.begin(); it != prev(cluster.end()); ++it) {
      g_contract.add_edge(*it, *next(it));
    }
    g_contract.add_edge(*prev(cluster.end()), *cluster.begin());
  }
  for (const auto &u : g.vertices()) {
    for (const auto &v : g.adj(u)) {
      if (scc[u] != scc[v]) {
        g_contract.add_edge(*scc_clusters[scc[u]].begin(),
                            *scc_clusters[scc[v]].begin());
      }
    }
  }
  return g_contract;
}

template <DirGraphConcept G> bool is_semiconnected(G &g) {
  auto cg = get_component_graph(g);
  topological_sort(cg);
  auto &top_sort =
      cg.get_graph_property<list<index_t>>(GraphPropertyTag::GraphTopSort);
  for (auto it = top_sort.begin(); it != prev(top_sort.end()); ++it) {
    if (!cg.has_edge({*it, *next(it)})) {
      return false;
    }
  }
  return true;
}

template <DirGraphConcept G, Arithmetic F>
decltype(auto) get_maxdiff(G &g, const VertexProperty<V<G>, F> &weight) {
  auto cg = get_component_graph(g);
  const auto &scc = g.get_vertex_property<index_t>(GraphPropertyTag::VertexScc);
  using iter = G::vertex_iterator_type;
  unordered_map<index_t, pair<iter, iter>> scc_minmax;
  auto end_it = g.vertices().end();
  for (auto i : cg.vertices()) {
    scc_minmax[i] = {end_it, end_it};
  }

  for (auto it = g.vertices().begin(); it != end_it; ++it) {
    auto &[min_it, max_it] = scc_minmax[scc[*it]];
    if (min_it == end_it && max_it == end_it) {
      min_it = it;
      max_it = it;
    }
    if (weight[*min_it] > weight[*it]) {
      min_it = it;
    }
    if (weight[*max_it] < weight[*it]) {
      max_it = it;
    }
  }

  topological_sort(cg);
  auto &top_sort =
      cg.get_graph_property<list<index_t>>(GraphPropertyTag::GraphTopSort);

  tuple<F, iter, iter> res = {numeric_limits<F>::lowest(), end_it, end_it};

  while (!top_sort.empty()) {
    auto i = top_sort.front();
    top_sort.pop_front();

    auto &[f, min_it, max_it] = res;
    auto [min_it_i, max_it_i] = scc_minmax[i];
    auto diff_i = weight[*max_it_i] - weight[*min_it_i];
    if (f < diff_i) {
      f = diff_i;
      min_it = min_it_i;
      max_it = max_it_i;
    }

    for (const auto &j : cg.adj(i)) {
      auto &[min_it_j, max_it_j] = scc_minmax[j];
      if (weight[*min_it_i] < weight[*min_it_j]) {
        min_it_j = min_it_i;
      }
      if (weight[*max_it_i] > weight[*max_it_j]) {
        max_it_j = max_it_i;
      }
    }
  }
  return res;
}

} // namespace frozenca

#endif //__CLRS4_SCC_H__