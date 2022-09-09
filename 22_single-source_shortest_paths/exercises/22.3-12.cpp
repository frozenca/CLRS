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
  auto &weight = g.add_edge_property<float>(fc::GraphPropertyTag::EdgeWeight);
  weight[{'s', 't'}] = 7.5f;
  weight[{'s', 'y'}] = 5.0f;
  weight[{'t', 'x'}] = 3.0f;
  weight[{'t', 'y'}] = 3.5f;
  weight[{'y', 't'}] = 4.0f;
  weight[{'y', 'x'}] = 7.0f;
  weight[{'x', 'z'}] = 4.5f;
  weight[{'z', 'x'}] = 5.5f;
  weight[{'z', 's'}] = 6.0f;
  weight[{'y', 'z'}] = 3.5f;

  fc::dijkstra_bounded_ratio(g, weight, 's');
  auto &d = g.get_vertex_property<float>(fc::GraphPropertyTag::VertexDistance);
  cout << "Dijkstra result:\n";
  for (const auto &u : g.vertices()) {
    cout << u << ": " << d[u] << '\n';
  }
}
