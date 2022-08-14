#include <20_elementary_graph_algorithms/inc/easy.h>
#include <cassert>
#include <iostream>

int main() {
  namespace fc = frozenca;
  using namespace std;
  fc::DirGraph<char> g;
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

  fc::dfs(g, true);
}
