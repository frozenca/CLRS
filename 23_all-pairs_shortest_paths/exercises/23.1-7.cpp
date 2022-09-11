#include <cassert>
#include <graph_algorithms.h>
#include <iostream>


int main() {
  namespace fc = frozenca;
  using namespace std;
  using G = fc::DirGraph<int>;
  G g;
  g.add_edge(1, 2);
  g.add_edge(1, 3);
  g.add_edge(1, 5);
  g.add_edge(2, 4);
  g.add_edge(2, 5);
  g.add_edge(3, 2);
  g.add_edge(4, 1);
  g.add_edge(4, 3);
  g.add_edge(5, 4);
  auto &weight = g.add_edge_property<int>(fc::GraphPropertyTag::EdgeWeight);
  weight[{1, 2}] = 3;
  weight[{1, 3}] = 8;
  weight[{1, 5}] = -4;
  weight[{2, 4}] = 1;
  weight[{2, 5}] = 7;
  weight[{3, 2}] = 4;
  weight[{4, 1}] = 2;
  weight[{4, 3}] = -5;
  weight[{5, 4}] = 6;
  fc::apsp_pred(g, weight);
  auto &shortest_paths =
      g.get_edge_property<int>(fc::GraphPropertyTag::EdgeAPSP);
  cout << "All-pair shortest paths:\n";
  for (const auto &[e, w] : shortest_paths.get()) {
    cout << e.first << '-' << e.second << ": " << w << '\n';
  }
  cout << "Direct predecessor:\n";
  auto& pred = g.get_edge_property<optional<int>>(fc::GraphPropertyTag::EdgeParent);
  for (const auto &[e, w] : shortest_paths.get()) {
    if (pred[e].has_value()) {
      cout << e.first << '-' << e.second << ": " << *pred[e] << '\n';
    }
  }
}
