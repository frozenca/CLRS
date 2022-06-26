#include <13_red-black_trees/inc/easy.h>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

int main() {
  namespace fc = frozenca;
  using namespace std;

  fc::RBTreeConvert<int> tree1;
  tree1.insert(4);
  tree1.insert(3);
  tree1.insert(2);
  tree1.insert(1);

  cout << "Tree1\n";
  tree1.print();
  cout << '\n';

  fc::RBTreeConvert<int> tree2;
  tree2.insert(1);
  tree2.insert(2);
  tree2.insert(3);
  tree2.insert(4);

  cout << "Tree2\n";
  tree2.print();
  cout << '\n';

  cout << "Right rotation sequence\n";
  auto seq = tree1.get_right_rotation_sequence(tree2);
  for (const auto &rot : seq) {
    cout << rot << ' ';
  }
  cout << '\n';
}
