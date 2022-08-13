#include <20_elementary_graph_algorithms/inc/easy.h>
#include <cassert>
#include <graph_algorithms.h>
#include <iostream>
#include <vector>

int main() {
  namespace fc = frozenca;
  using namespace std;
  fc::MultiGraph<int> g;
  g.add_edge(0, 1);
  g.add_edge(0, 1);
  g.add_edge(0, 2);
  g.add_edge(0, 3);
  g.add_edge(0, 4);
  g.add_edge(1, 1);
  g.add_edge(1, 3);
  g.add_edge(1, 2);
  g.add_edge(2, 3);
  g.add_edge(2, 2);
  g.add_edge(4, 2);

  auto g_rf = fc::refine(g);
  for (const auto &src : g_rf.vertices()) {
    for (const auto &dst : g_rf.adj(src)) {
      cout << src << ' ' << dst << '\n';
    }
  }
}
