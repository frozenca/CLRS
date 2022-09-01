#ifndef __CLRS4_PLANAR_GRAPH_VERTEX_QUERIES_H__
#define __CLRS4_PLANAR_GRAPH_VERTEX_QUERIES_H__

#include <common.h>
#include <graph.h>
#include <queue>
#include <stdexcept>
#include <vector>


namespace frozenca {

using namespace std;

template <Descriptor V>
V planar_newest_neighbor(AdjListUndirGraph<V> &g, const V &v) {
  return g.adj(v).back();
}

inline constexpr index_t planar_max_outdegree = 4;

template <Descriptor V>
void planar_insert_edge(AdjListUndirGraph<V> &g,const V &u_,
                   const V &v_) {
  auto &orientation =
      g.get_edge_property<EdgeOrientation>(GraphPropertyTag::EdgeOrientation);
  auto &outdegree =
      g.get_vertex_property<index_t>(GraphPropertyTag::VertexOutdegree);
  auto u = u_;
  auto v = v_;
  if (u > v) {
    swap(u, v);
  }
  g.add_edge(u, v);
  outdegree[u]++;
  outdegree[v] += 0;
  orientation[{u, v}] = EdgeOrientation::Less;
  if (outdegree[u] == planar_max_outdegree) {
    queue<V> q;
    q.push(u);
    index_t max_iter = 0;
    const index_t curr_size = ssize(g);
    while (!q.empty() && max_iter < curr_size) {
      auto w = q.front();
      q.pop();
      max_iter++;
      for (const auto &x : g.adj(w)) {
        if (w < x && orientation[{w, x}] == EdgeOrientation::Less) {
          orientation[{w, x}] = EdgeOrientation::Greater;
          outdegree[w]--;
          outdegree[x]++;
        } else if (x < w && orientation[{x, w}] == EdgeOrientation::Greater) {
          orientation[{x, w}] = EdgeOrientation::Less;
          outdegree[w]--;
          outdegree[x]++;
        }
        if (outdegree[x] == planar_max_outdegree) {
          q.push(x);
        }
      }
      assert(outdegree[w] == 0);
    }
    if (max_iter == curr_size) {
      throw invalid_argument("This insertion breaks planarity!\n");
    }
  }
}

template <Descriptor V>
void planar_insert(AdjListUndirGraph<V> &g, const V &v,
                   const vector<V> &neighbors) {
  for (const auto &u : neighbors) {
    planar_insert_edge(g, u, v);
  }
}

template <Descriptor V> void planar_initialize(AdjListUndirGraph<V> &g) {
  g.add_edge_property<EdgeOrientation>(GraphPropertyTag::EdgeOrientation);
  g.add_vertex_property<index_t>(GraphPropertyTag::VertexOutdegree);
}

} // namespace frozenca

#endif //__CLRS4_PLANAR_GRAPH_VERTEX_QUERIES_H__