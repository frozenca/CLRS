#include <14_dynamic_programming/inc/easy.h>
#include <graph.h>
#include <iostream>
#include <vector>

int main() {
  namespace fc = frozenca;
  using namespace std;

  fc::WeightedDiGraph<int, float> g;

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

  g(fc::e_w, {0, 1}) = 5.0f;
  g(fc::e_w, {0, 2}) = 3.0f;
  g(fc::e_w, {1, 2}) = 2.0f;
  g(fc::e_w, {1, 3}) = 6.0f;
  g(fc::e_w, {2, 3}) = 7.0f;
  g(fc::e_w, {2, 4}) = 4.0f;
  g(fc::e_w, {2, 5}) = 2.0f;
  g(fc::e_w, {3, 4}) = -1.0f;
  g(fc::e_w, {3, 5}) = 1.0f;
  g(fc::e_w, {4, 5}) = -2.0f;

  cout << fc::longest_simple_path_dag(g, 1, 5) << '\n';
}