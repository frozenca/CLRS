#ifndef __CLRS4_TEST_H__
#define __CLRS4_TEST_H__

#include <algorithm>
#include <cassert>
#include <chrono>
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <numeric>
#include <random>
#include <ranges>
#include <stdexcept>
#include <util.h>
#include <utility>
#include <vector>

namespace frozenca {

using namespace std;

namespace {

template <ranges::forward_range Range, typename Func1, typename Func2,
          typename... Args>
requires regular_invocable<Func1, Range, Args...> &&
    regular_invocable<Func2, Range, Args...>
void range_verify(Func1 &&f1, Func2 &&f2, int num_trials, int max_length,
                  Args &&...args) {
  mt19937 gen(random_device{}());
  uniform_int_distribution<> len_dist(0, max_length);

  for (int i = 0; i < num_trials; ++i) {
    Range v;
    int n = len_dist(gen);
    generate_n(back_inserter(v), n, ref(gen));
    f1(v, args...);
    if (!f2(v, args...)) {
      throw runtime_error("Verification failed");
    }
  }
  log("Verification success!\n", log_level::all);
}

template <ranges::forward_range Range, typename Func1, typename Func2,
          typename... Args>
requires regular_invocable<Func1, Range, Args...> &&
    regular_invocable<Func2, Range, Args...>
void sorted_range_verify(Func1 &&f1, Func2 &&f2, int num_trials, int max_length,
                         Args &&...args) {
  mt19937 gen(random_device{}());
  uniform_int_distribution<> len_dist(0, max_length);
  uniform_real_distribution<> contain_zero(0.2);

  for (int i = 0; i < num_trials; ++i) {
    Range v;
    int n = len_dist(gen);
    generate_n(back_inserter(v), n, ref(gen));
    if (contain_zero(gen)) {
      v.push_back({});
    }
    ranges::sort(v);
    if (f1(v, args...) != f2(v, args...)) {
      throw runtime_error("Verification failed");
    }
  }
  log("Verification success!\n", log_level::all);
}

struct stats {
  float average = 0.0f;
  float stdev = 0.0f;
  float percentile_95 = 0.0f;
  float percentile_99 = 0.0f;
  float percentile_999 = 0.0f;
};

stats get_statistics(vector<float> &v) {
  auto n = ssize(v);
  if (n == 0) {
    return {};
  }
  stats s;
  s.average = accumulate(v.begin(), v.end(), 0.0f) / n;
  float variance = 0.0f;
  for (auto value : v) {
    variance += pow(value - s.average, 2.0f);
  }
  variance /= n;
  s.stdev = sqrt(variance);
  ranges::sort(v);
  s.percentile_95 = *(v.begin() + (19 * n / 20));
  s.percentile_99 = *(v.begin() + (99 * n / 100));
  s.percentile_999 = *(v.begin() + (999 * n / 1000));
  return s;
}

template <ranges::forward_range Range, typename Func, typename... Args>
requires regular_invocable<Func, Range, Args...>
void range_check_perf(Func &&f, int num_trials, const vector<int> &max_lengths,
                      Args &&...args) {
  mt19937 gen(random_device{}());
  for (auto max_length : max_lengths) {
    vector<float> durations;
    uniform_int_distribution<> len_dist(0, max_length);
    for (int i = 0; i < num_trials; ++i) {
      Range v;
      int n = len_dist(gen);
      generate_n(back_inserter(v), n, ref(gen));
      auto start = chrono::steady_clock::now();
      f(v, args...);
      auto end = chrono::steady_clock::now();
      durations.push_back(
          chrono::duration_cast<chrono::duration<float, micro>>(end - start)
              .count());
    }
    auto stat = get_statistics(durations);
    log("Time to process a range of {:6} elements:\n"
        "Average : {:10.4f} us,\n"
        "Stdev   : {:10.4f} us,\n"
        "95%     : {:10.4f} us,\n"
        "99%     : {:10.4f} us,\n"
        "99.9%   : {:10.4f} us,\n",
        log_level::all, max_length, stat.average, stat.stdev,
        stat.percentile_95, stat.percentile_99, stat.percentile_999);
  }
}

} // anonymous namespace

template <ranges::forward_range Range = vector<int>, typename Func,
          typename Comp = ranges::less, typename Proj = identity>
requires sortable<ranges::iterator_t<Range>, Comp, Proj> &&
    regular_invocable<Func, Range, Comp, Proj>
void verify_sorting(Func &&f, int num_trials = 1'000, int max_length = 1'000,
                    Comp comp = {}, Proj proj = {}) {
  range_verify<Range>(f, ranges::is_sorted, num_trials, max_length, move(comp),
                      move(proj));
}

template <ranges::forward_range Range = vector<int>, typename Func,
          typename Comp = ranges::less, typename Proj = identity>
requires sortable<ranges::iterator_t<Range>, Comp, Proj> &&
    regular_invocable<Func, Range, Comp, Proj>
void perf_check_sorting(Func &&f, int num_trials = 10'000,
                        const vector<int> &max_lengths = {10, 30, 100, 300,
                                                          1'000, 3'000, 10'000},
                        Comp comp = {}, Proj proj = {}) {
  range_check_perf<Range>(f, num_trials, max_lengths, move(comp), move(proj));
}

template <ranges::forward_range Range = vector<int>, typename Func,
          typename Comp = ranges::less, typename Proj = identity>
requires regular_invocable<Func, Range, ranges::range_value_t<Range>, Comp,
                           Proj>
void verify_binary_search(Func &&f, int num_trials = 1'000,
                          int max_length = 1'000, Comp comp = {},
                          Proj proj = {}) {
  sorted_range_verify<Range>(f, ranges::binary_search, num_trials, max_length,
                             ranges::range_value_t<Range>{}, move(comp),
                             move(proj));
}

template <ranges::random_access_range Range = vector<int>, typename Func,
          typename Comp = ranges::less, typename Proj = identity>
requires sortable<ranges::iterator_t<Range>, Comp, Proj> &&
    regular_invocable<Func, Range, Comp, Proj>
void verify_heapify(Func &&f, int num_trials = 1'000, int max_length = 10,
                    Comp comp = {}, Proj proj = {}) {
  range_verify<Range>(f, ranges::is_heap, num_trials, max_length, move(comp),
                      move(proj));
}

} // namespace frozenca

#endif //__CLRS4_TEST_H__
