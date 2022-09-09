#include <cassert>
#include <graph_algorithms.h>
#include <iostream>

int main() {
  namespace fc = frozenca;
  using namespace std;
  using G = fc::DirGraph<char>;
  G g;
  g.add_edge('s', 't');
  g.add_edge('s', 'y');
  g.add_edge('t', 'x');
  g.add_edge('x', 't');
  g.add_edge('t', 'y');
  g.add_edge('t', 'z');
  g.add_edge('y', 'x');
  g.add_edge('y', 'z');
  g.add_edge('z', 'x');
  g.add_edge('z', 's');
  auto &weight = g.add_edge_property<int>(fc::GraphPropertyTag::EdgeWeight);
  weight[{'s', 't'}] = 6;
  weight[{'s', 'y'}] = 7;
  weight[{'t', 'x'}] = 5;
  weight[{'x', 't'}] = -2;
  weight[{'t', 'y'}] = 8;
  weight[{'t', 'z'}] = -4;
  weight[{'y', 'x'}] = -3;
  weight[{'y', 'z'}] = 9;
  weight[{'z', 'x'}] = 7;
  weight[{'z', 's'}] = 2;

  auto res = fc::bellman_ford_yen(g, weight, 's');
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
