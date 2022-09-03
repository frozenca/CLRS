#include <cassert>
#include <graph_algorithms.h>
#include <iomanip>
#include <iostream>

int main() {
  namespace fc = frozenca;
  using namespace std;
  using G = fc::UndirGraph<char>;
  G g;
  g.add_edge('a', 'b');
  g.add_edge('a', 'h');
  g.add_edge('b', 'h');
  g.add_edge('b', 'c');
  g.add_edge('c', 'd');
  g.add_edge('c', 'i');
  g.add_edge('c', 'f');
  g.add_edge('i', 'h');
  g.add_edge('i', 'g');
  g.add_edge('h', 'g');
  g.add_edge('f', 'g');
  g.add_edge('d', 'f');
  g.add_edge('d', 'e');
  g.add_edge('f', 'e');

  auto &weight = g.add_edge_property<float>(fc::GraphPropertyTag::EdgeWeight);
  weight[{'a', 'b'}] = 0.2f;
  weight[{'a', 'h'}] = 0.4f;
  weight[{'b', 'h'}] = 0.55f;
  weight[{'b', 'c'}] = 0.4f;
  weight[{'c', 'd'}] = 0.35f;
  weight[{'c', 'i'}] = 0.1f;
  weight[{'c', 'f'}] = 0.2f;
  weight[{'i', 'h'}] = 0.35f;
  weight[{'i', 'g'}] = 0.3f;
  weight[{'h', 'g'}] = 0.05f;
  weight[{'f', 'g'}] = 0.1f;
  weight[{'d', 'f'}] = 0.07f;
  weight[{'d', 'e'}] = 0.45f;
  weight[{'f', 'e'}] = 0.5f;

  fc::mst_kruskal_bucket(g, weight);
  auto &mst =
      g.get_graph_property<fc::EdgeSet<G>>(fc::GraphPropertyTag::GraphMST);
  cout << "Minimum spanning tree (Kruskal):\n";
  for (const auto &[u, v] : mst) {
    cout << u << '-' << v << ": " << weight[{u, v}] << '\n';
  }
}
