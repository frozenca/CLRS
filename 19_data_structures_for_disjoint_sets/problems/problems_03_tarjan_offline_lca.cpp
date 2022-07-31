#include <19_data_structures_for_disjoint_sets/inc/easy.h>
#include <cassert>
#include <iostream>
#include <vector>

int main() {
  namespace fc = frozenca;
  using namespace std;

  fc::DiGraph<int> g;
  g.add_edge(0, 1);
  g.add_edge(0, 2);
  g.add_edge(0, 3);
  g.add_edge(0, 4);
  g.add_edge(2, 5);
  g.add_edge(2, 6);
  g.add_edge(3, 7);
  g.add_edge(4, 8);
  g.add_edge(4, 9);
  g.add_edge(6, 10);
  g.add_edge(7, 11);
  g.add_edge(8, 12);
  g.add_edge(10, 13);

  fc::offline_lca(g, 0, 13);
}
