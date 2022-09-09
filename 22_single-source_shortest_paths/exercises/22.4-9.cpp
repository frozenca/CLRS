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
  auto &weight = g.add_edge_property<int>(fc::GraphPropertyTag::EdgeWeight);
  weight[{2, 1}] = 1;
  weight[{4, 1}] = -4;
  weight[{3, 2}] = 2;
  weight[{5, 2}] = 7;
  weight[{6, 2}] = 5;
  weight[{6, 3}] = 10;
  weight[{2, 4}] = 2;
  weight[{1, 5}] = -1;
  weight[{4, 5}] = 3;
  weight[{3, 6}] = -8;

  auto res = fc::bellman_ford_constraint(g, weight, 1);
  if (!res) {
    cout << "Negative cycle!\n";
  } else {
    auto &d = g.get_vertex_property<int>(fc::GraphPropertyTag::VertexDistance);
    cout << "Bellman-Ford result:\n";
    int max_val = numeric_limits<int>::lowest();
    int min_val = numeric_limits<int>::max();
    for (const auto &u : g.vertices()) {
      cout << u << ": " << d[u] << '\n';
      max_val = max(max_val, d[u]);
      min_val = min(min_val, d[u]);
    }
    cout << "Max difference: " << max_val - min_val << '\n';
  }
}
