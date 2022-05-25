#include <12_binary_search_trees/inc/easy.h>
#include <algorithm>
#include <chrono>
#include <iostream>
#include <memory>
#include <numeric>
#include <random>
#include <vector>

int main() {
  namespace fc = frozenca;
  using namespace std;

  constexpr int max_n = 10'000;

  {
    fc::BSTEqualAlternate<int> tree;
    cout << "Insert " << max_n << " identical numbers\n";
    auto start = chrono::steady_clock::now();
    for (int i = 0; i < max_n; ++i) {
      tree.tree_insert(1);
    }
    auto end = chrono::steady_clock::now();
    float duration = chrono::duration_cast<chrono::duration<float, micro>>(end - start).count();
    cout << duration << "us\n";
  }
  {
    fc::BSTEqualList<int> tree;
    cout << "Insert " << max_n << " identical numbers\n";
    auto start = chrono::steady_clock::now();
    for (int i = 0; i < max_n; ++i) {
      tree.tree_insert(1);
    }
    auto end = chrono::steady_clock::now();
    float duration = chrono::duration_cast<chrono::duration<float, micro>>(end - start).count();
    cout << duration << "us\n";
  }
  {
    fc::BSTEqualRandom<int> tree;
    cout << "Insert " << max_n << " identical numbers\n";
    auto start = chrono::steady_clock::now();
    for (int i = 0; i < max_n; ++i) {
      tree.tree_insert(1);
    }
    auto end = chrono::steady_clock::now();
    float duration = chrono::duration_cast<chrono::duration<float, micro>>(end - start).count();
    cout << duration << "us\n";
  }
}