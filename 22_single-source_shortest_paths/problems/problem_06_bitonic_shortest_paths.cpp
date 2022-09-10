#include <22_single-source_shortest_paths/inc/easy.h>
#include <cassert>
#include <iostream>
#include <vector>

int main() {
  namespace fc = frozenca;
  using namespace std;
  using G = fc::DirGraph<int>;
  G g;
  g.add_edge(0, 1);
  g.add_edge(0, 3);
  g.add_edge(1, 2);
  g.add_edge(1, 3);
  g.add_edge(1, 4);
  g.add_edge(2, 1);
  g.add_edge(3, 2);
  g.add_edge(3, 4);
  g.add_edge(4, 0);
  g.add_edge(4, 2);
  auto &weight = g.add_edge_property<int>(fc::GraphPropertyTag::EdgeWeight);
  weight[{0, 1}] = 6;
  weight[{0, 3}] = 7;
  weight[{1, 2}] = 5;
  weight[{1, 3}] = 8;
  weight[{1, 4}] = -4;
  weight[{2, 1}] = -2;
  weight[{3, 2}] = -3;
  weight[{3, 4}] = 9;
  weight[{4, 0}] = 2;
  weight[{4, 2}] = 7;
  fc::bitonic_shortest_path(g, weight, 0);
  auto &pred =
      g.get_vertex_property<optional<int>>(fc::GraphPropertyTag::VertexParent);
  for (int i = 1; i < 5; ++i) {
    auto curr = i;
    vector<int> v;
    while (pred[curr].has_value()) {
      v.push_back(curr);
      curr = *pred[curr];
    }
    cout << "Bitonic shortest path from " << 0 << " to " << i << ":\n";
    for (auto e : v | views::reverse) {
      cout << e << ' ';
    }
    cout << '\n';
  }
}
