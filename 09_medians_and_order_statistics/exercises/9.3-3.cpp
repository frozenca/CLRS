#include <09_medians_and_order_statistics/inc/easy.h>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

int main() {
  namespace fc = frozenca;
  using namespace std;

  {
    vector<int> A{27, 17, 3, 16, 13, 10, 1, 5, 7, 12, 4, 8, 9, 0};
    fc::quicksort_worst_nlogn(A);
    fc::print(A);
    if (ranges::is_sorted(A)) {
      cout << "Sorted\n";
    } else {
      cout << "Failed to sort\n";
    }
  }

  constexpr int max_k = 10'000;
  constexpr int num_trials = 10'000;

  mt19937 gen(random_device{}());
  uniform_int_distribution<> dist(0, max_k);

  {
    // std::sort
    vector<float> durations;
    for (int i = 0; i < num_trials; ++i) {
      vector<int> v;
      for (int j = 0; j < max_k; ++j) {
        v.push_back(dist(gen));
      }
      auto start = chrono::steady_clock::now();
      ranges::sort(v);
      auto end = chrono::steady_clock::now();
      durations.push_back(
          chrono::duration_cast<chrono::duration<float, micro>>(end - start)
              .count());
    }
    auto stat = fc::get_statistics(durations);
    fc::log("std::sort Time to process a range of {:6} elements:\n"
            "Average : {:10.4f} us,\n"
            "Stdev   : {:10.4f} us,\n"
            "95%     : {:10.4f} us,\n"
            "99%     : {:10.4f} us,\n"
            "99.9%   : {:10.4f} us,\n",
            fc::log_level::all, max_k, stat.average, stat.stdev,
            stat.percentile_95, stat.percentile_99, stat.percentile_999);
  }

  {
    // quicksort worst nlogn
    vector<float> durations;
    for (int i = 0; i < num_trials; ++i) {
      vector<int> v;
      for (int j = 0; j < max_k; ++j) {
        v.push_back(dist(gen));
      }
      auto start = chrono::steady_clock::now();
      fc::quicksort_worst_nlogn(v);
      auto end = chrono::steady_clock::now();
      durations.push_back(
          chrono::duration_cast<chrono::duration<float, micro>>(end - start)
              .count());
    }
    auto stat = fc::get_statistics(durations);
    fc::log(
        "Quicksort worst n log n Time to process a range of {:6} elements:\n"
        "Average : {:10.4f} us,\n"
        "Stdev   : {:10.4f} us,\n"
        "95%     : {:10.4f} us,\n"
        "99%     : {:10.4f} us,\n"
        "99.9%   : {:10.4f} us,\n",
        fc::log_level::all, max_k, stat.average, stat.stdev, stat.percentile_95,
        stat.percentile_99, stat.percentile_999);
  }
}