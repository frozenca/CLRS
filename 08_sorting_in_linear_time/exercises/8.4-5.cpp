#include <08_sorting_in_linear_time/inc/easy.h>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <numbers>
#include <random>


int main() {
  namespace fc = frozenca;
  using namespace std;

  constexpr int max_k = 10'000;
  constexpr int num_trials = 10'000;

  mt19937 gen(random_device{}());
  uniform_real_distribution<float> radius(0, 1);
  uniform_real_distribution<float> angle(0, 2 * numbers::pi_v<float>);

  {
    // std::sort
    vector<float> durations;
    for (int i = 0; i < num_trials; ++i) {
      vector<pair<float, float>> v;
      for (int j = 0; j < max_k; ++j) {
        auto r = radius(gen);
        auto t = angle(gen);
        v.emplace_back(r * cos(t), r * sin(t));
      }
      auto start = chrono::steady_clock::now();
      ranges::sort(v, [](const auto &a, const auto &b) {
        return hypot(a.first, a.second) > hypot(b.first, b.second);
      });
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
    // bucket sort 2d
    vector<float> durations;
    for (int i = 0; i < num_trials; ++i) {
      vector<pair<float, float>> v;
      for (int j = 0; j < max_k; ++j) {
        auto r = radius(gen);
        auto t = angle(gen);
        v.emplace_back(r * cos(t), r * sin(t));
      }
      auto start = chrono::steady_clock::now();
      fc::bucket_sort_2d_points(v);
      auto end = chrono::steady_clock::now();
      durations.push_back(
          chrono::duration_cast<chrono::duration<float, micro>>(end - start)
              .count());
    }
    auto stat = fc::get_statistics(durations);
    fc::log("Bucket sort 2D points Time to process a range of {:6} elements:\n"
            "Average : {:10.4f} us,\n"
            "Stdev   : {:10.4f} us,\n"
            "95%     : {:10.4f} us,\n"
            "99%     : {:10.4f} us,\n"
            "99.9%   : {:10.4f} us,\n",
            fc::log_level::all, max_k, stat.average, stat.stdev,
            stat.percentile_95, stat.percentile_99, stat.percentile_999);
  }
}