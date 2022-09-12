#include <cassert>
#include <graph_algorithms.h>
#include <iostream>

int main() {
  namespace fc = frozenca;
  using namespace std;
  using G = fc::DirGraph<int>;
  G g;
  for (int i = 1; i <= 5; ++i) {
    g.add_vertex(i);
  }
  fc::transitive_closure(g);
  fc::transitive_closure_add_edge(g, {1, 2});
  fc::transitive_closure_add_edge(g, {1, 3});
  fc::transitive_closure_add_edge(g, {1, 5});
  fc::transitive_closure_add_edge(g, {2, 4});
  fc::transitive_closure_add_edge(g, {2, 5});
  fc::transitive_closure_add_edge(g, {3, 2});
  fc::transitive_closure_add_edge(g, {4, 1});
  fc::transitive_closure_add_edge(g, {4, 3});
  fc::transitive_closure_add_edge(g, {5, 4});

  auto &trans_closure =
      g.get_edge_property<bool>(fc::GraphPropertyTag::EdgeTransitiveClosure);
  cout << "Transitive closure:\n";
  for (const auto &[e, w] : trans_closure.get()) {
    cout << e.first << '-' << e.second << ": " << w << '\n';
  }
}
