#ifndef __CLRS4_OFFLINE_MINIMUM_H__
#define __CLRS4_OFFLINE_MINIMUM_H__

#include <algorithm>
#include <cassert>
#include <common.h>
#include <graph_algorithms.h>
#include <limits>

namespace frozenca {

using namespace std;

vector<int> offline_minimum(const vector<int> &v, int m, int n) {
  UndirGraph<int> g;
  auto &color = g.add_vertex_property<int>(GraphPropertyTag::VertexColor);

  int index = 0;
  int prev = -1;
  auto INF = numeric_limits<int>::max();
  vector<int> color_repr(m + 1, INF);
  for (auto num : v) {
    if (num != -1) {
      g.add_vertex(num);
      color[num] = index;
      color_repr[index] = num;
      if (prev != -1) {
        g.add_edge(prev, num);
      }
    } else {
      index++;
    }
    prev = num;
  }
  union_find_by_size(g);

  auto &parent = g.get_vertex_property<int>(GraphPropertyTag::VertexParent);
  auto &link = g.get_vertex_property<int>(GraphPropertyTag::VertexLink);

  vector<int> extracted(m);

  for (int i = 1; i <= n; ++i) {
    auto j = color[find_set(parent, i)];
    if (j != m) {
      extracted[j] = i;
      int k = j + 1;
      while (k <= m && color_repr[k] == -1) {
        k++;
      }
      if (k <= m) {
        if (color_repr[k] != INF) {
          auto vr = find_set(parent, color_repr[k]);
          auto ur = find_set(parent, color_repr[j]);
          link_by_rank(parent, color, link, vr, ur);
          color_repr[j] = -1;
        } else {
          auto ur = find_set(parent, color_repr[j]);
          color[ur] = k;
          color_repr[k] = ur;
          color_repr[j] = -1;
        }
      }
    }
  }
  return extracted;
}

} // namespace frozenca

#endif //__CLRS4_OFFLINE_MINIMUM_H__
