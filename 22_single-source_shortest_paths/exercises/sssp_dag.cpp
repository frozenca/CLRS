#include <cassert>
#include <graph_algorithms.h>
#include <iostream>

int main() {
  namespace fc = frozenca;
  using namespace std;
  using G = fc::DirGraph<char>;
  G g;
  g.add_edge('r', 's');
  g.add_edge('r', 't');
  g.add_edge('s', 't');
  g.add_edge('s', 'x');
  g.add_edge('t', 'x');
  g.add_edge('t', 'y');
  g.add_edge('t', 'z');
  g.add_edge('x', 'y');
  g.add_edge('x', 'z');
  g.add_edge('y', 'z');
  auto &weight = g.add_edge_property<int>(fc::GraphPropertyTag::EdgeWeight);
  weight[{'r', 's'}] = 5;
  weight[{'r', 't'}] = 3;
  weight[{'s', 't'}] = 2;
  weight[{'s', 'x'}] = 6;
  weight[{'t', 'x'}] = 7;
  weight[{'t', 'y'}] = 4;
  weight[{'t', 'z'}] = 2;
  weight[{'x', 'y'}] = -1;
  weight[{'x', 'z'}] = 1;
  weight[{'y', 'z'}] = -2;

  fc::dag_shortest_paths(g, weight, 's');
  auto &d = g.get_vertex_property<int>(fc::GraphPropertyTag::VertexDistance);
  cout << "Single-source shortest paths in DAG:\n";
  for (const auto &u : g.vertices()) {
    cout << u << ": " << d[u] << '\n';
  }
}
