#ifndef __CLRS4_PROBLEMS_02_01_HARD_H__
#define __CLRS4_PROBLEMS_02_01_HARD_H__

#include <02_getting_started/hard/01_insertion_sort_hard.h>
#include <core/common.h>

namespace frozenca::hard {

using namespace std;

namespace {

struct insertion_and_merge_sort_func {

  static constexpr int insertion_sort_criterion = 20;

  template <bidirectional_iterator Iter, sentinel_for<Iter> Sentinel,
            typename Comp = ranges::less, typename Proj = identity>
  requires sortable<Iter, Comp, Proj>
  constexpr Iter merge_impl(Iter first, Sentinel last,
                            iter_value_t<Iter> *temp_first,
                            iter_value_t<Iter> *temp_middle,
                            iter_value_t<Iter> *temp_last, Comp comp = {},
                            Proj proj = {}) const {
    uninitialized_move(first, last, temp_first);

    auto l_curr = temp_first;
    auto r_curr = temp_middle;
    auto A_curr = first;
    while (l_curr != temp_middle && r_curr != temp_last) {
      if (invoke(comp, invoke(proj, *l_curr), invoke(proj, *r_curr))) {
        *A_curr = move(*l_curr);
        ++l_curr;
      } else {
        *A_curr = move(*r_curr);
        ++r_curr;
      }
      ++A_curr;
    }

    while (l_curr != temp_middle) {
      *A_curr = move(*l_curr);
      ++l_curr;
      ++A_curr;
    }
    while (r_curr != temp_last) {
      *A_curr = move(*r_curr);
      ++r_curr;
      ++A_curr;
    }
    return A_curr;
  }

  template <bidirectional_iterator Iter, sentinel_for<Iter> Sentinel,
            typename Comp = ranges::less, typename Proj = identity>
  requires sortable<Iter, Comp, Proj>
  constexpr Iter operator()(Iter first, Sentinel last, Comp comp = {},
                            Proj proj = {},
                            iter_value_t<Iter> *temp_buffer = nullptr) const {
    const auto len = ranges::distance(first, last);
    assert(len >= 0);
    if (len < 2) {
      return last;
    } else if (len < insertion_sort_criterion) {
      return insertion_sort(move(first), move(last), move(comp), move(proj));
    }
    using value_t = iter_value_t<Iter>;
    bool to_delete = false;
    if (!temp_buffer) {
      temp_buffer = new value_t[len];
      to_delete = true;
    }
    const auto mid = next(first, len / 2);

    (*this)(first, mid, comp, proj, temp_buffer);
    (*this)(mid, last, comp, proj, temp_buffer);
    const auto ret =
        merge_impl(first, last, temp_buffer, temp_buffer + (len / 2),
                   temp_buffer + len, move(comp), move(proj));
    if (to_delete) {
      delete[] temp_buffer;
    }
    return ret;
  }

  template <ranges::bidirectional_range Range, typename Comp = ranges::less,
            typename Proj = identity>
  requires sortable<ranges::iterator_t<Range>, Comp, Proj>
  constexpr auto operator()(Range &&r, Comp comp = {}, Proj proj = {}) const {
    using value_t = ranges::range_value_t<Range>;
    auto temp_buffer = make_unique_for_overwrite<value_t[]>(ranges::size(r));
    const auto ret = (*this)(ranges::begin(r), ranges::end(r), move(comp),
                             move(proj), temp_buffer.get());
    return ret;
  }
};

} // anonymous namespace

inline constexpr insertion_and_merge_sort_func insertion_and_merge_sort{};

} // namespace frozenca::hard

#endif //__CLRS4_PROBLEMS_02_01_HARD_H__
