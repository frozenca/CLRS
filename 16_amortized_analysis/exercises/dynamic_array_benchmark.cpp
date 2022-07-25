#include <algorithm>
#include <chrono>
#include <dynamicarray.h>
#include <functional>
#include <iostream>
#include <random>
#include <ranges>
#include <test.h>
#include <util.h>
#include <vector>

namespace fc = frozenca;
using namespace std;

template <typename DArrayType> void dynamic_array_test(bool warmup = false) {
  constexpr int max_n = 100'000;
  constexpr int max_trials = 1'000;

  mt19937 gen(random_device{}());
  vector<float> durations;
  bernoulli_distribution dist(0.8);

  for (int t = 0; t < max_trials; ++t) {
    DArrayType h;

    float duration = 0.0f;

    for (int i = 0; i < max_n; ++i) {
      if (dist(gen)) {
        auto start = chrono::steady_clock::now();
        h.push_back(1);
        auto end = chrono::steady_clock::now();
        duration +=
            chrono::duration_cast<chrono::duration<float, micro>>(end - start)
                .count();
      } else if (!h.empty()) {
        auto start = chrono::steady_clock::now();
        h.pop_back();
        auto end = chrono::steady_clock::now();
        duration +=
            chrono::duration_cast<chrono::duration<float, micro>>(end - start)
                .count();
      }
    }

    durations.push_back(duration);
  }
  if (!warmup) {
    {
      auto stat = fc::get_statistics(durations);
      fc::log("Time to process {:6} elements:\n"
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
  cout << "Dynamic array demo\n";

  dynamic_array_test<vector<int>>(true); // warm up for benchmarking

  cout << "Warming up complete...\n";
  cout << "frozenca::DynamicArray<int> test\n";
  dynamic_array_test<fc::DynamicArray<int>>();
  cout << "std::vector<int> test\n";
  dynamic_array_test<vector<int>>();
}
