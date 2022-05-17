#include <09_medians_and_order_statistics/inc/easy.h>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

int main() {
  namespace fc = frozenca;
  using namespace std;

  mt19937 gen(random_device{}());

  constexpr int num_trials = 10'000;
  constexpr int max_n = 10'000;
  uniform_int_distribution<> dist(1, max_n);

  int fail_count = 0;
  for (int i = 0; i < num_trials; ++i) {
    int n = dist(gen);
    vector<int> v(n);
    iota(v.begin(), v.end(), 0);
    ranges::shuffle(v, gen);

    uniform_int_distribution<> k_dist(0, n - 1);
    int k = k_dist(gen);
    auto kth = fc::select7(v, k);
    if (kth != k) {
      cout << "Select algorithm failed!\n";
      fail_count++;
    }
  }
  if (fail_count == 0) {
    cout << "Select algorithm succeeded\n";
  }

  // get median test
  {
    // std::nth_element
    vector<float> durations;
    for (int i = 0; i < num_trials; ++i) {
      vector<int> v;
      for (int j = 0; j < max_n; ++j) {
        v.push_back(dist(gen));
      }
      auto start = chrono::steady_clock::now();
      ranges::nth_element(v, ranges::next(v.begin(), max_n / 2));
      auto end = chrono::steady_clock::now();
      durations.push_back(
          chrono::duration_cast<chrono::duration<float, micro>>(end - start)
              .count());
    }
    auto stat = fc::get_statistics(durations);
    fc::log("std::nth_element Time to process a range of {:6} elements:\n"
            "Average : {:10.4f} us,\n"
            "Stdev   : {:10.4f} us,\n"
            "95%     : {:10.4f} us,\n"
            "99%     : {:10.4f} us,\n"
            "99.9%   : {:10.4f} us,\n",
            fc::log_level::all, max_n, stat.average, stat.stdev,
            stat.percentile_95, stat.percentile_99, stat.percentile_999);
  }

  {
    // fc::select
    vector<float> durations;
    for (int i = 0; i < num_trials; ++i) {
      vector<int> v;
      for (int j = 0; j < max_n; ++j) {
        v.push_back(dist(gen));
      }
      auto start = chrono::steady_clock::now();
      fc::select7(v, max_n / 2);
      auto end = chrono::steady_clock::now();
      durations.push_back(
          chrono::duration_cast<chrono::duration<float, micro>>(end - start)
              .count());
    }
    auto stat = fc::get_statistics(durations);
    fc::log("worst-case linear time select (using group of 7) Time to process "
            "a range of {:6} "
            "elements:\n"
            "Average : {:10.4f} us,\n"
            "Stdev   : {:10.4f} us,\n"
            "95%     : {:10.4f} us,\n"
            "99%     : {:10.4f} us,\n"
            "99.9%   : {:10.4f} us,\n",
            fc::log_level::all, max_n, stat.average, stat.stdev,
            stat.percentile_95, stat.percentile_99, stat.percentile_999);
  }
}