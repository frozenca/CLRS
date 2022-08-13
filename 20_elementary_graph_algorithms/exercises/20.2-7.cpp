#include <20_elementary_graph_algorithms/inc/easy.h>
#include <cassert>
#include <iomanip>
#include <iostream>


int main() {
  namespace fc = frozenca;
  using namespace std;
  using GraphType = fc::UndirGraph<int>;
  GraphType g;
  using Vs = GraphType::vertices_type;
  g.add_edge(0, 5);
  g.add_edge(1, 5);
  g.add_edge(1, 6);
  g.add_edge(2, 7);
  g.add_edge(2, 8);
  g.add_edge(3, 6);
  g.add_edge(4, 5);
  g.add_edge(4, 8);

  auto is_bipartite = fc::bipartite(g);
  cout << boolalpha << is_bipartite << '\n';
  if (is_bipartite) {
    auto &[part1, part2] = g.get_graph_property<pair<Vs, Vs>>(
        fc::GraphPropertyTag::GraphBipartite);
    for (const auto &vf : part1) {
      cout << vf << ' ';
    }
    cout << '\n';
    for (const auto &vf : part2) {
      cout << vf << ' ';
    }
    cout << '\n';
  }

  g.add_edge(0, 1);
  cout << boolalpha << fc::bipartite(g) << '\n';
}
