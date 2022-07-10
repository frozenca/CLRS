#include <17_augmenting_data_structures/inc/easy.h>
#include <algorithm>
#include <iostream>

int main() {
  namespace fc = frozenca;
  using namespace std;

  fc::OrderStatisticTree<int> tree;
  for (int i = 0; i < 10; ++i) {
    tree.insert(i);
  }
  cout << tree[3] << '\n';

  tree.erase(0);
  tree.erase(1);
  tree.erase(2);

  cout << tree[3] << '\n';

  cout << tree.index(5) << '\n';
}