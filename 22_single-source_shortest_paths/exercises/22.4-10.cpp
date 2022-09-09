#include <cassert>
#include <graph_algorithms.h>
#include <iostream>

int main() {
  namespace fc = frozenca;
  using namespace std;
  using G = fc::DirGraph<int>;
  // single-variable constraint
  G g;
  g.add_edge(1, 0);
  g.add_edge(2, 0);
  g.add_edge(3, 0);
  g.add_edge(0, 1);
  g.add_edge(0, 2);
  g.add_edge(0, 3);
  auto &weight = g.add_edge_property<int>(fc::GraphPropertyTag::EdgeWeight);
  // -x_1 <= 1, x_1 >= -1
  weight[{1, 0}] = 1;
  weight[{2, 0}] = 2;
  weight[{3, 0}] = 3;
  // x_1 <= 2
  weight[{0, 1}] = 2;
  weight[{0, 2}] = 3;
  weight[{0, 3}] = 4;

  auto res = fc::bellman_ford(g, weight, 0);
  if (!res) {
    cout << "Negative cycle!\n";
  } else {
    auto &d = g.get_vertex_property<int>(fc::GraphPropertyTag::VertexDistance);
    cout << "Bellman-Ford result:\n";
    for (const auto &u : g.vertices()) {
      cout << u << ": " << d[u] << '\n';
    }
  }
}
