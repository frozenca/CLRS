#include <cassert>
#include <graph_algorithms.h>
#include <iostream>
#include <vector>


int main() {
  namespace fc = frozenca;
  using namespace std;

  fc::UnionFindGraph<int> g;

  g.add_edge(1, 3);
  g.add_edge(4, 5);
  g.add_edge(0, 2);
  g.add_edge(7, 8);
  g.add_edge(0, 1);
  g.add_edge(5, 6);
  g.add_edge(1, 2);
  g.add_vertex(9);

  fc::union_find_by_size(g);

  for (auto v : g.vertices()) {
    auto vec = fc::enumerate_set(g, v);
    for (auto num : vec) {
      cout << num << ' ';
    }
    cout << '\n';
  }
}
