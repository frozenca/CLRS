#ifndef __CLRS4_SHORTEST_PATHS_MATMUL_H__
#define __CLRS4_SHORTEST_PATHS_MATMUL_H__

#include <bit>
#include <common.h>
#include <graph.h>
#include <unordered_map>
#include <vector>

namespace frozenca {

using namespace std;

template <DirGraphConcept G, Arithmetic F>
vector<F> get_weight_matrix(G &g, const DirEdgeProperty<V<G>, F> &w,
                            unordered_map<V<G>, index_t> &vertex_index) {
  const auto N = ssize(g);
  vector<F> W(N * N, numeric_limits<F>::max());
  for (const auto &u : g.vertices()) {
    W[vertex_index[u] * N + vertex_index[u]] = 0;
    for (const auto &v : g.adj(u)) {
      W[vertex_index[u] * N + vertex_index[v]] = w[{u, v}];
    }
  }
  return W;
}

template <Arithmetic F>
vector<F> extend_shortest_paths(const vector<F> &L, index_t N) {
  vector<F> L_(N * N, numeric_limits<F>::max());
  for (index_t i = 0; i < N; ++i) {
    for (index_t j = 0; j < N; ++j) {
      for (index_t k = 0; k < N; ++k) {
        if (L[i * N + k] != numeric_limits<F>::max() &&
            L[k * N + j] != numeric_limits<F>::max()) {
          L_[i * N + j] = min(L_[i * N + j], L[i * N + k] + L[k * N + j]);
        }
      }
    }
  }
  return L_;
}

template <Arithmetic F>
vector<F> extend_shortest_paths(const vector<F> &L1, const vector<F> &L2,
                                index_t N) {
  vector<F> L_(N * N, numeric_limits<F>::max());
  for (index_t i = 0; i < N; ++i) {
    for (index_t j = 0; j < N; ++j) {
      for (index_t k = 0; k < N; ++k) {
        if (L1[i * N + k] != numeric_limits<F>::max() &&
            L2[k * N + j] != numeric_limits<F>::max()) {
          L_[i * N + j] = min(L_[i * N + j], L1[i * N + k] + L2[k * N + j]);
        }
      }
    }
  }
  return L_;
}

template <DirGraphConcept G, Arithmetic F>
void apsp(G &g, const DirEdgeProperty<V<G>, F> &w) {
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
  index_t m = 1;
  while (m < N - 1) {
    L = extend_shortest_paths(L, N);
    m *= 2;
  }
  auto &shortest_paths = g.add_edge_property<F>(GraphPropertyTag::EdgeAPSP);
  for (index_t i = 0; i < N; ++i) {
    for (index_t j = 0; j < N; ++j) {
      shortest_paths[{index_vertex[i], index_vertex[j]}] = L[i * N + j];
    }
  }
}

template <DirGraphConcept G, Arithmetic F>
void apsp_pred(G &g, const DirEdgeProperty<V<G>, F> &w) {
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
  index_t m = 1;
  while (m < N - 1) {
    L = extend_shortest_paths(L, N);
    m *= 2;
  }
  auto &shortest_paths = g.add_edge_property<F>(GraphPropertyTag::EdgeAPSP);
  for (index_t i = 0; i < N; ++i) {
    for (index_t j = 0; j < N; ++j) {
      shortest_paths[{index_vertex[i], index_vertex[j]}] = L[i * N + j];
    }
  }
  auto &pred =
      g.add_edge_property<optional<V<G>>>(GraphPropertyTag::EdgeParent);
  for (index_t i = 0; i < N; ++i) {
    for (index_t j = 0; j < N; ++j) {
      if (i == j) {
        pred[{index_vertex[i], index_vertex[j]}] = nullopt;
      } else {
        optional<V<G>> p;
        for (index_t k = 0; k < N; ++k) {
          if (g.has_edge({index_vertex[k], index_vertex[j]}) &&
              L[i * N + k] + w[{index_vertex[k], index_vertex[j]}] ==
                  L[i * N + j]) {
            p = index_vertex[k];
          }
        }
        pred[{index_vertex[i], index_vertex[j]}] = p;
      }
    }
  }
}

template <DirGraphConcept G, Arithmetic F>
void apsp_path(G &g, const DirEdgeProperty<V<G>, F> &w) {
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
  index_t m = 1;
  while (m < N - 1) {
    L = extend_shortest_paths(L, N);
    m *= 2;
  }
  auto &shortest_length = g.add_edge_property<F>(GraphPropertyTag::EdgeAPSP);
  for (index_t i = 0; i < N; ++i) {
    for (index_t j = 0; j < N; ++j) {
      shortest_length[{index_vertex[i], index_vertex[j]}] = L[i * N + j];
    }
  }
  auto &pred =
      g.add_edge_property<optional<V<G>>>(GraphPropertyTag::EdgeParent);
  for (index_t i = 0; i < N; ++i) {
    for (index_t j = 0; j < N; ++j) {
      if (i == j) {
        pred[{index_vertex[i], index_vertex[j]}] = nullopt;
      } else {
        optional<V<G>> p;
        for (index_t k = 0; k < N; ++k) {
          if (g.has_edge({index_vertex[k], index_vertex[j]}) &&
              L[i * N + k] + w[{index_vertex[k], index_vertex[j]}] ==
                  L[i * N + j]) {
            p = index_vertex[k];
          }
        }
        pred[{index_vertex[i], index_vertex[j]}] = p;
      }
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

template <Arithmetic F>
bool has_diagonal_negative(const vector<F> &L, index_t N) {
  for (index_t i = 0; i < N; ++i) {
    if (L[i * N + i] < 0) {
      return true;
    }
  }
  return false;
}

template <DirGraphConcept G, Arithmetic F>
bool apsp_has_negative_cycle(G &g, const DirEdgeProperty<V<G>, F> &w) {
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
  index_t m = 1;
  while (m < N - 1) {
    L = extend_shortest_paths(L, N);
    m *= 2;
  }
  return has_diagonal_negative(L, N);
}

template <DirGraphConcept G, Arithmetic F>
index_t min_length_negative_cycle(G &g, const DirEdgeProperty<V<G>, F> &w) {
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
  index_t m = 1;
  vector<vector<F>> Ls;
  while (m <= N && !has_diagonal_negative(L, N)) {
    Ls.push_back(L);
    L = extend_shortest_paths(L, N);
    m *= 2;
  }
  if (m > N && !has_diagonal_negative(L, N)) {
    return -1;
  } else if (m <= 2) {
    return m;
  } else {
    auto low = m / 2;
    auto high = m;
    auto d = m / 4;
    while (d >= 1) {
      auto s = low + d;
      auto bl = static_cast<index_t>(bit_width(static_cast<size_t>(low)) - 1);
      auto dl = static_cast<index_t>(bit_width(static_cast<size_t>(d)) - 1);
      auto L_s = extend_shortest_paths(Ls[bl], Ls[dl], N);
      if (!has_diagonal_negative(L_s, N)) {
        high = s;
      } else {
        low = s;
        d = d / 2;
      }
    }
    return high - 1;
  }
}

} // namespace frozenca

#endif //__CLRS4_SHORTEST_PATHS_MATMUL_H__