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
  weight[{'s', 't'}] = 1.0f;
  weight[{'s', 'y'}] = 0.5f;
  weight[{'t', 'x'}] = 0.1f;
  weight[{'t', 'y'}] = 0.2f;
  weight[{'y', 't'}] = 0.3f;
  weight[{'y', 'x'}] = 0.9f;
  weight[{'x', 'z'}] = 0.4f;
  weight[{'z', 'x'}] = 0.6f;
  weight[{'z', 's'}] = 0.7f;
  weight[{'y', 'z'}] = 0.2f;

  fc::reliability(g, weight, 's');
  auto &d = g.get_vertex_property<float>(fc::GraphPropertyTag::VertexDistance);
  auto &pred =
      g.get_vertex_property<optional<char>>(fc::GraphPropertyTag::VertexParent);
  cout << "Reliability result:\n";
  for (const auto &u : g.vertices()) {
    cout << u << ": Reliability " << d[u] << ", Parent ";
    if (pred[u].has_value()) {
      cout << *pred[u] << '\n';
    } else {
      cout << "None\n";
    }
  }
}
