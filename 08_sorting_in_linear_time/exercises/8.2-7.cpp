#include <08_sorting_in_linear_time/inc/easy.h>
#include <algorithm>
#include <iostream>
#include <random>

int main() {
  namespace fc = frozenca;
  using namespace std;

  {
    vector<float> A{3.16f, 2.87f, 1.92f, 2.68f, 0.53f, 4.76f};
    fc::counting_sort_fraction(A, 5, 2);
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
  uniform_real_distribution<> dist(0, max_k);

  {
    // std::sort
    vector<float> durations;
    for (int i = 0; i < num_trials; ++i) {
      vector<float> v;
      for (int j = 0; j < max_k; ++j) {
        v.push_back(static_cast<int>(dist(gen) * 100) / 100.0f);
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
      vector<float> v;
      for (int j = 0; j < max_k; ++j) {
        v.push_back(static_cast<int>(dist(gen) * 100) / 100.0f);
      }
      auto start = chrono::steady_clock::now();
      fc::counting_sort_fraction(v, max_k, 2);
      auto end = chrono::steady_clock::now();
      durations.push_back(
          chrono::duration_cast<chrono::duration<float, micro>>(end - start)
              .count());
    }
    auto stat = fc::get_statistics(durations);
    fc::log("Counting sort fraction Time to process a range of {:6} elements:\n"
            "Average : {:10.4f} us,\n"
            "Stdev   : {:10.4f} us,\n"
            "95%     : {:10.4f} us,\n"
            "99%     : {:10.4f} us,\n"
            "99.9%   : {:10.4f} us,\n",
            fc::log_level::all, max_k, stat.average, stat.stdev,
            stat.percentile_95, stat.percentile_99, stat.percentile_999);
  }
}