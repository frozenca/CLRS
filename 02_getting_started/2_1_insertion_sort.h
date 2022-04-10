#ifndef __CLRS4_INSERTION_SORT_H__
#define __CLRS4_INSERTION_SORT_H__

#include <algorithm>
#include <cassert>
#include <concepts>
#include <functional>
#include <iterator>
#include <list>
#include <ranges>
#include <utility>
#include <vector>

namespace frozenca {

using namespace std;

namespace easy {

void insertion_sort(vector<int>& A) {
    long n = ssize(A);
    for (long i = 1; i < n; ++i) {
        int key = A[i];
        long j = i - 1;
        while (j >= 0 && A[j] > key) {
            A[j + 1] = A[j];
            --j;
        }
        A[j + 1] = key;
    }
}

} // namespace easy

struct insertion_sort_func {
    template<bidirectional_iterator Iter,
            sentinel_for <Iter> Sentinel,
            typename Comp = ranges::less,
            typename Proj = identity>
    requires sortable<Iter, Comp, Proj>
    constexpr Iter
    operator()(Iter first, Sentinel last, Comp comp = {}, Proj proj = {}) const {
        if (first == last) return first;
        for (auto i = next(first); i != last; ++i) {
            auto key = *i;
            auto j = i;
            while (j != first && !invoke(comp, invoke(proj, *prev(j)), invoke(proj, key))) {
                iter_swap(prev(j), j);
                --j;
            }
            *j = key;
        }
        return last;
    }

    template<ranges::bidirectional_range Range,
            typename Comp = ranges::less,
            typename Proj = identity>
    requires sortable<ranges::iterator_t<Range>, Comp, Proj>
    constexpr ranges::borrowed_iterator_t<Range>
    operator()(Range&& r, Comp comp = {}, Proj proj = {}) const {
        return (*this)(ranges::begin(r), ranges::end(r), move(comp), move(proj));
    }
};

inline constexpr insertion_sort_func insertion_sort{};

} // namespace frozenca

#endif //__CLRS4_INSERTION_SORT_H__
