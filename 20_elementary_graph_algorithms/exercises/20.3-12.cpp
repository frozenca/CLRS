#include <20_elementary_graph_algorithms/inc/easy.h>
#include <cassert>
#include <iostream>

int main() {
  namespace fc = frozenca;
  using namespace std;
  fc::UndirGraph<char> g;
  g.add_edge('s', 'z');
  g.add_edge('s', 'w');
  g.add_edge('z', 'w');
  g.add_edge('z', 'y');
  g.add_edge('y', 'x');
  g.add_edge('x', 'z');
  g.add_edge('w', 'x');
  g.add_edge('v', 's');
  g.add_edge('v', 'w');
  g.add_edge('t', 'v');
  g.add_edge('t', 'u');
  g.add_edge('u', 'v');
  g.add_edge('u', 't');
  g.add_edge('a', 'b');
  g.add_edge('a', 'c');
  g.add_edge('d', 'e');
  g.add_edge('f', 'g');

  fc::dfs(g);
  const auto &cc =
      g.get_vertex_property<fc::index_t>(fc::GraphPropertyTag::VertexComponent);
  for (const auto &v : g.vertices()) {
    cout << v << " : " << cc[v] << '\n';
  }
}
