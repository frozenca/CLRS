#ifndef __CLRS4_LINEAR_SEARCH_HARD_H__
#define __CLRS4_LINEAR_SEARCH_HARD_H__

#include "../../core/common.h"

namespace frozenca::hard {

using namespace std;

namespace {

struct linear_search_func {
  template <input_iterator Iter, sentinel_for<Iter> Sentinel, typename T,
            typename Proj = identity>
  requires indirect_binary_predicate < ranges::equal_to, projected<Iter, Proj>,
  const T * >
      constexpr iter_difference_t<Iter> operator()(Iter first, Sentinel last,
                                                   const T &value,
                                                   Proj proj = {}) const {
    auto it = first;
    for (; it != last; ++it) {
      if (invoke(proj, *it) == value) {
        return ranges::distance(first, it);
      }
    }
    return ranges::distance(first, it);
  }

  template <ranges::input_range Range, class T, class Proj = identity>
  requires indirect_binary_predicate < ranges::equal_to,
      projected<ranges::iterator_t<Range>, Proj>,
  const T * > constexpr auto operator()(Range &&r, const T &value,
                                        Proj proj = {}) const {
    return (*this)(ranges::begin(r), ranges::end(r), value, ref(proj));
  }
};

} // anonymous namespace

inline constexpr linear_search_func linear_search{};

} // namespace frozenca::hard

#endif //__CLRS4_LINEAR_SEARCH_HARD_H__
