#include <20_elementary_graph_algorithms/inc/easy.h>
#include <cassert>
#include <iostream>

int main() {
  namespace fc = frozenca;
  using namespace std;
  fc::UndirGraph<int> g;
  g.add_edge(0, 1);
  g.add_edge(0, 2);
  g.add_edge(0, 3);
  g.add_edge(1, 2);
  g.add_edge(1, 3);
  g.add_edge(2, 3);
  g.add_edge(3, 4);
  g.add_edge(4, 5);
  g.add_edge(4, 6);
  g.add_edge(4, 7);
  g.add_edge(4, 8);
  g.add_edge(4, 10);
  g.add_edge(5, 6);
  g.add_edge(7, 8);
  g.add_edge(7, 9);
  g.add_edge(10, 11);
  g.add_edge(10, 12);
  g.add_edge(11, 13);
  g.add_edge(12, 13);
  g.add_edge(13, 14);
  g.add_edge(14, 16);
  g.add_edge(14, 17);
  g.add_edge(15, 16);
  g.add_edge(15, 17);
  g.add_edge(15, 18);
  g.add_edge(16, 17);
  g.add_edge(16, 19);
  g.add_edge(16, 20);
  g.add_edge(17, 18);
  g.add_edge(19, 21);
  g.add_edge(19, 22);
  g.add_edge(21, 22);
  fc::biconnected_components(g);
  const auto &articulation_points = g.get_graph_property<unordered_set<int>>(
      fc::GraphPropertyTag::GraphBCCArticulationPoints);

  cout << "Articulation points:\n";
  for (const auto &p : articulation_points) {
    cout << p << ' ';
  }
  cout << '\n';

  const auto &bridges = g.get_graph_property<
      unordered_set<fc::Edge<int>, fc::Hash<fc::Edge<int>>>>(
      fc::GraphPropertyTag::GraphBCCBridges);

  cout << "Bridges:\n";
  for (const auto &[u, v] : bridges) {
    cout << u << '-' << v << '\n';
  }

  const auto &bcc =
      g.get_edge_property<fc::index_t>(fc::GraphPropertyTag::EdgeBcc);

  cout << "Biconnected components:\n";
  for (const auto &u : g.vertices()) {
    for (const auto &v : g.adj(u)) {
      if (bcc.get().contains({u, v})) {
        cout << u << "-" << v << " : " << bcc[{u, v}] << '\n';
      }
    }
  }
}
