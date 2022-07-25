#ifndef __CLRS4_QUICKSORT_IMPL_HARD_H__
#define __CLRS4_QUICKSORT_IMPL_HARD_H__

#include <cassert>
#include <common.h>
#include <iostream>
#include <random>

namespace frozenca {

using namespace std;

namespace {

struct partition_range_func {
  template <permutable Iter, sentinel_for<Iter> Sentinel,
            typename Proj = identity,
            indirect_unary_predicate<projected<Iter, Proj>> Pred>
  constexpr ranges::subrange<Iter> operator()(Iter first, Sentinel last,
                                              Pred pred, Proj proj = {}) const {
    first = ranges::find_if_not(first, last, ref(pred), ref(proj));
    if (first == last) {
      return {first, first};
    }

    auto begin = first;
    for (auto i = ranges::next(first); i != last; ++i) {
      if (invoke(pred, invoke(proj, *i))) {
        ranges::iter_swap(i, first);
        ++first;
      }
    }
    return {move(first), move(last)};
  }

  template <
      ranges::forward_range Range, typename Proj = identity,
      indirect_unary_predicate<projected<ranges::iterator_t<Range>, Proj>> Pred>
  requires permutable<ranges::iterator_t<Range>>
  constexpr auto operator()(Range &&r, Pred pred, Proj proj = {}) const {
    return (*this)(ranges::begin(r), ranges::end(r), move(pred), move(proj));
  }
};

struct partition_naive_func {
  template <forward_iterator Iter, sentinel_for<Iter> Sentinel,
            typename Comp = ranges::less, typename Proj = identity>
  constexpr Iter operator()(Iter first, Sentinel last, Comp comp = {},
                            Proj proj = {}) const {
    auto pivot = invoke(proj, *(--last));
    auto i = first;
    for (auto j = first; j != last; ++j) {
      if (invoke(comp, invoke(proj, *j), pivot)) {
        iter_swap(i, j);
        ++i;
      }
    }
    iter_swap(i, last);
    return i;
  }

  template <ranges::forward_range Range, typename Comp = ranges::less,
            typename Proj = identity>
  requires permutable<ranges::iterator_t<Range>>
  constexpr auto operator()(Range &&r, Comp comp = {}, Proj proj = {}) const {
    return (*this)(ranges::begin(r), ranges::end(r), move(comp), move(proj));
  }
};

} // anonymous namespace

inline constexpr partition_range_func partition_range{};
inline constexpr partition_naive_func partition_naive{};

namespace {

struct quicksort_func {
  template <forward_iterator Iter, sentinel_for<Iter> Sentinel,
            typename Comp = ranges::less, typename Proj = identity>
  requires sortable<Iter, Comp, Proj>
  constexpr void operator()(Iter first, Sentinel last, Comp cmp = {},
                            Proj proj = {}) const {
    using reference = iter_reference_t<Iter>;

    if (first == last) {
      return;
    }

    auto size = ranges::distance(first, last);
    auto pivot = ranges::next(first, size - 1);
    ranges::iter_swap(pivot, ranges::next(first, size / 2));

    ranges::subrange tail = partition_range(
        first, pivot,
        [=](reference em) {
          // em < pivot
          return invoke(cmp, em, invoke(proj, *pivot));
        },
        proj);

    ranges::iter_swap(pivot, tail.begin());
    (*this)(first, tail.begin(), ref(cmp), ref(proj));
    (*this)(ranges::next(tail.begin()), last, ref(cmp), ref(proj));
  }

  template <ranges::forward_range Range, typename Comp = ranges::less,
            typename Proj = identity>
  requires sortable<ranges::iterator_t<Range>, Comp, Proj>
  constexpr void operator()(Range &&r, Comp comp = {}, Proj proj = {}) const {
    (*this)(ranges::begin(r), ranges::end(r), move(comp), move(proj));
  }
};

struct randomized_quicksort_func {
  template <forward_iterator Iter, sentinel_for<Iter> Sentinel,
            typename Comp = ranges::less, typename Proj = identity>
  requires sortable<Iter, Comp, Proj>
  constexpr void operator()(Iter first, Sentinel last, Comp cmp = {},
                            Proj proj = {}) const {
    using reference = iter_reference_t<Iter>;

    if (first == last) {
      return;
    }

    auto size = ranges::distance(first, last);
    auto pivot = ranges::next(first, size - 1);
    mt19937 gen(random_device{}());
    uniform_int_distribution<index_t> dist(0, size - 1);
    ranges::iter_swap(pivot, ranges::next(first, dist(gen)));

    ranges::subrange tail = partition_range(
        first, pivot,
        [=](reference em) {
          // em < pivot
          return invoke(cmp, em, invoke(proj, *pivot));
        },
        proj);

    ranges::iter_swap(pivot, tail.begin());
    (*this)(first, tail.begin(), ref(cmp), ref(proj));
    (*this)(ranges::next(tail.begin()), last, ref(cmp), ref(proj));
  }

  template <ranges::forward_range Range, typename Comp = ranges::less,
            typename Proj = identity>
  requires sortable<ranges::iterator_t<Range>, Comp, Proj>
  constexpr void operator()(Range &&r, Comp comp = {}, Proj proj = {}) const {
    (*this)(ranges::begin(r), ranges::end(r), move(comp), move(proj));
  }
};

} // anonymous namespace

inline constexpr quicksort_func quicksort{};
inline constexpr randomized_quicksort_func randomized_quicksort{};

namespace {

struct partition_hoare_func {
  template <bidirectional_iterator Iter, sentinel_for<Iter> Sentinel,
            typename Comp = ranges::less, typename Proj = identity>
  requires sortable<Iter, Comp, Proj>
  constexpr Iter operator()(Iter first, Sentinel last, Comp comp = {},
                            Proj proj = {}) const {
    auto x = invoke(proj, *first);
    auto i = first;
    auto j = last;
    while (true) {
      do {
        --j;
      } while (invoke(comp, x, invoke(proj, *j)));
      while (invoke(comp, invoke(proj, *i), x)) {
        ++i;
      }
      if (ranges::distance(i, j) > 0) {
        ranges::iter_swap(i, j);
      } else {
        return ranges::next(j);
      }
    }
  }

  template <ranges::bidirectional_range Range, typename Comp = ranges::less,
            typename Proj = identity>
  requires sortable<ranges::iterator_t<Range>, Comp, Proj>
  constexpr auto operator()(Range &&r, Comp comp = {}, Proj proj = {}) const {
    return (*this)(ranges::begin(r), ranges::end(r), move(comp), move(proj));
  }
};

} // anonymous namespace

inline constexpr partition_hoare_func partition_hoare{};

namespace {

struct quicksort_hoare_func {
  template <bidirectional_iterator Iter, sentinel_for<Iter> Sentinel,
            typename Comp = ranges::less, typename Proj = identity>
  requires sortable<Iter, Comp, Proj>
  constexpr void operator()(Iter first, Sentinel last, Comp cmp = {},
                            Proj proj = {}) const {
    using reference = iter_reference_t<Iter>;

    if (ranges::distance(first, last) < 2) {
      return;
    }
    auto mid = partition_hoare(first, last, ref(cmp), ref(proj));
    (*this)(first, mid, ref(cmp), ref(proj));
    (*this)(mid, last, ref(cmp), ref(proj));
  }

  template <ranges::bidirectional_range Range, typename Comp = ranges::less,
            typename Proj = identity>
  requires sortable<ranges::iterator_t<Range>, Comp, Proj>
  constexpr void operator()(Range &&r, Comp comp = {}, Proj proj = {}) const {
    (*this)(ranges::begin(r), ranges::end(r), move(comp), move(proj));
  }
};

} // anonymous namespace

inline constexpr quicksort_hoare_func quicksort_hoare{};

namespace {

struct partition_threeway_func {
  template <permutable Iter, sentinel_for<Iter> Sentinel,
            typename Comp = ranges::less, typename Proj = identity>
  requires sortable<Iter, Comp, Proj>
  constexpr ranges::subrange<Iter>
  operator()(Iter first, Sentinel last, Comp comp = {}, Proj proj = {}) const {
    auto x = invoke(proj, *first);
    auto l = first;
    auto m = first;
    auto h = last;
    while (m != h) {
      if (invoke(comp, invoke(proj, *m), x)) {
        ranges::iter_swap(l, m);
        ++l;
        ++m;
      } else if (invoke(comp, x, invoke(proj, *m))) {
        --h;
        ranges::iter_swap(m, h);
      } else {
        ++m;
      }
    }
    return {move(l), move(m)};
  }

  template <ranges::bidirectional_range Range, typename Comp = ranges::less,
            typename Proj = identity>
  requires sortable<ranges::iterator_t<Range>, Comp, Proj>
  constexpr auto operator()(Range &&r, Comp comp = {}, Proj proj = {}) const {
    return (*this)(ranges::begin(r), ranges::end(r), move(comp), move(proj));
  }
};

} // anonymous namespace

inline constexpr partition_threeway_func partition_threeway{};

namespace {

struct quicksort_threeway_func {
  template <bidirectional_iterator Iter, sentinel_for<Iter> Sentinel,
            typename Comp = ranges::less, typename Proj = identity>
  requires sortable<Iter, Comp, Proj>
  constexpr void operator()(Iter first, Sentinel last, Comp cmp = {},
                            Proj proj = {}) const {
    using reference = iter_reference_t<Iter>;

    if (ranges::distance(first, last) < 2) {
      return;
    }
    auto [l, m] = partition_threeway(first, last, ref(cmp), ref(proj));
    (*this)(first, l, ref(cmp), ref(proj));
    (*this)(m, last, ref(cmp), ref(proj));
  }

  template <ranges::bidirectional_range Range, typename Comp = ranges::less,
            typename Proj = identity>
  requires sortable<ranges::iterator_t<Range>, Comp, Proj>
  constexpr void operator()(Range &&r, Comp comp = {}, Proj proj = {}) const {
    (*this)(ranges::begin(r), ranges::end(r), move(comp), move(proj));
  }
};

} // anonymous namespace

inline constexpr quicksort_threeway_func quicksort_threeway{};

namespace {

struct quicksort_tre_func {
  template <forward_iterator Iter, sentinel_for<Iter> Sentinel,
            typename Comp = ranges::less, typename Proj = identity>
  requires sortable<Iter, Comp, Proj>
  constexpr void operator()(Iter first, Sentinel last, Comp cmp = {},
                            Proj proj = {}) const {
    auto p = first;
    auto r = last;
    while (ranges::distance(p, r) > 1) {
      auto size = ranges::distance(p, r);
      auto q = partition_naive(p, r, cmp, proj);
      auto m = ranges::next(p, size / 2);
      if (ranges::distance(q, m) > 0) {
        (*this)(p, q, cmp, proj);
        p = ranges::next(q);
      } else {
        (*this)(q, r, cmp, proj);
        r = q;
      }
    }
  }

  template <ranges::forward_range Range, typename Comp = ranges::less,
            typename Proj = identity>
  requires sortable<ranges::iterator_t<Range>, Comp, Proj>
  constexpr void operator()(Range &&r, Comp comp = {}, Proj proj = {}) const {
    (*this)(ranges::begin(r), ranges::end(r), move(comp), move(proj));
  }
};

} // anonymous namespace

inline constexpr quicksort_tre_func quicksort_tre{};

} // namespace frozenca

#endif //__CLRS4_QUICKSORT_IMPL_HARD_H__
