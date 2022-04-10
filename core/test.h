#ifndef __CLRS4_TEST_H__
#define __CLRS4_TEST_H__

#include <algorithm>
#include <cassert>
#include <functional>
#include <iostream>
#include <iterator>
#include <random>
#include <ranges>
#include <stdexcept>
#include <utility>
#include <vector>
#include "util.h"

namespace frozenca {

using namespace std;

mt19937 gen(random_device{}());

namespace {

template<ranges::forward_range Range, typename Func1, typename Func2, typename... Args>
requires regular_invocable<Func1, Range, Args...> && regular_invocable<Func2, Range, Args...>
void range_verify(Func1&& f1, Func2&& f2, int num_trials, int max_length, Args&&... args) {
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
    cout << "Verification success!\n";

}

} // anonymous namespace

template <ranges::forward_range Range = vector<int>,
        typename Func,
        typename Comp = ranges::less,
        typename Proj = identity>
requires sortable<ranges::iterator_t<Range>, Comp, Proj> && regular_invocable<Func, Range, Comp, Proj>
void verify_sorting(Func&& f, int num_trials = 1'000, int max_length = 1'000,
                    Comp comp = {}, Proj proj = {}) {
    range_verify<Range>(f, ranges::is_sorted, num_trials, max_length, comp, proj);
}

} // namespace frozenca

#endif //__CLRS4_TEST_H__
