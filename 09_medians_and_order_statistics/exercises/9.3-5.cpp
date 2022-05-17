#include <algorithm>
#include <common.h>
#include <iostream>
#include <random>
#include <test.h>
#include <util.h>
#include <utility>


using namespace std;

inline float median_five(float a, float b, float c, float d, float e) {
  if (a > b) {
    swap(a, b); // ensure a <= b
  }
  if (c > d) {
    swap(c, d); // ensure c <= d
  }
  if (c < a) {
    swap(b, d);
    c = a; // eliminate the lowest among a, b, c, d
  }

  // gets e to a
  a = e;

  if (a > b) {
    swap(a, b); // ensure a <= b
  }
  if (a < c) {
    swap(b, d);
    a = c; // eliminate another lowest among a, b, c, d
  }

  return min(a, d);
}

int main() {

  namespace fc = frozenca;

  constexpr int num_trials = 1'000;
  mt19937 gen(random_device{}());
  uniform_real_distribution<float> dist(0, 100.0f);

  {
    // std::nth_element
    vector<float> durations;
    for (int i = 0; i < num_trials; ++i) {
      vector<float> v;
      for (int j = 0; j < 5; ++j) {
        v.push_back(dist(gen));
      }
      auto start = chrono::steady_clock::now();
      ranges::nth_element(v, ranges::next(v.begin(), 2));
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
            fc::log_level::all, 5, stat.average, stat.stdev, stat.percentile_95,
            stat.percentile_99, stat.percentile_999);
  }

  {
    // median of five
    vector<float> durations;
    for (int i = 0; i < num_trials; ++i) {
      vector<float> v;
      for (int j = 0; j < 5; ++j) {
        v.push_back(dist(gen));
      }
      auto start = chrono::steady_clock::now();
      auto med = median_five(v[0], v[1], v[2], v[3], v[4]);
      auto end = chrono::steady_clock::now();
      // verification
      auto med_iter = ranges::next(v.begin(), 2);
      ranges::nth_element(v, med_iter);
      if (med != *med_iter) {
        cout << "Median of five algorithm failed!\n";
        cout << "Median : " << *med_iter << ", median of five : " << med
             << '\n';
        fc::print(v);
      }
      durations.push_back(
          chrono::duration_cast<chrono::duration<float, micro>>(end - start)
              .count());
    }
    auto stat = fc::get_statistics(durations);
    fc::log("Median of five Time to process a range of {:6} elements:\n"
            "Average : {:10.4f} us,\n"
            "Stdev   : {:10.4f} us,\n"
            "95%     : {:10.4f} us,\n"
            "99%     : {:10.4f} us,\n"
            "99.9%   : {:10.4f} us,\n",
            fc::log_level::all, 5, stat.average, stat.stdev, stat.percentile_95,
            stat.percentile_99, stat.percentile_999);
  }
}