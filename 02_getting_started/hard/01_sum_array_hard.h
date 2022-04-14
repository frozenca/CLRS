#ifndef __CLRS4_SUM_ARRAY_HARD_H__
#define __CLRS4_SUM_ARRAY_HARD_H__

#include <core/common.h>

namespace frozenca::hard {

using namespace std;

namespace {

struct sum_array_func {
  template <input_iterator Iter, typename BinaryOp = plus<>>
  requires is_nothrow_convertible_v<
      invoke_result_t<BinaryOp, iter_value_t<Iter>, iter_value_t<Iter>>,
      iter_value_t<Iter>>
  constexpr iter_value_t<Iter> operator()(Iter first, Iter last,
                                          iter_value_t<Iter> init = {},
                                          BinaryOp op = {}) const {
    for (; first != last; ++first) {
      init = op(move(init), *first);
    }
    return init;
  }

  template <ranges::input_range Range, typename BinaryOp = plus<>>
  requires is_nothrow_convertible_v<
      invoke_result_t<BinaryOp, ranges::range_value_t<Range>,
                      ranges::range_value_t<Range>>,
      ranges::range_value_t<Range>>
  constexpr auto operator()(Range &&r, ranges::range_value_t<Range> init = {},
                            BinaryOp op = {}) const {
    return (*this)(ranges::begin(r), ranges::end(r), move(init), move(op));
  }
};

} // anonymous namespace

inline constexpr sum_array_func sum_array{};

} // namespace frozenca::hard

#endif //__CLRS4_SUM_ARRAY_HARD_H__
