#ifndef __CLRS4_SUM_ARRAY_H__
#define __CLRS4_SUM_ARRAY_H__

#include "../../core/common.h"

namespace frozenca {

using namespace std;

struct sum_array_func {
    template<input_iterator Iter,
            typename T = iter_value_t<Iter>,
            typename BinaryOp = plus<>>
    requires is_nothrow_convertible_v<invoke_result_t<BinaryOp, T, T>, T>
    constexpr T operator()(Iter first, Iter last, T init = {}, BinaryOp op = {}) const {
        for (; first != last; ++first) {
            init = op(move(init), *first);
        }
        return init;
    }

    template <ranges::input_range Range,
            typename T = ranges::range_value_t<Range>,
            typename BinaryOp = plus<>>
    requires is_nothrow_convertible_v<invoke_result_t<BinaryOp, T, T>, T>
    constexpr T operator()(Range&& r, T init = {}, BinaryOp op = {}) const {
        return (*this)(ranges::begin(r), ranges::end(r), move(init), move(op));
    }
};

inline constexpr sum_array_func sum_array{};

} // namespace frozenca

#endif //__CLRS4_SUM_ARRAY_H__
