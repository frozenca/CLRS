#include <20_elementary_graph_algorithms/inc/easy.h>
#include <cassert>
#include <iomanip>
#include <iostream>
#include <list>

int main() {
  namespace fc = frozenca;
  using namespace std;
  fc::AdjListUndirGraph<int> g;
  fc::planar_initialize(g);
  // Goldner-Hanary
  fc::planar_insert(g, 0, {});
  fc::planar_insert(g, 1, {0});
  fc::planar_insert(g, 2, {0, 1});
  fc::planar_insert(g, 3, {0, 2});
  fc::planar_insert(g, 4, {0, 3});
  fc::planar_insert(g, 5, {0, 4});
  fc::planar_insert(g, 6, {0, 5});
  fc::planar_insert(g, 7, {0, 5, 6});
  fc::planar_insert(g, 8, {2, 4});
  fc::planar_insert(g, 9, {4, 6});
  fc::planar_insert(g, 10, {0, 1, 2, 4, 6, 7, 8, 9});
  const auto &orientation = g.get_edge_property<fc::EdgeOrientation>(
      fc::GraphPropertyTag::EdgeOrientation);
  cout << "Edge orientations for the Goldner-Hanary graph:\n";
  for (const auto &u : g.vertices()) {
    for (const auto &v : g.adj(u)) {
      if (u < v) {
        if (orientation[{u, v}] == fc::EdgeOrientation::Less) {
          cout << u << "->" << v << '\n';
        } else {
          cout << v << "->" << u << '\n';
        }
      }
    }
  }
  cout << "Orientation outdegrees:\n";
  const auto &outdegree =
      g.get_vertex_property<fc::index_t>(fc::GraphPropertyTag::VertexOutdegree);
  for (const auto &u : g.vertices()) {
    cout << u << ": " << outdegree[u] << '\n';
  }
  cout << "Most recently inserted neighbor:\n";
  for (const auto &u : g.vertices()) {
    cout << u << ": " << fc::planar_newest_neighbor(g, u) << '\n';
  }
  cout << "Now try to insert another edge...\n";
  try {
    fc::planar_insert_edge(g, 1, 3);
  } catch (const exception &e) {
    cout << e.what() << '\n';
  }
}
