#include <20_elementary_graph_algorithms/inc/easy.h>
#include <cassert>
#include <iomanip>
#include <iostream>

int main() {
  namespace fc = frozenca;
  using namespace std;
  fc::UndirGraph<int> g;
  g.add_edge(0, 1);
  g.add_edge(1, 2);
  g.add_edge(1, 3);
  g.add_edge(2, 4);
  g.add_edge(3, 5);
  g.add_edge(0, 6);
  g.add_edge(0, 7);
  g.add_edge(7, 8);

  cout << fc::tree_diameter(g);
}
