#include <19_data_structures_for_disjoint_sets/inc/easy.h>
#include <cassert>
#include <iostream>
#include <vector>

int main() {
  namespace fc = frozenca;
  using namespace std;

  fc::DirGraph<int> g;
  auto &parent = g.add_vertex_property<int>(fc::GraphPropertyTag::VertexParent);
  auto &depth = g.add_vertex_property<int>(fc::GraphPropertyTag::VertexDepth);

  for (int i = 0; i < 14; ++i) {
    fc::make_tree(parent, depth, i);
  }
  fc::graft(parent, depth, 1, 0);
  fc::graft(parent, depth, 2, 0);
  fc::graft(parent, depth, 3, 0);
  fc::graft(parent, depth, 4, 0);
  fc::graft(parent, depth, 5, 2);
  fc::graft(parent, depth, 6, 2);
  fc::graft(parent, depth, 7, 3);
  fc::graft(parent, depth, 8, 4);
  fc::graft(parent, depth, 9, 4);
  fc::graft(parent, depth, 10, 6);
  fc::graft(parent, depth, 11, 7);
  fc::graft(parent, depth, 12, 8);
  fc::graft(parent, depth, 13, 10);

  for (int i = 0; i < 14; ++i) {
    cout << i << ' ' << fc::find_depth(parent, depth, i) << '\n';
  }
}
