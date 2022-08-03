#include <14_dynamic_programming/inc/easy.h>
#include <graph.h>
#include <iostream>
#include <vector>

int main() {
  namespace fc = frozenca;
  using namespace std;

  fc::DirGraph<int> g;

  g.add_edge(0, 1);
  g.add_edge(0, 2);
  g.add_edge(1, 2);
  g.add_edge(1, 3);
  g.add_edge(2, 3);
  g.add_edge(2, 4);
  g.add_edge(2, 5);
  g.add_edge(3, 4);
  g.add_edge(3, 5);
  g.add_edge(4, 5);

  auto &weight =
      g.add_edge_property<float>(fc::GraphPropertyTag::EdgeWeight);

  weight[{0, 1}] = 5.0f;
  weight[{0, 2}] = 3.0f;
  weight[{1, 2}] = 2.0f;
  weight[{1, 3}] = 6.0f;
  weight[{2, 3}] = 7.0f;
  weight[{2, 4}] = 4.0f;
  weight[{2, 5}] = 2.0f;
  weight[{3, 4}] = -1.0f;
  weight[{3, 5}] = 1.0f;
  weight[{4, 5}] = -2.0f;

  cout << fc::longest_simple_path_dag(g, 1, 5) << '\n';
}