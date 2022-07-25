#include <chrono>
#include <iostream>
#include <random>
#include <rbtree.h>
#include <string>
#include <test.h>
#include <util.h>

namespace fc = frozenca;
using namespace std;

int main() {
  fc::TreeSet<int> tree1;
  constexpr int mid_value = 1'000'000;
  for (int i = 0; i < mid_value; ++i) {
    tree1.insert(i);
  }

  fc::TreeSet<int> tree2;
  for (int i = mid_value + 1; i < 2 * mid_value; ++i) {
    tree2.insert(i);
  }

  auto start = chrono::steady_clock::now();
  auto tree = fc::detail::join(move(tree1), mid_value, move(tree2));
  auto end = chrono::steady_clock::now();
  float elapsed =
      chrono::duration_cast<chrono::duration<float, micro>>(end - start)
          .count();
  cout << "elapsed " << elapsed << "us\n";

  for (int i = 0; i < 2 * mid_value; ++i) {
    if (!tree.contains(i)) {
      cout << "Verification failed!\n";
      break;
    }
  }

  fc::TreeSet<string> tree_str1;
  tree_str1.insert("abc");
  tree_str1.insert("aaa");
  tree_str1.insert("abcde");

  fc::TreeSet<string> tree_str2;
  tree_str2.insert("xyz");
  tree_str2.insert("fcfc");

  auto tree_str =
      fc::detail::join(move(tree_str1), "ccccc", move(tree_str2));

  cout << tree_str << '\n';
}
