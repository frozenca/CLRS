#include <14_dynamic_programming/inc/easy.h>
#include <graph.h>
#include <iostream>
#include <vector>

int main() {
  namespace fc = frozenca;
  using namespace std;

  fc::hard::WeightedDiGraph<> g;

  g.add_edge(0, 1, 5.0);
  g.add_edge(0, 2, 3.0);
  g.add_edge(1, 2, 2.0);
  g.add_edge(1, 3, 6.0);
  g.add_edge(2, 3, 7.0);
  g.add_edge(2, 4, 4.0);
  g.add_edge(2, 5, 2.0);
  g.add_edge(3, 4, -1.0);
  g.add_edge(3, 5, 1.0);
  g.add_edge(4, 5, -2.0);

  cout << fc::longest_simple_path_dag(g, 1, 5) << '\n';
}