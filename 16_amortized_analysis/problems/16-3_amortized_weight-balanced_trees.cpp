#include <algorithm>
#include <binarysearchtree.h>
#include <chrono>
#include <functional>
#include <iostream>
#include <numeric>
#include <random>
#include <ranges>
#include <rbtree.h>
#include <set>
#include <test.h>
#include <util.h>
#include <vector>

namespace fc = frozenca;
using namespace std;

template <typename RBTreeType, bool auto_balance = false>
void rb_tree_test(bool warmup = false) {
  constexpr int max_n = 500;
  constexpr int max_trials = 20;

  mt19937 gen(random_device{}());
  vector<float> durations_insert;
  vector<float> durations_find;
  vector<float> durations_erase;
  vector<int> v(max_n);
  iota(v.begin(), v.end(), 0);

  for (int t = 0; t < max_trials; ++t) {
    RBTreeType h;

    float duration = 0.0f;
    ranges::shuffle(v, gen);
    for (auto num : v) {
      auto start = chrono::steady_clock::now();
      h.insert(num);
      auto end = chrono::steady_clock::now();
      if constexpr (auto_balance) {
        h.tree_balance();
      }
      duration +=
          chrono::duration_cast<chrono::duration<float, micro>>(end - start)
              .count();
    }
    durations_insert.push_back(duration);

    duration = 0.0f;
    ranges::shuffle(v, gen);
    for (auto num : v) {
      auto start = chrono::steady_clock::now();
      if (!h.contains(num)) {
        cout << "Lookup verification fail!\n";
      }
      auto end = chrono::steady_clock::now();
      duration +=
          chrono::duration_cast<chrono::duration<float, micro>>(end - start)
              .count();
    }
    durations_find.push_back(duration);

    duration = 0.0f;
    ranges::shuffle(v, gen);
    for (auto num : v) {
      auto start = chrono::steady_clock::now();
      if (!h.erase(num)) {
        cout << "Erase verification fail!\n";
      }
      auto end = chrono::steady_clock::now();
      if constexpr (auto_balance) {
        h.tree_balance();
      }
      duration +=
          chrono::duration_cast<chrono::duration<float, micro>>(end - start)
              .count();
    }
    durations_erase.push_back(duration);
  }
  if (!warmup) {
    {
      auto stat = fc::get_statistics(durations_insert);
      fc::log("Time to insert {:6} elements: {}\n"
              "Average : {:10.4f} us,\n"
              "Stdev   : {:10.4f} us,\n"
              "95%     : {:10.4f} us,\n"
              "99%     : {:10.4f} us,\n"
              "99.9%   : {:10.4f} us,\n",
              fc::log_level::all, max_n,
              (auto_balance) ? "(without autobalance)" : "", stat.average,
              stat.stdev, stat.percentile_95, stat.percentile_99,
              stat.percentile_999);
    }
    {
      auto stat = fc::get_statistics(durations_find);
      fc::log("Time to find {:6} elements: {}\n"
              "Average : {:10.4f} us,\n"
              "Stdev   : {:10.4f} us,\n"
              "95%     : {:10.4f} us,\n"
              "99%     : {:10.4f} us,\n"
              "99.9%   : {:10.4f} us,\n",
              fc::log_level::all, max_n,
              (auto_balance) ? "(without autobalance)" : "", stat.average,
              stat.stdev, stat.percentile_95, stat.percentile_99,
              stat.percentile_999);
    }
    {
      auto stat = fc::get_statistics(durations_erase);
      fc::log("Time to erase {:6} elements: {}\n"
              "Average : {:10.4f} us,\n"
              "Stdev   : {:10.4f} us,\n"
              "95%     : {:10.4f} us,\n"
              "99%     : {:10.4f} us,\n"
              "99.9%   : {:10.4f} us,\n",
              fc::log_level::all, max_n,
              (auto_balance) ? "(without autobalance)" : "", stat.average,
              stat.stdev, stat.percentile_95, stat.percentile_99,
              stat.percentile_999);
    }
  }
}

int main() {
  rb_tree_test<set<int>>(true); // warm up for benchmarking

  cout << "Warming up complete...\n";
  cout << "frozenca::BinarySearchTree<int> test\n";
  rb_tree_test<fc::BinarySearchTree<int>, true>();
  cout << "frozenca::TreeSet<int> test\n";
  rb_tree_test<fc::TreeSet<int>>();
  cout << "std::set<int> test\n";
  rb_tree_test<set<int>>();
}
