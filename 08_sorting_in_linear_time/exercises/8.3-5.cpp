#include <08_sorting_in_linear_time/inc/easy.h>
#include <algorithm>
#include <iostream>
#include <random>

int main() {
  namespace fc = frozenca;
  using namespace std;

  {
    vector<int> A{27, 11, 3, 16};
    fc::radix_sort_cubic(A);
    fc::print(A);
    if (ranges::is_sorted(A)) {
      cout << "Sorted\n";
    } else {
      cout << "Failed to sort\n";
    }
  }

  
  constexpr int n = 200;
  constexpr int max_k = n * n * n;
  constexpr int num_trials = 10'000;

  mt19937 gen(random_device{}());
  uniform_int_distribution<> dist(0, max_k);

  {
    // std::sort
    vector<float> durations;
    for (int i = 0; i < num_trials; ++i) {
      vector<int> v;
      for (int j = 0; j < n; ++j) {
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
            fc::log_level::all, n, stat.average, stat.stdev,
            stat.percentile_95, stat.percentile_99, stat.percentile_999);
  }

  {
    // radix sort cubic
    vector<float> durations;
    for (int i = 0; i < num_trials; ++i) {
      vector<int> v;
      for (int j = 0; j < n; ++j) {
        v.push_back(dist(gen));
      }
      auto start = chrono::steady_clock::now();
      fc::radix_sort_cubic(v);
      auto end = chrono::steady_clock::now();
      durations.push_back(
          chrono::duration_cast<chrono::duration<float, micro>>(end - start)
              .count());
    }
    auto stat = fc::get_statistics(durations);
    fc::log("Radix sort cubic Time to process a range of {:6} elements:\n"
            "Average : {:10.4f} us,\n"
            "Stdev   : {:10.4f} us,\n"
            "95%     : {:10.4f} us,\n"
            "99%     : {:10.4f} us,\n"
            "99.9%   : {:10.4f} us,\n",
            fc::log_level::all, n, stat.average, stat.stdev,
            stat.percentile_95, stat.percentile_99, stat.percentile_999);
  }

}