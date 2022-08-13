#ifndef __CLRS4_REPRESENTATION_GRAPHS_H__
#define __CLRS4_REPRESENTATION_GRAPHS_H__

#include <common.h>
#include <graph.h>

namespace frozenca {

using namespace std;

template <GraphConcept GraphType> auto outdegree(GraphType &g) {
  using V = GraphType::vertex_type;
  auto &outdegrees =
      g.add_vertex_property<index_t>(GraphPropertyTag::VertexOutdegree);
  for (const auto &vertex : g.vertices()) {
    outdegrees[vertex] = ranges::ssize(g.adj(vertex));
  }
  return outdegrees.get();
}

} // namespace frozenca

#endif //__CLRS4_REPRESENTATION_GRAPHS_H__