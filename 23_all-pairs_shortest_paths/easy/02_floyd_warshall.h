#ifndef __CLRS4_FLOYD_WARSHALL_H__
#define __CLRS4_FLOYD_WARSHALL_H__

#include "01_shortest_paths_matmul.h"
#include <20_elementary_graph_algorithms/easy/05_strongly_connected_components.h>
#include <22_single-source_shortest_paths/easy/03_dijkstra.h>

namespace frozenca {

using namespace std;

template <DirGraphConcept G, Arithmetic F>
void floyd_warshall(G &g, const DirEdgeProperty<V<G>, F> &w) {
  const auto N = ssize(g);
  unordered_map<V<G>, index_t> vertex_index;
  vector<V<G>> index_vertex(N);
  index_t id = 0;
  for (const auto &u : g.vertices()) {
    vertex_index[u] = id;
    index_vertex[id] = u;
    id++;
  }
  auto L = get_weight_matrix(g, w, vertex_index);

  for (index_t k = 0; k < N; k++) {
    for (index_t i = 0; i < N; i++) {
      for (index_t j = 0; j < N; j++) {
        if (L[i * N + k] != numeric_limits<F>::max() &&
            L[k * N + j] != numeric_limits<F>::max()) {
          L[i * N + j] = min(L[i * N + j], L[i * N + k] + L[k * N + j]);
        }
      }
    }
  }

  auto &shortest_paths = g.add_edge_property<F>(GraphPropertyTag::EdgeAPSP);
  for (index_t i = 0; i < N; ++i) {
    for (index_t j = 0; j < N; ++j) {
      shortest_paths[{index_vertex[i], index_vertex[j]}] = L[i * N + j];
    }
  }
}

template <DirGraphConcept G, Arithmetic F>
bool floyd_warshall_negative_cycle(G &g, const DirEdgeProperty<V<G>, F> &w) {
  const auto N = ssize(g);
  unordered_map<V<G>, index_t> vertex_index;
  vector<V<G>> index_vertex(N);
  index_t id = 0;
  for (const auto &u : g.vertices()) {
    vertex_index[u] = id;
    index_vertex[id] = u;
    id++;
  }
  auto L = get_weight_matrix(g, w, vertex_index);

  for (index_t k = 0; k < N; k++) {
    for (index_t i = 0; i < N; i++) {
      for (index_t j = 0; j < N; j++) {
        if (L[i * N + k] != numeric_limits<F>::max() &&
            L[k * N + j] != numeric_limits<F>::max()) {
          L[i * N + j] = min(L[i * N + j], L[i * N + k] + L[k * N + j]);
        }
      }
    }
  }

  auto &shortest_paths = g.add_edge_property<F>(GraphPropertyTag::EdgeAPSP);
  for (index_t i = 0; i < N; ++i) {
    for (index_t j = 0; j < N; ++j) {
      shortest_paths[{index_vertex[i], index_vertex[j]}] = L[i * N + j];
    }
  }

  return has_diagonal_negative(L, N);
}

template <DirGraphConcept G>
vector<char>
get_connectivity_matrix(G &g, unordered_map<V<G>, index_t> &vertex_index) {
  const auto N = ssize(g);
  vector<char> T(N * N);
  for (const auto &u : g.vertices()) {
    T[vertex_index[u] * N + vertex_index[u]] = 1;
    for (const auto &v : g.adj(u)) {
      T[vertex_index[u] * N + vertex_index[v]] = 1;
    }
  }
  return T;
}

template <DirGraphConcept G> void transitive_closure(G &g) {
  const auto N = ssize(g);
  unordered_map<V<G>, index_t> vertex_index;
  vector<V<G>> index_vertex(N);
  index_t id = 0;
  for (const auto &u : g.vertices()) {
    vertex_index[u] = id;
    index_vertex[id] = u;
    id++;
  }
  auto T = get_connectivity_matrix(g, vertex_index);
  for (index_t k = 0; k < N; k++) {
    vector<char> T_(N * N);
    for (index_t i = 0; i < N; i++) {
      for (index_t j = 0; j < N; j++) {
        T_[i * N + j] = T[i * N + j] || (T[i * N + k] && T[k * N + j]);
      }
    }
    swap(T, T_);
  }
  auto &trans_closure =
      g.add_edge_property<bool>(GraphPropertyTag::EdgeTransitiveClosure);
  for (index_t i = 0; i < N; ++i) {
    for (index_t j = 0; j < N; ++j) {
      trans_closure[{index_vertex[i], index_vertex[j]}] = T[i * N + j];
    }
  }
}

template <DirGraphConcept G, Arithmetic F>
void floyd_warshall_path(G &g, const DirEdgeProperty<V<G>, F> &w) {
  const auto N = ssize(g);
  unordered_map<V<G>, index_t> vertex_index;
  vector<V<G>> index_vertex(N);
  index_t id = 0;
  for (const auto &u : g.vertices()) {
    vertex_index[u] = id;
    index_vertex[id] = u;
    id++;
  }
  auto L = get_weight_matrix(g, w, vertex_index);

  auto &pred =
      g.add_edge_property<optional<V<G>>>(GraphPropertyTag::EdgeParent);

  for (index_t i = 0; i < N; ++i) {
    for (index_t j = 0; j < N; ++j) {
      if (i == j || !g.has_edge({index_vertex[i], index_vertex[j]})) {
        pred[{index_vertex[i], index_vertex[j]}] = nullopt;
      } else {
        pred[{index_vertex[i], index_vertex[j]}] = index_vertex[i];
      }
    }
  }

  for (index_t k = 0; k < N; k++) {
    for (index_t i = 0; i < N; i++) {
      for (index_t j = 0; j < N; j++) {
        if (L[i * N + k] != numeric_limits<F>::max() &&
            L[k * N + j] != numeric_limits<F>::max() &&
            L[i * N + j] > L[i * N + k] + L[k * N + j]) {
          L[i * N + j] = L[i * N + k] + L[k * N + j];
          pred[{index_vertex[i], index_vertex[j]}] =
              pred[{index_vertex[k], index_vertex[j]}];
        }
      }
    }
  }

  auto &shortest_paths = g.add_edge_property<F>(GraphPropertyTag::EdgeAPSP);
  for (index_t i = 0; i < N; ++i) {
    for (index_t j = 0; j < N; ++j) {
      shortest_paths[{index_vertex[i], index_vertex[j]}] = L[i * N + j];
    }
  }
  auto &paths = g.add_edge_property<vector<V<G>>>(GraphPropertyTag::EdgePath);
  for (const auto &u : g.vertices()) {
    for (const auto &v : g.vertices()) {
      if (u == v) {
        continue;
      }
      vector<V<G>> curr_path;
      V<G> curr_pred = *pred[{u, v}];
      curr_path.push_back(v);
      curr_path.push_back(curr_pred);
      while (curr_path.back() != u) {
        auto next_pred = *pred[{u, curr_pred}];
        curr_pred = next_pred;
        curr_path.push_back(curr_pred);
      }
      ranges::reverse(curr_path);
      paths[{u, v}] = move(curr_path);
    }
  }
}

template <DirGraphConcept G> void transitive_closure_ve(G &g) {
  const auto N = ssize(g);
  unordered_map<V<G>, index_t> vertex_index;
  vector<V<G>> index_vertex(N);
  index_t id = 0;
  for (const auto &u : g.vertices()) {
    vertex_index[u] = id;
    index_vertex[id] = u;
    id++;
  }
  auto T = get_connectivity_matrix(g, vertex_index);
  auto &wi =
      g.add_edge_property<index_t>(GraphPropertyTag::EdgeWeightAuxiliary);
  index_t E = 0;
  for (const auto &u : g.vertices()) {
    T[vertex_index[u] * N + vertex_index[u]] = 1;
    for (const auto &v : g.adj(u)) {
      T[vertex_index[u] * N + vertex_index[v]] = 1;
      wi[{u, v}] = 1;
      E++;
    }
  }
  for (index_t u = 0; u < N; ++u) {
    dijkstra_discrete(g, wi, index_vertex[u]);
    const auto &dist =
        g.get_vertex_property<index_t>(GraphPropertyTag::VertexDistance);
    for (index_t v = 0; v < N; ++v) {
      T[u * N + v] = (dist[index_vertex[v]] != E + 1);
    }
  }
  auto &trans_closure =
      g.add_edge_property<bool>(GraphPropertyTag::EdgeTransitiveClosure);
  for (index_t i = 0; i < N; ++i) {
    for (index_t j = 0; j < N; ++j) {
      trans_closure[{index_vertex[i], index_vertex[j]}] = T[i * N + j];
    }
  }
}

template <DirGraphConcept G> void transitive_closure_scc(G &g) {
  auto cg = get_component_graph(g);
  transitive_closure(cg);
  const auto &scc = g.get_vertex_property<index_t>(GraphPropertyTag::VertexScc);
  const auto &TSCC =
      cg.get_edge_property<bool>(GraphPropertyTag::EdgeTransitiveClosure);
  auto &trans_closure =
      g.add_edge_property<bool>(GraphPropertyTag::EdgeTransitiveClosure);
  for (const auto &u : g.vertices()) {
    for (const auto &v : g.vertices()) {
      if (scc[u] == scc[v]) {
        trans_closure[{u, v}] = true;
      } else {
        trans_closure[{u, v}] = TSCC[{scc[u], scc[v]}];
      }
    }
  }
}

} // namespace frozenca

#endif //__CLRS4_FLOYD_WARSHALL_H__