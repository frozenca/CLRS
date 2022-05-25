#include <binarysearchtree.h>
#include <algorithm>
#include <iostream>
#include <vector>
#include <random>
#include <numeric>
#include <memory>

int main() {
  namespace fc = frozenca;
  using namespace std;

  fc::BinarySearchTree<int> tree;
  
  constexpr int max_n = 10'000;

  mt19937 gen(random_device{}());

  vector<int> v (max_n);
  iota(v.begin(), v.end(), 0);

  ranges::shuffle(v, gen);
  for (auto num : v) {
    tree.tree_insert_recursive(num);
  }
  ranges::shuffle(v, gen);
  for (auto num : v) {
    auto node = tree.tree_search(num);
    if (!node) {
      cout << "Fail\n";
    }
  }
  cout << "Finished\n";
}