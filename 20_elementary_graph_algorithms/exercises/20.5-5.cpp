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

  auto g_comp = fc::get_component_graph(g);
  for (const auto &u : g_comp.vertices()) {
    for (const auto &v : g_comp.adj(u)) {
      cout << u << " -> " << v << '\n';
    }
  }
}
