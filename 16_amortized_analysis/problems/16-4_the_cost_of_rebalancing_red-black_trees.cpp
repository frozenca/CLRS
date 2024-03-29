#include <algorithm>
#include <chrono>
#include <functional>
#include <iostream>
#include <list>
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

template <typename RBTreeType, bool inspect_potential = false>
void rb_tree_test(bool warmup = false) {
  constexpr int max_n = 1'000;
  constexpr int max_trials = 500;

  mt19937 gen(random_device{}());
  vector<float> durations_insert;
  vector<float> durations_find;
  vector<float> durations_erase;
  vector<int> v(max_n);
  iota(v.begin(), v.end(), 0);
  [[maybe_unused]] vector<float> potentials_insert;
  [[maybe_unused]] vector<float> potentials_erase;

  for (int t = 0; t < max_trials; ++t) {
    RBTreeType h;

    float duration = 0.0f;
    ranges::shuffle(v, gen);
    for (auto num : v) {
      auto start = chrono::steady_clock::now();
      h.insert(num);
      auto end = chrono::steady_clock::now();
      duration +=
          chrono::duration_cast<chrono::duration<float, micro>>(end - start)
              .count();
    }
    durations_insert.push_back(duration);

    if constexpr (inspect_potential) {
      potentials_insert.push_back(static_cast<float>(h.get_potential()));
    }

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
      duration +=
          chrono::duration_cast<chrono::duration<float, micro>>(end - start)
              .count();
    }
    durations_erase.push_back(duration);
    if constexpr (inspect_potential) {
      potentials_erase.push_back(static_cast<float>(h.get_potential()));
    }
  }
  if (!warmup) {
    {
      auto stat = fc::get_statistics(durations_insert);
      fc::log("Time to insert {:6} elements:\n"
              "Average : {:10.4f} us,\n"
              "Stdev   : {:10.4f} us,\n"
              "95%     : {:10.4f} us,\n"
              "99%     : {:10.4f} us,\n"
              "99.9%   : {:10.4f} us,\n",
              fc::log_level::all, max_n, stat.average, stat.stdev,
              stat.percentile_95, stat.percentile_99, stat.percentile_999);
    }
    {
      auto stat = fc::get_statistics(durations_find);
      fc::log("Time to find {:6} elements:\n"
              "Average : {:10.4f} us,\n"
              "Stdev   : {:10.4f} us,\n"
              "95%     : {:10.4f} us,\n"
              "99%     : {:10.4f} us,\n"
              "99.9%   : {:10.4f} us,\n",
              fc::log_level::all, max_n, stat.average, stat.stdev,
              stat.percentile_95, stat.percentile_99, stat.percentile_999);
    }
    {
      auto stat = fc::get_statistics(durations_erase);
      fc::log("Time to erase {:6} elements:\n"
              "Average : {:10.4f} us,\n"
              "Stdev   : {:10.4f} us,\n"
              "95%     : {:10.4f} us,\n"
              "99%     : {:10.4f} us,\n"
              "99.9%   : {:10.4f} us,\n",
              fc::log_level::all, max_n, stat.average, stat.stdev,
              stat.percentile_95, stat.percentile_99, stat.percentile_999);
    }
    if constexpr (inspect_potential) {
      {
        auto stat = fc::get_statistics(potentials_insert);
        fc::log("Potential after inserting {:6} elements:\n"
                "Average : {:10.4f},\n"
                "Stdev   : {:10.4f},\n"
                "95%     : {:10.4f},\n"
                "99%     : {:10.4f},\n"
                "99.9%   : {:10.4f},\n",
                fc::log_level::all, max_n, stat.average, stat.stdev,
                stat.percentile_95, stat.percentile_99, stat.percentile_999);
      }
      {
        auto stat = fc::get_statistics(potentials_erase);
        fc::log("Potential after inserting and erasing {:6} elements:\n"
                "Average : {:10.4f},\n"
                "Stdev   : {:10.4f},\n"
                "95%     : {:10.4f},\n"
                "99%     : {:10.4f},\n"
                "99.9%   : {:10.4f},\n",
                fc::log_level::all, max_n, stat.average, stat.stdev,
                stat.percentile_95, stat.percentile_99, stat.percentile_999);
      }
    }
  }
}

int main() {
  cout << "Red-black tree demo\n";

  rb_tree_test<set<int>>(true); // warm up for benchmarking

  cout << "Warming up complete...\n";
  cout << "frozenca::TreeSet<int> test\n";
  rb_tree_test<fc::TreeSet<int>, true>();
  cout << "std::set<int> test\n";
  rb_tree_test<set<int>>();
}
