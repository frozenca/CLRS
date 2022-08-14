#include <20_elementary_graph_algorithms/inc/easy.h>
#include <cassert>
#include <iomanip>
#include <iostream>
#include <list>

int main() {
  namespace fc = frozenca;
  using namespace std;
  fc::DirGraph<char> g;
  g.add_edge('a', 'b');
  g.add_edge('b', 'e');
  g.add_edge('e', 'a');
  g.add_edge('b', 'f');
  g.add_edge('e', 'f');
  g.add_edge('f', 'g');
  g.add_edge('g', 'f');
  g.add_edge('b', 'c');
  g.add_edge('c', 'g');
  g.add_edge('c', 'd');
  g.add_edge('d', 'c');
  g.add_edge('g', 'h');
  g.add_edge('d', 'h');
  g.add_edge('h', 'h');

  fc::strongly_connected_components(g);
  const auto &scc =
      g.get_vertex_property<fc::index_t>(fc::GraphPropertyTag::VertexScc);
  for (const auto &v : g.vertices()) {
    cout << v << " : " << scc[v] << '\n';
  }
}
