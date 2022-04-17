#ifndef __CLRS4_HEAP_IMPL_HARD_H__
#define __CLRS4_HEAP_IMPL_HARD_H__

#include <cassert>
#include <common.h>

namespace frozenca::hard {

using namespace std;

namespace {

template <random_access_iterator Iter, sentinel_for<Iter> Sentinel,
          typename Comp = ranges::less, typename Proj = identity>
requires sortable<Iter, Comp, Proj>
constexpr void sift(Iter first, Sentinel last, index_t i, Comp comp = {},
                    Proj proj = {}) {
  const auto n{ranges::distance(first, last)};
  while (true) {
    auto l{2 * i + 1};
    auto r{2 * i + 2};
    index_t next_index = i;
    if (l < n &&
        invoke(comp, invoke(proj, *(first + i)), invoke(proj, *(first + l)))) {
      next_index = l;
    }
    if (r < n && invoke(comp, invoke(proj, *(first + next_index)),
                        invoke(proj, *(first + r)))) {
      next_index = r;
    }
    if (next_index != i) {
      iter_swap(first + i, first + next_index);
      i = next_index;
    } else {
      return;
    }
  }
}

struct heapify_func {
  template <random_access_iterator Iter, sentinel_for<Iter> Sentinel,
            typename Comp = ranges::less, typename Proj = identity>
  requires sortable<Iter, Comp, Proj>
  constexpr Iter operator()(Iter first, Sentinel last, Comp comp = {},
                            Proj proj = {}) const {
    for (index_t i{ranges::distance(first, last) / 2 - 1}; i >= 0; --i) {
      sift(first, last, i, comp, proj);
    }
    return last;
  }

  template <ranges::random_access_range Range, typename Comp = ranges::less,
            typename Proj = identity>
  requires sortable<ranges::iterator_t<Range>, Comp, Proj>
  constexpr auto operator()(Range &&r, Comp comp = {}, Proj proj = {}) const {
    return (*this)(ranges::begin(r), ranges::end(r), move(comp), move(proj));
  }
};

} // anonymous namespace

inline constexpr heapify_func heapify{};

namespace {

struct push_heap_func {
  template <random_access_iterator Iter, sentinel_for<Iter> Sentinel,
            typename Comp = ranges::less, typename Proj = identity>
  requires sortable<Iter, Comp, Proj>
  constexpr Iter operator()(Iter first, Sentinel last, Comp comp = {},
                            Proj proj = {}) const {
    const auto n{ranges::distance(first, last)};
    const auto length{n};
    if (n > 1) {
      Iter last{first + n};
      n = (n - 2) / 2;
      Iter i{first + n};
      if (invoke(comp, invoke(proj, *i), invoke(proj, *--last))) {
        iter_value_t<Iter> v{ranges::iter_move(last)};
        do {
          *last = ranges::iter_move(i);
          last = i;
          if (n == 0)
            break;
          n = (n - 1) / 2;
          i = first + n;
        } while (invoke(comp, invoke(proj, *i), invoke(proj, v)));
        *last = move(v);
      }
    }
    return first + length;
  }

  template <ranges::random_access_range R, class Comp = ranges::less,
            class Proj = identity>
  requires sortable<ranges::iterator_t<R>, Comp, Proj>
  constexpr auto operator()(R &&r, Comp comp = {}, Proj proj = {}) const {
    return (*this)(ranges::begin(r), ranges::end(r), move(comp), move(proj));
  }
};

struct pop_heap_func {
  template <random_access_iterator Iter, sentinel_for<Iter> Sentinel,
            typename Comp = ranges::less, typename Proj = identity>
  requires sortable<Iter, Comp, Proj>
  constexpr Iter operator()(Iter first, Sentinel last, Comp comp = {},
                            Proj proj = {}) const {
    const auto n{ranges::distance(first, last)};
    const auto length{n};
    if (n > 1) {
      Iter new_last{first + n - 1};
      iter_swap(first, new_last);
      heapify(first, new_last, move(comp), move(proj));
    }
    return first + length;
  }

  template <ranges::random_access_range R, class Comp = ranges::less,
            class Proj = identity>
  requires sortable<ranges::iterator_t<R>, Comp, Proj>
  constexpr auto operator()(R &&r, Comp comp = {}, Proj proj = {}) const {
    return (*this)(ranges::begin(r), ranges::end(r), move(comp), move(proj));
  }
};

} // anonymous namespace

inline constexpr push_heap_func heap_push{};
inline constexpr pop_heap_func heap_pop{};

namespace {

struct sort_heap_func {
  template <random_access_iterator Iter, sentinel_for<Iter> Sentinel,
            typename Comp = ranges::less, class Proj = identity>
  requires sortable<Iter, Comp, Proj>
  constexpr Iter operator()(Iter first, Sentinel last, Comp comp = {},
                            Proj proj = {}) const {
    auto ret{ranges::next(first, last)};
    for (; first != last; --last) {
      heap_pop(first, last, comp, proj);
    }
    return ret;
  }

  template <ranges::random_access_range Range, typename Comp = ranges::less,
            typename Proj = identity>
  requires sortable<ranges::iterator_t<Range>, Comp, Proj>
  constexpr ranges::borrowed_iterator_t<Range>
  operator()(Range &&r, Comp comp = {}, Proj proj = {}) const {
    return (*this)(ranges::begin(r), ranges::end(r), move(comp), move(proj));
  }
};

} // anonymous namespace

inline constexpr sort_heap_func heap_sort{};


namespace {

struct make_and_sort_heap_func {
  template <random_access_iterator Iter, sentinel_for<Iter> Sentinel,
            typename Comp = ranges::less, class Proj = identity>
  requires sortable<Iter, Comp, Proj>
  constexpr Iter operator()(Iter first, Sentinel last, Comp comp = {},
                            Proj proj = {}) const {
    heapify(first, last, comp, proj);
    heap_sort(first, last, move(comp), move(proj));
    return last;
  }

  template <ranges::random_access_range Range, typename Comp = ranges::less,
            typename Proj = identity>
  requires sortable<ranges::iterator_t<Range>, Comp, Proj>
  constexpr ranges::borrowed_iterator_t<Range>
  operator()(Range &&r, Comp comp = {}, Proj proj = {}) const {
    return (*this)(ranges::begin(r), ranges::end(r), move(comp), move(proj));
  }
};

} // anonymous namespace

inline constexpr make_and_sort_heap_func make_heap_and_sort{};

} // namespace frozenca::hard

#endif //__CLRS4_HEAP_IMPL_HARD_H__
