#include <20_elementary_graph_algorithms/inc/easy.h>
#include <cassert>
#include <iostream>

int main() {
  namespace fc = frozenca;
  using namespace std;
  fc::AdjMatUndirGraph<char> g;
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

  fc::init_properties(g);
  fc::bfs(g, 's');

  fc::print_path(g, 's', 'x');
}
