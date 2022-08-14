#include <20_elementary_graph_algorithms/inc/easy.h>
#include <cassert>
#include <iomanip>
#include <iostream>

int main() {
  namespace fc = frozenca;
  using namespace std;
  fc::UndirGraph<char> g;
  g.add_edge('m', 'q');
  g.add_edge('m', 'x');
  cout << boolalpha << fc::has_cycle(g) << '\n';
  g.add_edge('q', 'x');
  cout << boolalpha << fc::has_cycle(g) << '\n';
}
