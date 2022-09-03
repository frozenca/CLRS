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

  auto &weight = g.add_edge_property<int>(fc::GraphPropertyTag::EdgeWeight);
  weight[{'a', 'b'}] = 4;
  weight[{'a', 'h'}] = 8;
  weight[{'b', 'h'}] = 11;
  weight[{'b', 'c'}] = 8;
  weight[{'c', 'd'}] = 7;
  weight[{'c', 'i'}] = 2;
  weight[{'c', 'f'}] = 4;
  weight[{'i', 'h'}] = 7;
  weight[{'i', 'g'}] = 6;
  weight[{'h', 'g'}] = 1;
  weight[{'f', 'g'}] = 2;
  weight[{'d', 'f'}] = 14;
  weight[{'d', 'e'}] = 9;
  weight[{'f', 'e'}] = 10;

  fc::mst_kruskal(g, weight);
  auto &mst =
      g.add_graph_property<fc::EdgeSet<G>>(fc::GraphPropertyTag::GraphMST);
  cout << "Minimum spanning tree:\n";
  for (const auto &[u, v] : mst) {
    cout << u << '-' << v << ": " << weight[{u, v}] << '\n';
  }

  fc::mst_update_edge(g, weight, {'d', 'f'}, 6);
  cout << "Minimum spanning tree after relaxation:\n";
  for (const auto &[u, v] : mst) {
    cout << u << '-' << v << ": " << weight[{u, v}] << '\n';
  }
}
