#include <20_elementary_graph_algorithms/inc/easy.h>
#include <cassert>
#include <iostream>

int main() {
  namespace fc = frozenca;
  using namespace std;
  fc::UndirGraph<int> g;
  g.add_edge(0, 1);
  g.add_edge(0, 3);
  g.add_edge(0, 4);
  g.add_edge(0, 5);
  g.add_edge(1, 2);
  g.add_edge(1, 3);
  g.add_edge(1, 4);
  g.add_edge(2, 3);
  g.add_edge(3, 4);
  g.add_edge(4, 5);
  auto has_euler_tour = fc::euler_tour(g);
  if (has_euler_tour) {
    const auto &tour =
        g.get_graph_property<vector<int>>(fc::GraphPropertyTag::GraphEulerTour);
    fc::print(tour);
  } else {
    cout << "This graph doesn't have an euler tour\n";
  }
}
