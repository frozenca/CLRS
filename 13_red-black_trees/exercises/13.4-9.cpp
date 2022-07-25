#include <algorithm>
#include <iostream>
#include <numeric>
#include <random>
#include <rbtree.h>
#include <vector>

int main() {
  namespace fc = frozenca;
  using namespace std;

  fc::TreeSet<int> tree;
  tree.insert(1);
  tree.insert(2);
  tree.insert(3);
  tree.insert(5);
  tree.insert(8);
  tree.insert(13);

  {
    auto [l, h] = tree.enumerate(3, 8);
    for (auto it = l; it != h; ++it) {
      cout << *it << ' ';
    }
    cout << '\n';
  }
  {
    auto [l, h] = tree.enumerate(4, 9);
    for (auto it = l; it != h; ++it) {
      cout << *it << ' ';
    }
    cout << '\n';
  }
}
