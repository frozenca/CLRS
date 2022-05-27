#include <algorithm>
#include <chrono>
#include <functional>
#include <hashtable.h>
#include <iostream>
#include <list>
#include <numeric>
#include <random>
#include <ranges>
#include <test.h>
#include <unordered_set>
#include <util.h>
#include <vector>


namespace fc = frozenca;
using namespace std;

template <typename HashTableType> void hash_table_test(bool warmup = false) {
  constexpr int max_n = 100'000;
  constexpr int max_trials = 500;

  mt19937 gen(random_device{}());
  vector<float> durations_insert;
  vector<float> durations_find;
  vector<float> durations_erase;
  vector<int> v(max_n);
  iota(v.begin(), v.end(), 0);

  for (int t = 0; t < max_trials; ++t) {
    HashTableType h;

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

    duration = 0.0f;
    ranges::shuffle(v, gen);
    for (auto num : v) {
      auto start = chrono::steady_clock::now();
      if (!h.contains(num)) {
        cout << "Verification fail!\n";
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
        cout << "Verification fail!\n";
      }
      auto end = chrono::steady_clock::now();
      duration +=
          chrono::duration_cast<chrono::duration<float, micro>>(end - start)
              .count();
    }
    durations_erase.push_back(duration);
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
  }
}

int main() {
  cout << "Hash table demo\n";
  
  hash_table_test<unordered_set<int>>(true); // warm up for benchmarking

  cout << "Warming up complete...\n";
  cout << "frozenca::hard::HashSet<int, std::hash<int>> test\n";
  hash_table_test<fc::hard::HashSet<int, hash<int>>>();
  cout << "frozenca::hard::HashSet<int, frozenca::hard::Hash<int>> test\n";
  hash_table_test<fc::hard::HashSet<int>>();
  cout << "std::unordered_set<int, std::hash<int>> test\n";
  hash_table_test<unordered_set<int>>();
  cout << "std::unordered_set<int, frozenca::hard::Hash<int>> test\n";
  hash_table_test<unordered_set<int, fc::hard::Hash<int>>>();
}