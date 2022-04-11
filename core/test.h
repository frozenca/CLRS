#ifndef __CLRS4_TEST_H__
#define __CLRS4_TEST_H__

#include "util.h"
#include <algorithm>
#include <cassert>
#include <chrono>
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <random>
#include <ranges>
#include <stdexcept>
#include <utility>
#include <vector>

namespace frozenca {

using namespace std;

mt19937 gen(random_device{}());

namespace {

template <ranges::forward_range Range, typename Func1, typename Func2,
          typename... Args>
requires regular_invocable<Func1, Range, Args...> &&
    regular_invocable<Func2, Range, Args...>
void range_verify(Func1 &&f1, Func2 &&f2, int num_trials, int max_length,
                  Args &&...args) {
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

template <ranges::forward_range Range, typename Func, typename... Args>
requires regular_invocable<Func, Range, Args...>
void range_check_perf(Func &&f, int num_trials, const vector<int> &max_lengths,
                      Args &&...args) {
  for (auto max_length : max_lengths) {
    chrono::duration<double, micro> curr_length_duration(0);
    uniform_int_distribution<> len_dist(0, max_length);
    for (int i = 0; i < num_trials; ++i) {
      Range v;
      int n = len_dist(gen);
      generate_n(back_inserter(v), n, ref(gen));
      auto start = chrono::steady_clock::now();
      f(v, args...);
      auto end = chrono::steady_clock::now();
      curr_length_duration += (end - start);
    }
    log("Time to process a range of {:6} elements : {:10.4f} us\n",
        log_level::all, max_length,
        (curr_length_duration.count() / num_trials));
  }
}

} // anonymous namespace

template <ranges::forward_range Range = vector<int>, typename Func,
          typename Comp = ranges::less, typename Proj = identity>
requires sortable<ranges::iterator_t<Range>, Comp, Proj> &&
    regular_invocable<Func, Range, Comp, Proj>
void verify_sorting(Func &&f, int num_trials = 1'000, int max_length = 1'000,
                    Comp comp = {}, Proj proj = {}) {
  range_verify<Range>(f, ranges::is_sorted, num_trials, max_length, comp, proj);
}

template <ranges::forward_range Range = vector<int>, typename Func,
          typename Comp = ranges::less, typename Proj = identity>
requires sortable<ranges::iterator_t<Range>, Comp, Proj> &&
    regular_invocable<Func, Range, Comp, Proj>
void perf_check_sorting(Func &&f, int num_trials = 1'000,
                        const vector<int> &max_lengths = {10, 30, 100, 300,
                                                          1'000, 3'000},
                        Comp comp = {}, Proj proj = {}) {
  range_check_perf<Range>(f, num_trials, max_lengths, comp, proj);
}

} // namespace frozenca

#endif //__CLRS4_TEST_H__
