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
  g.add_edge('c', 'g');
  g.add_edge('c', 'd');
  g.add_edge('d', 'c');
  g.add_edge('g', 'h');
  g.add_edge('d', 'h');
  g.add_edge('h', 'h');
  g.add_edge('c', 'i');

  auto &weight = g.add_vertex_property<int>(fc::GraphPropertyTag::VertexWeight);
  weight['a'] = 12;
  weight['b'] = 6;
  weight['e'] = 4;
  weight['c'] = 2;
  weight['d'] = 8;
  weight['f'] = 14;
  weight['g'] = 10;
  weight['h'] = 17;
  weight['i'] = 11;

  auto [max_diff, min_it, max_it] = fc::get_maxdiff(g, weight);
  cout << "Max diff " << max_diff << ": " << *min_it << " -> " << *max_it
       << '\n';
}
