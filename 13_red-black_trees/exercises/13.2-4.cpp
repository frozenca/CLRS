#include <13_red-black_trees/inc/easy.h>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

int main() {
  namespace fc = frozenca;
  using namespace std;

  vector<int> v(20);
  iota(v.begin(), v.end(), 0);
  mt19937 gen(random_device{}());

  ranges::shuffle(v, gen);
  fc::RBTreeConvert<int> tree1;
  for (auto num : v) {
    tree1.insert(num);
  }

  cout << "Tree1\n";
  tree1.print();
  cout << '\n';

  ranges::shuffle(v, gen);
  fc::RBTreeConvert<int> tree2;
  for (auto num : v) {
    tree2.insert(num);
  }

  cout << "Tree2\n";
  tree2.print();
  cout << '\n';

  cout << "Rotation sequence\n";
  auto seq = tree1.get_rotation_sequence(tree2);
  for (const auto &rot : seq) {
    cout << rot << ' ';
  }
  cout << '\n';
}