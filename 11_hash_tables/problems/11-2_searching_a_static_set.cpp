#include <11_hash_tables/inc/hard.h>
#include <algorithm>
#include <iostream>
#include <limits>
#include <random>
#include <ranges>
#include <tuple>
#include <unordered_set>
#include <vector>


int main() {

  namespace fc = frozenca;
  using namespace std;
  constexpr int num_trials = 1'000'000;

  mt19937 gen(random_device{}());
  uniform_int_distribution<size_t> dist(0, numeric_limits<size_t>::max());

  // std::unordered_set
  {
    unordered_set<size_t> s;

    {
      vector<float> durations;
      for (int i = 0; i < num_trials; ++i) {
        auto num = dist(gen);
        auto start = chrono::steady_clock::now();
        s.insert(num);
        auto end = chrono::steady_clock::now();
        durations.push_back(
            chrono::duration_cast<chrono::duration<float, micro>>(end - start)
                .count());
      }
      auto stat = fc::get_statistics(durations);
      fc::log("unordered_set time to insert {:6} elements:\n"
              "Average : {:10.4f} us,\n"
              "Stdev   : {:10.4f} us,\n"
              "95%     : {:10.4f} us,\n"
              "99%     : {:10.4f} us,\n"
              "99.9%   : {:10.4f} us,\n",
              fc::log_level::all, num_trials, stat.average, stat.stdev,
              stat.percentile_95, stat.percentile_99, stat.percentile_999);
    }
    {
      vector<float> durations;
      for (int i = 0; i < num_trials; ++i) {
        auto num = dist(gen);
        auto start = chrono::steady_clock::now();
        ignore = s.contains(num);
        auto end = chrono::steady_clock::now();
        durations.push_back(
            chrono::duration_cast<chrono::duration<float, micro>>(end - start)
                .count());
      }
      auto stat = fc::get_statistics(durations);
      fc::log("unordered_set time to retrieve {:6} elements:\n"
              "Average : {:10.4f} us,\n"
              "Stdev   : {:10.4f} us,\n"
              "95%     : {:10.4f} us,\n"
              "99%     : {:10.4f} us,\n"
              "99.9%   : {:10.4f} us,\n",
              fc::log_level::all, num_trials, stat.average, stat.stdev,
              stat.percentile_95, stat.percentile_99, stat.percentile_999);
    }
  }

  // std::vector, sorted
  {
    vector<size_t> s;

    {
      vector<float> durations;
      for (int i = 0; i < num_trials; ++i) {
        auto num = dist(gen);
        auto start = chrono::steady_clock::now();
        s.push_back(num);
        auto end = chrono::steady_clock::now();
        durations.push_back(
            chrono::duration_cast<chrono::duration<float, micro>>(end - start)
                .count());
      }
      auto stat = fc::get_statistics(durations);
      fc::log("array time to insert {:6} elements:\n"
              "Average : {:10.4f} us,\n"
              "Stdev   : {:10.4f} us,\n"
              "95%     : {:10.4f} us,\n"
              "99%     : {:10.4f} us,\n"
              "99.9%   : {:10.4f} us,\n",
              fc::log_level::all, num_trials, stat.average, stat.stdev,
              stat.percentile_95, stat.percentile_99, stat.percentile_999);
    }
    ranges::sort(s);
    {
      vector<float> durations;
      for (int i = 0; i < num_trials; ++i) {
        auto num = dist(gen);
        auto start = chrono::steady_clock::now();
        ignore = ranges::binary_search(s, num);
        auto end = chrono::steady_clock::now();
        durations.push_back(
            chrono::duration_cast<chrono::duration<float, micro>>(end - start)
                .count());
      }
      auto stat = fc::get_statistics(durations);
      fc::log("sorted array time to retrieve {:6} elements:\n"
              "Average : {:10.4f} us,\n"
              "Stdev   : {:10.4f} us,\n"
              "95%     : {:10.4f} us,\n"
              "99%     : {:10.4f} us,\n"
              "99.9%   : {:10.4f} us,\n",
              fc::log_level::all, num_trials, stat.average, stat.stdev,
              stat.percentile_95, stat.percentile_99, stat.percentile_999);
    }
  }
}