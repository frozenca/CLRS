#ifndef __CLRS4_DFS_H__
#define __CLRS4_DFS_H__

#include <common.h>
#include <graph.h>

namespace frozenca {

using namespace std;

template <GraphConcept G> void init_properties_dfs(G &g) {
  auto &visited =
      g.add_vertex_property<VisitMark>(GraphPropertyTag::VertexVisited);
  auto &d = g.add_vertex_property<index_t>(GraphPropertyTag::VertexTime);
  auto &f = g.add_vertex_property<index_t>(GraphPropertyTag::VertexTimeFinish);
  auto &pred =
      g.add_vertex_property<optional<V<G>>>(GraphPropertyTag::VertexParent);
  auto &cc = g.add_vertex_property<index_t>(GraphPropertyTag::VertexComponent);
  for (const auto &v : g.vertices()) {
    visited[v] = VisitMark::Unvisited;
    pred[v] = nullopt;
    d[v] = 0;
    f[v] = 0;
    cc[v] = 0;
  }
}

template <GraphConcept G>
void dfs_visit(G &g, const V<G> &u, VertexProperty<V<G>, VisitMark> &visited,
               VertexProperty<V<G>, index_t> &d,
               VertexProperty<V<G>, index_t> &f,
               VertexProperty<V<G>, optional<V<G>>> &pred,
               VertexProperty<V<G>, index_t> &cc, index_t &time,
               index_t &curr_cc, bool print) {
  time++;
  d[u] = time;
  visited[u] = VisitMark::Visiting;
  cc[u] = curr_cc;
  for (const auto &v : g.adj(u)) {
    if (print) {
      if (visited[v] == VisitMark::Unvisited) {
        cout << '{' << u << ',' << v << "} : tree edge\n";
      } else if (visited[v] == VisitMark::Visiting) {
        cout << '{' << u << ',' << v << "} : back edge\n";
      } else if (d[v] > d[u]) {
        cout << '{' << u << ',' << v << "} : forward edge\n";
      } else {
        cout << '{' << u << ',' << v << "} : cross edge\n";
      }
    }
    if (visited[v] == VisitMark::Unvisited) {
      pred[v] = u;
      dfs_visit(g, v, visited, d, f, pred, cc, time, curr_cc, print);
    }
  }
  time++;
  f[u] = time;
  visited[u] = VisitMark::Visited;
}

template <GraphConcept G> void dfs(G &g, bool print = false) {
  init_properties_dfs(g);
  auto &visited =
      g.get_vertex_property<VisitMark>(GraphPropertyTag::VertexVisited);
  auto &d = g.get_vertex_property<index_t>(GraphPropertyTag::VertexTime);
  auto &f = g.get_vertex_property<index_t>(GraphPropertyTag::VertexTimeFinish);
  auto &pred =
      g.get_vertex_property<optional<V<G>>>(GraphPropertyTag::VertexParent);
  auto &cc = g.get_vertex_property<index_t>(GraphPropertyTag::VertexComponent);
  index_t time = 0;
  index_t curr_cc = 0;
  for (const auto &u : g.vertices()) {
    if (visited[u] == VisitMark::Unvisited) {
      curr_cc++;
      dfs_visit(g, u, visited, d, f, pred, cc, time, curr_cc, print);
    }
  }
}

template <GraphConcept G> void dfs_iterative(G &g) {
  init_properties_dfs(g);
  auto &visited =
      g.get_vertex_property<VisitMark>(GraphPropertyTag::VertexVisited);
  auto &d = g.get_vertex_property<index_t>(GraphPropertyTag::VertexTime);
  auto &f = g.get_vertex_property<index_t>(GraphPropertyTag::VertexTimeFinish);
  auto &pred =
      g.get_vertex_property<optional<V<G>>>(GraphPropertyTag::VertexParent);
  index_t time = 0;
  vector<V<G>> s;
  for (const auto &u : g.vertices()) {
    if (visited[u] == VisitMark::Unvisited) {
      s.push_back(u);
      while (!s.empty()) {
        auto v = s.back();
        visited[v] = VisitMark::Visiting;
        if (!d[v]) {
          time++;
          d[v] = time;
        }
        bool finished = true;
        for (const auto &w : g.adj(v)) {
          if (visited[w] == VisitMark::Unvisited) {
            s.push_back(w);
            pred[w] = v;
            finished = false;
          }
        }
        if (finished) {
          s.pop_back();
          time++;
          visited[v] = VisitMark::Visited;
          f[v] = time;
        }
      }
    }
  }
}

template <DirGraphConcept G> bool is_singly_connected(G &g) {
  init_properties_dfs(g);
  auto &visited =
      g.get_vertex_property<VisitMark>(GraphPropertyTag::VertexVisited);
  auto &d = g.get_vertex_property<index_t>(GraphPropertyTag::VertexTime);
  auto &f = g.get_vertex_property<index_t>(GraphPropertyTag::VertexTimeFinish);
  auto &pred =
      g.get_vertex_property<optional<V<G>>>(GraphPropertyTag::VertexParent);
  index_t time = 0;
  vector<V<G>> s;
  for (const auto &u : g.vertices()) {
    if (visited[u] == VisitMark::Unvisited) {
      s.push_back(u);
      while (!s.empty()) {
        auto v = s.back();
        visited[v] = VisitMark::Visiting;
        if (!d[v]) {
          time++;
          d[v] = time;
        }
        bool finished = true;
        for (const auto &w : g.adj(v)) {
          if (visited[w] == VisitMark::Unvisited) {
            s.push_back(w);
            pred[w] = v;
            finished = false;
          } else if (visited[w] == VisitMark::Visiting) {
            return false;
          }
        }
        if (finished) {
          s.pop_back();
          time++;
          visited[v] = VisitMark::Visited;
          f[v] = time;
        }
      }
    }
  }
  return true;
}

} // namespace frozenca

#endif //__CLRS4_DFS_H__