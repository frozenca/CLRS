#ifndef __CLRS4_EULER_TOUR_H__
#define __CLRS4_EULER_TOUR_H__

#include <common.h>
#include <graph.h>
#include <list>

namespace frozenca {

using namespace std;

template <DirGraphConcept G>
list<V<G>> euler_cycle(G &g, list<V<G>> &L, const V<G> &v) {
  list<V<G>> C;
  auto u = v;
  while (!g.adj(u).empty()) {
    auto w = *g.adj(u).begin();
    g.remove_edge(u, w);
    C.push_front(u);
    if (!g.adj(u).empty()) {
      L.emplace_front(u);
    }
    u = w;
  }
  C.reverse();
  return C;
}

template <DirGraphConcept G> bool euler_tour(G &g) {
  // unchecked assumption: g is connected
  auto &euler_tour =
      g.add_graph_property<list<V<G>>>(GraphPropertyTag::GraphEulerTour);
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

  list<V<G>> L;
  const auto &vertices = g.vertices();
  L.emplace_front(*vertices.begin());
  typename list<V<G>>::iterator it = euler_tour.begin();
  while (!L.empty()) {
    auto v = L.front();
    L.pop_front();
    auto C = euler_cycle(g, L, v);
    if (euler_tour.empty()) {
      euler_tour = C;
    } else {
      euler_tour.splice(euler_tour.end(), C);
    }
  }
  return true;
}

} // namespace frozenca

#endif //__CLRS4_EULER_TOUR_H__