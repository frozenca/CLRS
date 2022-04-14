#ifndef __CLRS4_SELECTION_SORT_HARD_H__
#define __CLRS4_SELECTION_SORT_HARD_H__

#include <core/common.h>

namespace frozenca::hard {

using namespace std;

namespace {

struct selection_sort_func {
  template <forward_iterator Iter, sentinel_for<Iter> Sentinel,
            typename Comp = ranges::less, typename Proj = identity>
  requires sortable<Iter, Comp, Proj>
  constexpr Iter operator()(Iter first, Sentinel last, Comp comp = {},
                            Proj proj = {}) const {
    if (first == last)
      return first;
    for (auto i = first; i != last; ++i) {
      iter_swap(i, ranges::min_element(i, last, comp, proj));
    }
    return last;
  }

  template <ranges::forward_range Range, typename Comp = ranges::less,
            typename Proj = identity>
  requires sortable<ranges::iterator_t<Range>, Comp, Proj>
  constexpr auto operator()(Range &&r, Comp comp = {}, Proj proj = {}) const {
    return (*this)(ranges::begin(r), ranges::end(r), move(comp), move(proj));
  }
};

} // anonymous namespace

inline constexpr selection_sort_func selection_sort{};

} // namespace frozenca::hard

#endif //__CLRS4_SELECTION_SORT_HARD_H__
