#ifndef __CLRS4_DYNAMIC_TRANSITIVE_CLOSURE_H__
#define __CLRS4_DYNAMIC_TRANSITIVE_CLOSURE_H__

#include <common.h>
#include <graph.h>

namespace frozenca {

using namespace std;

template <DirGraphConcept G>
void transitive_closure_add_edge(G &g, const E<G> &e) {
  auto &T = g.get_edge_property<bool>(GraphPropertyTag::EdgeTransitiveClosure);
  auto [src, dst] = e;
  g.add_edge(src, dst);
  for (const auto &u : g.vertices()) {
    if (T[{u, src}] && !T[{u, dst}]) {
      for (const auto &v : g.vertices()) {
        if (T[{dst, v}]) {
          T[{u, v}] = true;
        }
      }
    }
  }
}

} // namespace frozenca

#endif //__CLRS4_DYNAMIC_TRANSITIVE_CLOSURE_H__