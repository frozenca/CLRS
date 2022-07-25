#ifndef __CLRS4_QUICKSELECT_IMPL_HARD_H__
#define __CLRS4_QUICKSELECT_IMPL_HARD_H__

#include <02_getting_started/hard/01_insertion_sort_hard.h>
#include <cassert>
#include <cmath>
#include <common.h>
#include <iostream>
#include <random>

namespace frozenca {

using namespace std;

namespace {

struct quickselect_func {

  // move median of (*a, *b, *c) to *result
  template <forward_iterator Iter, typename Comp = ranges::less,
            typename Proj = identity>
  inline void move_median_to(Iter result, Iter a, Iter b, Iter c,
                             Comp comp = {}, Proj proj = {}) const {
    if (invoke(comp, invoke(proj, *a), invoke(proj, *b))) {
      if (invoke(comp, invoke(proj, *b), invoke(proj, *c))) {
        iter_swap(result, b);
      } else if (invoke(comp, invoke(proj, *a), invoke(proj, *c))) {
        iter_swap(result, c);
      } else {
        iter_swap(result, a);
      }
    } else if (invoke(comp, invoke(proj, *a), invoke(proj, *c))) {
      iter_swap(result, a);
    } else if (invoke(comp, invoke(proj, *b), invoke(proj, *c))) {
      iter_swap(result, c);
    } else {
      iter_swap(result, b);
    }
  }

  template <random_access_iterator Iter, sentinel_for<Iter> Sentinel,
            typename Comp = ranges::less, typename Proj = identity>
  requires sortable<Iter, Comp, Proj>
  constexpr Iter unguarded_partition(Iter first, Sentinel last, Iter pivot,
                                     Comp comp = {}, Proj proj = {}) const {
    while (true) {
      while (invoke(comp, invoke(proj, *first), invoke(proj, *pivot))) {
        ++first;
      }
      --last;
      while (invoke(comp, invoke(proj, *pivot), invoke(proj, *last))) {
        --last;
      }
      if (!(first < last)) {
        return first;
      }
      iter_swap(first, last);
      ++first;
    }
  }

  template <random_access_iterator Iter, typename Comp = ranges::less,
            typename Proj = identity>
  requires sortable<Iter, Comp, Proj>
  constexpr Iter partition_pivot(Iter first, Iter last, Comp comp = {},
                                 Proj proj = {}) const {
    Iter mid = first + (last - first) / 2;
    move_median_to(first, first + 1, mid, last - 1, comp, proj);
    return unguarded_partition(first + 1, last, first, comp, proj);
  }

  static constexpr int sort_criteria = 3;

  template <random_access_iterator Iter, sentinel_for<Iter> Sentinel,
            typename Comp = ranges::less, typename Proj = identity>
  requires sortable<Iter, Comp, Proj>
  constexpr Iter operator()(Iter first, Iter nth, Sentinel last, Comp comp = {},
                            Proj proj = {}) const {
    if (first == last || nth == last) {
      return last;
    }
    Iter ret = last;
    auto depth = static_cast<index_t>(2 * log2(last - first));
    while (last - first > sort_criteria) {
      if (depth == 0) {
        auto mid = nth + 1;
        ranges::make_heap(first, mid, comp, proj);
        for (Iter it = mid; it < last; ++it) {
          if (invoke(comp, invoke(proj, *it), invoke(proj, *first))) {
            ranges::pop_heap(first, mid, comp, proj);
          }
        }
        iter_swap(first, nth);
        return ret;
      }
      --depth;
      Iter cut = partition_pivot(first, last, comp, proj);
      if (cut <= nth) {
        first = cut;
      } else {
        last = cut;
      }
    }
    insertion_sort(move(first), move(last), move(comp), move(proj));

    return ret;
  }

  template <ranges::random_access_range Range, typename Comp = ranges::less,
            typename Proj = identity>
  requires sortable<ranges::iterator_t<Range>, Comp, Proj>
  constexpr auto operator()(Range &&r, ranges::iterator_t<Range> nth,
                            Comp comp = {}, Proj proj = {}) const {
    return (*this)(ranges::begin(r), move(nth), ranges::end(r), move(comp),
                   move(proj));
  }
};

} // anonymous namespace

inline constexpr quickselect_func quickselect{};

} // namespace frozenca

#endif //__CLRS4_QUICKSELECT_IMPL_HARD_H__
