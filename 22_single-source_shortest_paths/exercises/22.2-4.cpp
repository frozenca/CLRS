#include <cassert>
#include <graph_algorithms.h>
#include <iostream>

int main() {
  namespace fc = frozenca;
  using namespace std;
  using G = fc::DirGraph<char>;
  G g;
  g.add_edge('r', 's');
  g.add_edge('s', 't');
  g.add_edge('t', 'x');
  g.add_edge('x', 'y');
  g.add_edge('y', 'z');

  cout << "Number of all paths in the DAG:\n";
  cout << fc::count_all_paths(g) << '\n';

  g.add_edge('r', 't');
  g.add_edge('s', 'x');
  g.add_edge('t', 'y');
  g.add_edge('t', 'z');
  g.add_edge('x', 'z');

  cout << "Number of all paths in the DAG:\n";
  cout << fc::count_all_paths(g) << '\n';
}
