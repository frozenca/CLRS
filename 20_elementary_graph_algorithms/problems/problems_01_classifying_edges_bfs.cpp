#include <20_elementary_graph_algorithms/inc/easy.h>
#include <cassert>
#include <iostream>

int main() {
  namespace fc = frozenca;
  using namespace std;
  {
    fc::UndirGraph<char> g;
    g.add_edge('w', 'x');
    g.add_edge('w', 'z');
    g.add_edge('w', 'v');
    g.add_edge('w', 'r');
    g.add_edge('x', 'z');
    g.add_edge('r', 's');
    g.add_edge('r', 't');
    g.add_edge('v', 's');
    g.add_edge('v', 'y');
    g.add_edge('x', 'y');
    g.add_edge('s', 'u');
    g.add_edge('t', 'u');
    g.add_edge('y', 'u');

    fc::bfs(g, 's', true);
  }
  cout << '\n';
  {
    fc::DirGraph<char> g;
    g.add_edge('a', 'b');
    g.add_edge('b', 'e');
    g.add_edge('e', 'a');
    g.add_edge('b', 'f');
    g.add_edge('e', 'f');
    g.add_edge('f', 'g');
    g.add_edge('g', 'f');
    g.add_edge('c', 'g');
    g.add_edge('c', 'd');
    g.add_edge('d', 'c');
    g.add_edge('g', 'h');
    g.add_edge('d', 'h');
    g.add_edge('h', 'h');
    g.add_edge('c', 'i');

    fc::bfs(g, 'a', true);
  }
}
