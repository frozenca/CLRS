#ifndef __CLRS4_EULER_TOUR_H__
#define __CLRS4_EULER_TOUR_H__

#include <common.h>
#include <graph.h>
#include <list>

namespace frozenca {

using namespace std;

template <GraphConcept G>
void hierholzer(G &g, const V<G> &curr, vector<V<G>> &euler_tour) {
  while (!g.adj(curr).empty()) {
    auto dest = *g.adj(curr).begin();
    g.remove_edge(curr, dest);
    hierholzer(g, dest, euler_tour);
  }
  euler_tour.push_back(curr);
}

template <GraphConcept G> bool euler_tour(G &g) {
  // unchecked assumption: g is connected
  auto &euler_tour =
      g.add_graph_property<vector<V<G>>>(GraphPropertyTag::GraphEulerTour);
  euler_tour.clear();
  if (g.empty()) {
    return true;
  }
  auto &indegree =
      g.add_vertex_property<index_t>(GraphPropertyTag::VertexIndegree);
  auto &outdegree =
      g.add_vertex_property<index_t>(GraphPropertyTag::VertexOutdegree);
  for (const auto &u : g.vertices()) {
    indegree[u] = 0;
    outdegree[u] = 0;
  }
  for (const auto &u : g.vertices()) {
    for (const auto &v : g.adj(u)) {
      indegree[v]++;
      outdegree[u]++;
    }
  }
  for (const auto &u : g.vertices()) {
    if (indegree[u] != outdegree[u]) {
      return false;
    }
  }

  G g_clone;
  for (const auto &u : g.vertices()) {
    g_clone.add_vertex(u);
    for (const auto &v : g.adj(u)) {
      g_clone.add_edge(u, v);
    }
  }
  for (const auto &u : g.vertices()) {
    if (outdegree[u]) {
      hierholzer(g_clone, u, euler_tour);
      break;
    }
  }

  return true;
}

} // namespace frozenca

#endif //__CLRS4_EULER_TOUR_H__