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
  g.add_edge('t', 'y');
  g.add_edge('y', 't');
  g.add_edge('y', 'x');
  g.add_edge('x', 'z');
  g.add_edge('z', 'x');
  g.add_edge('z', 's');
  g.add_edge('y', 'z');
  auto &weight = g.add_edge_property<int>(fc::GraphPropertyTag::EdgeWeight);
  weight[{'s', 't'}] = 10;
  weight[{'s', 'y'}] = 5;
  weight[{'t', 'x'}] = 1;
  weight[{'t', 'y'}] = 2;
  weight[{'y', 't'}] = 3;
  weight[{'y', 'x'}] = 9;
  weight[{'x', 'z'}] = 4;
  weight[{'z', 'x'}] = 6;
  weight[{'z', 's'}] = 7;
  weight[{'y', 'z'}] = 2;

  fc::dijkstra(g, weight, 's');
  auto &d = g.get_vertex_property<int>(fc::GraphPropertyTag::VertexDistance);
  cout << "Dijkstra result:\n";
  for (const auto &u : g.vertices()) {
    cout << u << ": " << d[u] << '\n';
  }
}
