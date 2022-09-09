#ifndef __CLRS4_NESTING_BOXES_H__
#define __CLRS4_NESTING_BOXES_H__

#include <22_single-source_shortest_paths/easy/02_sssp_dag.h>
#include <array>
#include <common.h>
#include <graph.h>
#include <list>

namespace frozenca {

using namespace std;

template <Arithmetic F, index_t d> using Box = array<F, d>;

template <Arithmetic F, index_t d>
auto operator<=>(const Box<F, d> &b1, const Box<F, d> &b2) {
  bool b1_in_b2 = true;
  bool b2_in_b1 = true;
  for (index_t i = 0; i < d; i++) {
    if (b1[i] > b2[i]) {
      b1_in_b2 = false;
    } else if (b1[i] < b2[i]) {
      b2_in_b1 = false;
    } else {
      b1_in_b2 = false;
      b2_in_b1 = false;
    }
  }
  if (b1_in_b2) {
    return 1;
  } else if (b2_in_b1) {
    return -1;
  } else {
    return 0;
  }
}

void construct_path(VertexProperty<index_t, optional<index_t>> &pred,
                    index_t src, index_t dst, list<index_t> &path) {
  if (dst == src) {
    path.push_front(src);
  } else if (pred[dst].has_value()) {
    construct_path(pred, src, *pred[dst], path);
    path.push_back(dst);
  }
}

template <Arithmetic F, index_t d>
vector<Box<F, d>> longest_nesting_boxes(vector<Box<F, d>> &boxes) {
  const auto N = ssize(boxes);

  for (index_t i = 0; i < N; ++i) {
    ranges::sort(boxes[i]);
  }

  DirGraph<index_t> g;

  vector<int> zero_indegrees(N, 1);
  vector<int> zero_outdegrees(N, 1);

  auto &w = g.add_edge_property<int>(GraphPropertyTag::EdgeWeight);

  for (index_t i = 0; i < N; i++) {
    for (index_t j = i + 1; j < N; j++) {
      auto result = boxes[i] <=> boxes[j];
      if (result == 1) {
        g.add_edge(i, j);
        w[{i, j}] = -1;
        zero_indegrees[j] = 0;
        zero_outdegrees[i] = 0;
      } else if (result == -1) {
        g.add_edge(j, i);
        w[{j, i}] = -1;
        zero_indegrees[i] = 0;
        zero_outdegrees[j] = 0;
      }
    }
  }
  for (index_t i = 0; i < N; i++) {
    // supersource
    if (zero_indegrees[i]) {
      g.add_edge(N, i);
      w[{N, i}] = -1;
    }
    // supersink
    if (zero_outdegrees[i]) {
      g.add_edge(i, N + 1);
      w[{i, N + 1}] = -1;
    }
  }
  dag_shortest_paths(g, w, N);
  auto &pred =
      g.get_vertex_property<optional<index_t>>(GraphPropertyTag::VertexParent);

  list<index_t> path;
  construct_path(pred, N, N + 1, path);
  vector<Box<F, d>> result;
  for (const auto &p : path) {
    if (p != N && p != N + 1) {
      result.push_back(boxes[p]);
    }
  }
  return result;
}

} // namespace frozenca

#endif //__CLRS4_NESTING_BOXES_H__