#include <08_sorting_in_linear_time/inc/easy.h>
#include <algorithm>
#include <iostream>
#include <random>

int main() {
  namespace fc = frozenca;
  using namespace std;

  {
    vector<int> A{2, 5, 3, 0, 2, 3, 0, 3};
    fc::counting_sort_inplace(A, 5);
    fc::print(A);
    if (ranges::is_sorted(A)) {
      cout << "Sorted\n";
    } else {
      cout << "Failed to sort\n";
    }
  }

  {
    vector<int> A{27, 17, 3, 16, 13, 10, 1, 5, 7, 12, 4, 8, 9, 0};
    fc::counting_sort_inplace(A, 30);
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
    // counting sort
    vector<float> durations;
    for (int i = 0; i < num_trials; ++i) {
      vector<int> v;
      for (int j = 0; j < max_k; ++j) {
        v.push_back(dist(gen));
      }
      auto start = chrono::steady_clock::now();
      fc::counting_sort(v, max_k);
      auto end = chrono::steady_clock::now();
      durations.push_back(
          chrono::duration_cast<chrono::duration<float, micro>>(end - start)
              .count());
    }
    auto stat = fc::get_statistics(durations);
    fc::log("Counting sort Time to process a range of {:6} elements:\n"
            "Average : {:10.4f} us,\n"
            "Stdev   : {:10.4f} us,\n"
            "95%     : {:10.4f} us,\n"
            "99%     : {:10.4f} us,\n"
            "99.9%   : {:10.4f} us,\n",
            fc::log_level::all, max_k, stat.average, stat.stdev,
            stat.percentile_95, stat.percentile_99, stat.percentile_999);
  }

  {
    // counting sort inplace
    vector<float> durations;
    for (int i = 0; i < num_trials; ++i) {
      vector<int> v;
      for (int j = 0; j < max_k; ++j) {
        v.push_back(dist(gen));
      }
      auto start = chrono::steady_clock::now();
      fc::counting_sort_inplace(v, max_k);
      auto end = chrono::steady_clock::now();
      durations.push_back(
          chrono::duration_cast<chrono::duration<float, micro>>(end - start)
              .count());
    }
    auto stat = fc::get_statistics(durations);
    fc::log("Counting sort inplace Time to process a range of {:6} elements:\n"
            "Average : {:10.4f} us,\n"
            "Stdev   : {:10.4f} us,\n"
            "95%     : {:10.4f} us,\n"
            "99%     : {:10.4f} us,\n"
            "99.9%   : {:10.4f} us,\n",
            fc::log_level::all, max_k, stat.average, stat.stdev,
            stat.percentile_95, stat.percentile_99, stat.percentile_999);
  }
}