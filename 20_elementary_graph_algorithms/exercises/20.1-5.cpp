#include <20_elementary_graph_algorithms/inc/easy.h>
#include <cassert>
#include <iostream>
#include <vector>

int main() {
  namespace fc = frozenca;
  using namespace std;
  {
    fc::DirGraph<int> g;
    g.add_edge(0, 1);
    g.add_edge(0, 2);
    g.add_edge(0, 3);
    g.add_edge(0, 4);
    g.add_edge(1, 2);
    g.add_edge(1, 3);
    g.add_edge(2, 3);
    g.add_edge(4, 2);

    auto g_sq = fc::square(g);
    for (const auto &src : g_sq.vertices()) {
      for (const auto &dst : g_sq.adj(src)) {
        cout << src << ' ' << dst << '\n';
      }
    }
  }
  cout << '\n';
  {
    fc::AdjMatDirGraph<int> g;
    g.add_edge(0, 1);
    g.add_edge(0, 2);
    g.add_edge(0, 3);
    g.add_edge(0, 4);
    g.add_edge(1, 2);
    g.add_edge(1, 3);
    g.add_edge(2, 3);
    g.add_edge(4, 2);

    auto g_sq = fc::square(g);
    for (const auto &src : g_sq.vertices()) {
      for (const auto &dst : g_sq.adj(src)) {
        cout << src << ' ' << dst << '\n';
      }
    }
  }
}
