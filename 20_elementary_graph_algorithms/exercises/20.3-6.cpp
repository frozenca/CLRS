#include <20_elementary_graph_algorithms/inc/easy.h>
#include <cassert>
#include <iostream>

int main() {
  namespace fc = frozenca;
  using namespace std;
  fc::UndirGraph<char> g;
  g.add_edge('w', 'x');
  g.add_edge('w', 'z');
  g.add_edge('w', 'v');
  g.add_edge('w', 'r');
  g.add_edge('x', 'z');
  g.add_edge('r', 's');
  g.add_edge('r', 't');
  g.add_edge('v', 's');
  g.add_edge('v', 'y');
  g.add_edge('x', 'y');
  g.add_edge('s', 'u');
  g.add_edge('t', 'u');
  g.add_edge('y', 'u');

  fc::dfs(g);

  const auto &d =
      g.get_vertex_property<fc::index_t>(fc::GraphPropertyTag::VertexTime);
  const auto &f = g.get_vertex_property<fc::index_t>(
      fc::GraphPropertyTag::VertexTimeFinish);

  for (const auto &v : g.vertices()) {
    cout << v << " : " << d[v] << ' ' << f[v] << '\n';
  }

  cout << '\n';

  fc::dfs_iterative(g);

  for (const auto &v : g.vertices()) {
    cout << v << " : " << d[v] << ' ' << f[v] << '\n';
  }
}
