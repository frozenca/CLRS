#ifndef __CLRS4_DEPTH_DETERMINATION_H__
#define __CLRS4_DEPTH_DETERMINATION_H__

#include <common.h>
#include <graph_algorithms.h>

namespace frozenca {

using namespace std;

template <Descriptor V>
void make_tree(VertexProperty<V, V> &parent, VertexProperty<V, int> &depth,
               const V &vertex) {
  parent[vertex] = vertex;
  depth[vertex] = 0;
}

template <Descriptor V>
V find_root(VertexProperty<V, V> &parent, VertexProperty<V, int> &depth,
            const V &v) {
  if (parent[v] != parent[parent[v]]) {
    auto y = parent[v];
    parent[v] = find_root(parent, depth, y);
    depth[v] = depth[y] + 1;
  }
  return parent[v];
}

template <Descriptor V>
int find_depth(VertexProperty<V, V> &parent, VertexProperty<V, int> &depth,
               const V &v) {
  find_root(parent, depth, v);
  return depth[v];
}

template <Descriptor V>
void graft(VertexProperty<V, V> &parent, VertexProperty<V, int> &depth,
           const V &r, const V &v) {
  auto vd = find_depth(parent, depth, v);
  parent[r] = parent[v];
  depth[r] = vd + 1;
}

} // namespace frozenca

#endif //__CLRS4_DEPTH_DETERMINATION_H__
