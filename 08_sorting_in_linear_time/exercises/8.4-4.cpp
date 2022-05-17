#include <08_sorting_in_linear_time/inc/easy.h>
#include <algorithm>
#include <iostream>
#include <random>

int main() {
  namespace fc = frozenca;
  using namespace std;

  {
    vector<float> A{0.18f, 0.227f, 0.93f, 0.34f, 0.52f, 0.46f, 0.112f};
    fc::bucket_sort_two_rv(A);
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
  uniform_real_distribution<float> dist(0, 1);

  {
    // std::sort
    vector<float> durations;
    for (int i = 0; i < num_trials; ++i) {
      vector<float> v;
      for (int j = 0; j < max_k; ++j) {
        v.push_back(floor(10 * dist(gen)) / 10.0f + dist(gen) / max_k);
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
    // bucket sort two r.v
    vector<float> durations;
    for (int i = 0; i < num_trials; ++i) {
      vector<float> v;
      for (int j = 0; j < max_k; ++j) {
        v.push_back(floor(10 * dist(gen)) / 10.0f + dist(gen) / max_k);
      }
      auto start = chrono::steady_clock::now();
      fc::bucket_sort_two_rv(v);
      auto end = chrono::steady_clock::now();
      durations.push_back(
          chrono::duration_cast<chrono::duration<float, micro>>(end - start)
              .count());
    }
    auto stat = fc::get_statistics(durations);
    fc::log("Bucket sort two r.v. Time to process a range of {:6} elements:\n"
            "Average : {:10.4f} us,\n"
            "Stdev   : {:10.4f} us,\n"
            "95%     : {:10.4f} us,\n"
            "99%     : {:10.4f} us,\n"
            "99.9%   : {:10.4f} us,\n",
            fc::log_level::all, max_k, stat.average, stat.stdev,
            stat.percentile_95, stat.percentile_99, stat.percentile_999);
  }

}