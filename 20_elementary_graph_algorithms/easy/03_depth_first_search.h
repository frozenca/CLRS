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
  g.erase_property(GraphPropertyTag::VertexParent);
  auto &pred =
      g.add_vertex_property<optional<V<G>>>(GraphPropertyTag::VertexParent);
  auto &cc = g.add_vertex_property<index_t>(GraphPropertyTag::VertexComponent);
  auto &finish_order =
      g.add_graph_property<vector<V<G>>>(GraphPropertyTag::GraphVisitSort);
  finish_order.clear();
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
               VertexProperty<V<G>, index_t> &cc, vector<V<G>> &finish_order,
               index_t &time, index_t &curr_cc, bool print) {
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
      dfs_visit(g, v, visited, d, f, pred, cc, finish_order, time, curr_cc,
                print);
    }
  }
  time++;
  f[u] = time;
  finish_order.push_back(u);
  visited[u] = VisitMark::Visited;
}

template <GraphConcept G>
void dfs(G &g, bool print = false, const vector<V<G>> &dfs_order = {}) {
  init_properties_dfs(g);
  auto &visited =
      g.get_vertex_property<VisitMark>(GraphPropertyTag::VertexVisited);
  auto &d = g.get_vertex_property<index_t>(GraphPropertyTag::VertexTime);
  auto &f = g.get_vertex_property<index_t>(GraphPropertyTag::VertexTimeFinish);
  auto &pred =
      g.get_vertex_property<optional<V<G>>>(GraphPropertyTag::VertexParent);
  auto &cc = g.get_vertex_property<index_t>(GraphPropertyTag::VertexComponent);
  auto &finish_order =
      g.get_graph_property<vector<V<G>>>(GraphPropertyTag::GraphVisitSort);
  index_t time = 0;
  index_t curr_cc = 0;
  if (dfs_order.empty()) {
    for (const auto &u : g.vertices()) {
      if (visited[u] == VisitMark::Unvisited) {
        curr_cc++;
        dfs_visit(g, u, visited, d, f, pred, cc, finish_order, time, curr_cc,
                  print);
      }
    }
  } else {
    for (const auto &u : dfs_order | views::reverse) {
      if (visited[u] == VisitMark::Unvisited) {
        curr_cc++;
        dfs_visit(g, u, visited, d, f, pred, cc, finish_order, time, curr_cc,
                  print);
      }
    }
  }
}

template <GraphConcept G>
bool dfs_check_cycle(G &g, const V<G> &u,
                     VertexProperty<V<G>, VisitMark> &visited,
                     VertexProperty<V<G>, optional<V<G>>> &pred) {
  visited[u] = VisitMark::Visiting;
  for (const auto &v : g.adj(u)) {
    if (visited[v] == VisitMark::Unvisited) {
      pred[v] = u;
      if (dfs_check_cycle(g, v, visited, pred)) {
        return true;
      }
    } else if (visited[v] == VisitMark::Visiting && *pred[u] != v) {
      return true;
    }
  }
  visited[u] = VisitMark::Visited;
  return false;
}

template <GraphConcept G> bool has_cycle(G &g) {
  init_properties_dfs(g);
  auto &visited =
      g.get_vertex_property<VisitMark>(GraphPropertyTag::VertexVisited);
  auto &pred =
      g.get_vertex_property<optional<V<G>>>(GraphPropertyTag::VertexParent);
  for (const auto &u : g.vertices()) {
    if (visited[u] == VisitMark::Unvisited) {
      if (dfs_check_cycle(g, u, visited, pred)) {
        return true;
      }
    }
  }
  return false;
}

template <GraphConcept G>
EdgeSet<G> dfs_get_cycle(G &g, const V<G> &u,
                         VertexProperty<V<G>, VisitMark> &visited,
                         VertexProperty<V<G>, optional<V<G>>> &pred) {
  visited[u] = VisitMark::Visiting;
  for (const auto &v : g.adj(u)) {
    if (visited[v] == VisitMark::Unvisited) {
      pred[v] = u;
      auto curr_cycle = dfs_get_cycle(g, v, visited, pred);
      if (!curr_cycle.empty()) {
        return curr_cycle;
      }
    } else if (visited[v] == VisitMark::Visiting && *pred[u] != v) {
      EdgeSet<G> cycle;
      cycle.emplace(v, u);
      cycle.emplace(u, *pred[u]);
      auto curr = *pred[u];
      while (curr != v && pred[curr].has_value()) {
        cycle.emplace(curr, *pred[curr]);
        curr = *pred[curr];
      }
      return cycle;
    }
  }
  visited[u] = VisitMark::Visited;
  return {};
}

template <UndirGraphConcept G> EdgeSet<G> get_cycle(G &g) {
  init_properties_dfs(g);
  auto &visited =
      g.get_vertex_property<VisitMark>(GraphPropertyTag::VertexVisited);
  auto &pred =
      g.get_vertex_property<optional<V<G>>>(GraphPropertyTag::VertexParent);
  for (const auto &u : g.vertices()) {
    if (visited[u] == VisitMark::Unvisited) {
      auto curr_cycle = dfs_get_cycle(g, u, visited, pred);
      if (!curr_cycle.empty()) {
        return curr_cycle;
      }
    }
  }
  return {};
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

template <GraphConcept G> bool is_connected(G &g) {
  init_properties_dfs(g);
  auto &visited =
      g.get_vertex_property<VisitMark>(GraphPropertyTag::VertexVisited);
  bool is_first = true;
  for (const auto &u : g.vertices()) {
    if (visited[u] == VisitMark::Unvisited) {
      if (!is_first) {
        return false;
      }
      vector<V<G>> s;
      s.push_back(u);
      while (!s.empty()) {
        auto v = s.back();
        visited[v] = VisitMark::Visiting;
        bool finished = true;
        for (const auto &w : g.adj(v)) {
          if (visited[w] == VisitMark::Unvisited) {
            s.push_back(w);
            finished = false;
          }
        }
        if (finished) {
          s.pop_back();
          visited[v] = VisitMark::Visited;
        }
      }
      is_first = false;
    }
  }
  return true;
}

template <GraphConcept G> bool get_spanning_forest(G &g) {
  auto &visited =
      g.add_vertex_property<VisitMark>(GraphPropertyTag::VertexVisited);
  bool is_first = true;
  bool is_connected = true;
  auto &forest =
      g.add_graph_property<EdgeSet<G>>(GraphPropertyTag::GraphSpanningForest);
  auto &cc_repr =
      g.add_vertex_property<V<G>>(GraphPropertyTag::VertexComponent);
  for (const auto &u : g.vertices()) {
    if (visited[u] == VisitMark::Unvisited) {
      if (!is_first) {
        is_connected = false;
      }
      vector<V<G>> s;
      s.push_back(u);
      visited[u] = VisitMark::Visiting;
      while (!s.empty()) {
        auto v = s.back();
        cc_repr[v] = u;
        bool finished = true;
        for (const auto &w : g.adj(v)) {
          if (visited[w] == VisitMark::Unvisited) {
            forest.emplace(v, w);
            visited[w] = VisitMark::Visiting;
            s.push_back(w);
            finished = false;
          }
        }
        if (finished) {
          s.pop_back();
          visited[v] = VisitMark::Visited;
        }
      }
      is_first = false;
    }
  }
  return is_connected;
}

} // namespace frozenca

#endif //__CLRS4_DFS_H__