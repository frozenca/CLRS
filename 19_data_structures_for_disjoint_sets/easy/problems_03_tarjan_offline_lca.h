#ifndef __CLRS4_OFFLINE_LCA_H__
#define __CLRS4_OFFLINE_LCA_H__

#include <common.h>
#include <graph_algorithms.h>
#include <iostream>

namespace frozenca {

using namespace std;

template <Descriptor V>
void offline_lca(DiGraph<V> &g, const V &u, const V &v) {
  auto &parent = g.add_vertex_property<V>(GraphPropertyTag::VertexParent);
  auto &set_rank = g.add_vertex_property<int>(GraphPropertyTag::VertexRank);
  auto &link = g.add_vertex_property<V>(GraphPropertyTag::VertexLink);
  auto &visited =
      g.add_vertex_property<VisitMark>(GraphPropertyTag::VertexVisited);
  offline_lca_impl(g, parent, set_rank, link, visited, u, v);
}

template <Descriptor V>
void offline_lca_impl(DiGraph<V> &g, VertexProperty<V, V> &parent,
                      VertexProperty<V, int> &set_rank,
                      VertexProperty<V, V> &link,
                      VertexProperty<V, VisitMark> &visited, const V &u,
                      const V &v) {
  make_set_with_link(parent, set_rank, link, u);
  for (const auto &[_, other] : g.adj(u)) {
    offline_lca_impl(g, parent, set_rank, link, visited, other, v);
    link_by_size(parent, set_rank, u, other);
    link[find_set(parent, u)] = u;
  }
  visited[u] = VisitMark::Visited;

  if (visited[v] == VisitMark::Visited) {
    cout << "The lowest common ancestor of " << u << " and " << v << " is "
         << link[find_set(parent, v)] << '\n';
  }
}

} // namespace frozenca

#endif //__CLRS4_OFFLINE_LCA_H__
