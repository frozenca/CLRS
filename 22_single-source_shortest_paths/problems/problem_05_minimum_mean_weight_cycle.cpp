#include <cassert>
#include <graph_algorithms.h>
#include <iostream>

int main() {
  namespace fc = frozenca;
  using namespace std;
  using G = fc::DirGraph<int>;
  G g;
  g.add_edge(0, 1);
  g.add_edge(1, 2);
  g.add_edge(2, 1);
  g.add_edge(2, 0);
  g.add_edge(4, 2);
  g.add_edge(1, 3);
  g.add_edge(3, 4);
  g.add_edge(4, 5);
  g.add_edge(3, 5);
  auto &weight = g.add_edge_property<int>(fc::GraphPropertyTag::EdgeWeight);
  weight[{0, 1}] = 1;
  weight[{1, 2}] = 2;
  weight[{2, 1}] = 2;
  weight[{2, 0}] = 3;
  weight[{4, 2}] = 1;
  weight[{1, 3}] = 2;
  weight[{3, 4}] = 1;
  weight[{4, 5}] = 1;
  weight[{3, 5}] = 1;
  auto res = fc::minimum_mean_weight_cycle(g, weight);
  if (!res.has_value()) {
    cout << "No cycle\n";
  } else {
    cout << "Minimum mean weight cycle: " << *res << '\n';
  }
}
