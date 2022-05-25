#include <algorithm>
#include <binarysearchtree.h>
#include <common.h>
#include <iostream>
#include <memory>
#include <numeric>
#include <random>
#include <test.h>
#include <util.h>
#include <vector>

int main() {
  namespace fc = frozenca;
  using namespace std;

  mt19937 gen(random_device{}());

  {
    constexpr int max_n = 1'000;
    vector<int> v(max_n);
    iota(v.begin(), v.end(), 0);

    cout << "Binary search tree demo\n";
    cout << "Insert 1 to " << max_n << " in random order\n";

    vector<float> heights;
    for (int i = 0; i < max_n; ++i) {
      fc::BinarySearchTree<int> tree;
      ranges::shuffle(v, gen);
      for (auto num : v) {
        tree.tree_insert(num);
      }
      heights.push_back(static_cast<float>(tree.tree_height()));
    }
    auto stat = fc::get_statistics(heights);
    fc::log(
        "Height of randomly built binary search tree with inserting distinct "
        "{:6} elements in random order:\n"
        "Average : {:10.4f},\n"
        "Stdev   : {:10.4f},\n"
        "95%     : {:10.4f},\n"
        "99%     : {:10.4f},\n"
        "99.9%   : {:10.4f},\n",
        fc::log_level::all, max_n, stat.average, stat.stdev, stat.percentile_95,
        stat.percentile_99, stat.percentile_999);
  }
  {
    constexpr int max_n = 1'000;
    vector<int> v(max_n);
    iota(v.begin(), v.end(), 0);
    ranges::shuffle(v, gen);
    fc::BinarySearchTree<int> tree;
    for (auto num : v) {
      tree.tree_insert(num);
    }
    // just inserting to binary search tree in the same order within the array
    // and doing inorder traversal of the tree corresponds to quicksort.
    cout << tree << '\n';
  }
}