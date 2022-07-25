#include <algorithm>
#include <chrono>
#include <functional>
#include <hashtable.h>
#include <iostream>
#include <numeric>
#include <random>
#include <ranges>
#include <test.h>
#include <unordered_set>
#include <util.h>
#include <vector>
#include <string>


namespace fc = frozenca;
using namespace std;

vector<string> generate_random_strings(int max_n, int max_length, bool allow_duplicates) {
  vector<string> res;

  mt19937 gen(random_device{}());
  uniform_int_distribution<int> length_dist (1, max_length);
  uniform_int_distribution<int> ch_dist (32, 126);

  for (int i = 0; i < max_n; ++i) {
    int len = length_dist(gen);
    string s;
    for (int l = 0; l < len; ++l) {
      s += static_cast<char>(ch_dist(gen));
    }
    res.push_back(move(s));
  }

  if (!allow_duplicates) {
    ranges::sort(res);
    auto ret = ranges::unique(res);
    res.erase(ret.begin(), ret.end());
  }

  return res;  
}


template <typename HashTableType> void hash_table_string_test(bool warmup = false) {
  constexpr int max_n = 10'000;
  constexpr int max_length = 30;
  constexpr int max_trials = 200;

  mt19937 gen(random_device{}());

  vector<float> durations_insert;
  vector<float> durations_find;
  vector<float> durations_erase;

  for (int t = 0; t < max_trials; ++t) {
    auto v = generate_random_strings(max_n, max_length, false);

    HashTableType h;

    float duration = 0.0f;
    ranges::shuffle(v, gen);
    for (const auto& str : v) {
      auto start = chrono::steady_clock::now();
      h.insert(str);
      auto end = chrono::steady_clock::now();
      duration +=
          chrono::duration_cast<chrono::duration<float, micro>>(end - start)
              .count();
    }
    durations_insert.push_back(duration);

    duration = 0.0f;
    ranges::shuffle(v, gen);
    for (const auto& str : v) {
      auto start = chrono::steady_clock::now();
      if (!h.contains(str)) {
        cout << "Contains verification fail!\n";
      }
      auto end = chrono::steady_clock::now();
      duration +=
          chrono::duration_cast<chrono::duration<float, micro>>(end - start)
              .count();
    }
    durations_find.push_back(duration);

    duration = 0.0f;
    ranges::shuffle(v, gen);
    for (const auto& str : v) {
      auto start = chrono::steady_clock::now();
      if (!h.erase(str)) {
        cout << "Erase verification fail!\n";
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
  cout << "Hash table demo with std::string\n";
  
  hash_table_string_test<unordered_set<string>>(true); // warm up for benchmarking

  cout << "Warming up complete...\n";
  cout << "frozenca::HashSet<std::string, std::hash<std::string>> test\n";
  hash_table_string_test<fc::HashSet<string, hash<string>>>();
  cout << "frozenca::HashSet<std::string, frozenca::Hash<std::string>> test\n";
  hash_table_string_test<fc::HashSet<string>>();
  cout << "std::unordered_set<std::string, std::hash<std::string>> test\n";
  hash_table_string_test<unordered_set<string>>();
  cout << "std::unordered_set<std::string, frozenca::Hash<std::string>> test\n";
  hash_table_string_test<unordered_set<string, fc::Hash<string>>>();
}