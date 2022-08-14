#include <20_elementary_graph_algorithms/inc/easy.h>
#include <cassert>
#include <iomanip>
#include <iostream>
#include <list>

int main() {
  namespace fc = frozenca;
  using namespace std;
  fc::DirGraph<char> g;
  g.add_edge('m', 'q');
  g.add_edge('m', 'x');
  g.add_edge('m', 'r');
  g.add_edge('n', 'q');
  g.add_edge('n', 'u');
  g.add_edge('n', 'o');
  g.add_edge('q', 't');
  g.add_edge('u', 't');
  g.add_edge('r', 'u');
  g.add_edge('r', 'y');
  g.add_edge('o', 'r');
  g.add_edge('o', 's');
  g.add_edge('p', 'o');
  g.add_edge('s', 'r');
  g.add_edge('v', 'x');
  g.add_edge('y', 'v');
  g.add_edge('o', 'v');
  g.add_edge('p', 's');
  g.add_edge('v', 'w');
  g.add_edge('w', 'z');
  g.add_edge('p', 'z');

  cout << boolalpha << fc::topological_sort_kahn(g) << '\n';

  const auto &top_sort =
      g.add_graph_property<list<char>>(fc::GraphPropertyTag::GraphTopSort);
  for (const auto &v : top_sort) {
    cout << v << ' ';
  }
  cout << '\n';

  g.add_edge('t', 'm');

  cout << boolalpha << fc::topological_sort_kahn(g) << '\n';
}
