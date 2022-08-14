#ifndef __CLRS4_SCC_H__
#define __CLRS4_SCC_H__

#include "01_representations_of_graphs.h"
#include "03_depth_first_search.h"
#include <common.h>
#include <graph.h>

namespace frozenca {

using namespace std;

template <DirGraphConcept G> void strongly_connected_components(G &g) {
  auto &scc = g.add_vertex_property<index_t>(GraphPropertyTag::VertexScc);
  dfs(g);
  auto g_t = transpose(g);
  const auto &finish_order =
      g.get_graph_property<vector<V<G>>>(GraphPropertyTag::GraphVisitSort);
  dfs(g_t, false, finish_order);
  const auto &cc =
      g_t.get_vertex_property<index_t>(GraphPropertyTag::VertexComponent);
  scc = cc;
}

} // namespace frozenca

#endif //__CLRS4_SCC_H__