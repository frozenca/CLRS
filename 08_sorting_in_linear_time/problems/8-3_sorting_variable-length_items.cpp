#include <08_sorting_in_linear_time/inc/easy.h>
#include <algorithm>
#include <iostream>
#include <random>
#include <string>

using namespace std;

static const string chars = "abcdefghijklmnopqrstuvwxyz";

string make_random_string() {
  string res;
  mt19937 gen(random_device{}());
  uniform_int_distribution<> char_dist(0, 25);
  uniform_int_distribution<> length_dist(1, 10);
  int l = length_dist(gen);
  for (int i = 0; i < l; i++) {
    res += chars[char_dist(gen)];
  }
  return res;
}

int main() {
  namespace fc = frozenca;

  {
    vector<uint16_t> A{9, 3, 7, 15, 11, 31, 33};
    fc::bucket_radix_sort(A);
    fc::print(A);
    if (ranges::is_sorted(A)) {
      cout << "Sorted\n";
    } else {
      cout << "Failed to sort\n";
    }
  }

  {
    constexpr int n = 15;
    constexpr int num_trials = 10'000;

    mt19937 gen(random_device{}());
    uniform_int_distribution<uint16_t> dist(1, (1 << n) - 1);

    {
      // std::sort
      vector<float> durations;
      for (int i = 0; i < num_trials; ++i) {
        vector<uint16_t> v;
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
      // bucket radix sort
      vector<float> durations;
      for (int i = 0; i < num_trials; ++i) {
        vector<uint16_t> v;
        for (int j = 0; j < n; ++j) {
          v.push_back(dist(gen));
        }
        auto start = chrono::steady_clock::now();
        fc::bucket_radix_sort(v);
        auto end = chrono::steady_clock::now();
        durations.push_back(
            chrono::duration_cast<chrono::duration<float, micro>>(end - start)
                .count());
      }
      auto stat = fc::get_statistics(durations);
      fc::log("Bucket sort Time to process a range of {:6} elements:\n"
              "Average : {:10.4f} us,\n"
              "Stdev   : {:10.4f} us,\n"
              "95%     : {:10.4f} us,\n"
              "99%     : {:10.4f} us,\n"
              "99.9%   : {:10.4f} us,\n",
              fc::log_level::all, n, stat.average, stat.stdev,
              stat.percentile_95, stat.percentile_99, stat.percentile_999);
    }
  }

  {
    constexpr int n = 25;
    constexpr int num_trials = 10'000;

    mt19937 gen(random_device{}());

    {
      // std::sort
      vector<float> durations;
      for (int i = 0; i < num_trials; ++i) {
        vector<string> v;
        for (int j = 0; j < n; ++j) {
          v.push_back(make_random_string());
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
      // bucket string sort
      vector<float> durations;
      for (int i = 0; i < num_trials; ++i) {
        vector<string> v;
        for (int j = 0; j < n; ++j) {
          v.push_back(make_random_string());
        }
        auto start = chrono::steady_clock::now();
        fc::bucket_string_sort(v);
        auto end = chrono::steady_clock::now();
        durations.push_back(
            chrono::duration_cast<chrono::duration<float, micro>>(end - start)
                .count());
      }
      auto stat = fc::get_statistics(durations);
      fc::log("Bucket string sort Time to process a range of {:6} elements:\n"
              "Average : {:10.4f} us,\n"
              "Stdev   : {:10.4f} us,\n"
              "95%     : {:10.4f} us,\n"
              "99%     : {:10.4f} us,\n"
              "99.9%   : {:10.4f} us,\n",
              fc::log_level::all, n, stat.average, stat.stdev,
              stat.percentile_95, stat.percentile_99, stat.percentile_999);
    }
  }
}