#include <cassert>
#include <graph_algorithms.h>
#include <iostream>

int main() {
  namespace fc = frozenca;
  using namespace std;
  using G = fc::DirGraph<int>;
  G g;
  g.add_edge(2, 1);
  g.add_edge(4, 1);
  g.add_edge(3, 2);
  g.add_edge(5, 2);
  g.add_edge(6, 2);
  g.add_edge(6, 3);
  g.add_edge(2, 4);
  g.add_edge(1, 5);
  g.add_edge(4, 5);
  g.add_edge(3, 6);
  auto &weight = g.add_edge_property<float>(fc::GraphPropertyTag::EdgeWeight);
  weight[{2, 1}] = 1.6f;
  weight[{4, 1}] = -4.3f;
  weight[{3, 2}] = 2.5f;
  weight[{5, 2}] = 7.2f;
  weight[{6, 2}] = 5.8f;
  weight[{6, 3}] = 10.9f;
  weight[{2, 4}] = 2.3f;
  weight[{1, 5}] = -1.6f;
  weight[{4, 5}] = 3.5f;
  weight[{3, 6}] = -8.2f;

  unordered_set<int> to_round;
  to_round.insert(1);
  to_round.insert(2);
  to_round.insert(3);

  auto res = fc::bellman_ford_round_selective(g, weight, 1, to_round);
  if (!res) {
    cout << "Negative cycle!\n";
  } else {
    auto &d =
        g.get_vertex_property<float>(fc::GraphPropertyTag::VertexDistance);
    cout << "Bellman-Ford result:\n";
    for (const auto &u : g.vertices()) {
      cout << u << ": " << d[u] << '\n';
    }
  }
}
