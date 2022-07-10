#ifndef __CLRS4_INTERVAL_H__
#define __CLRS4_INTERVAL_H__

#include <cassert>

namespace frozenca {

struct Interval {
  float first_ = 0.0f;
  float second_ = 0.0f;
  Interval() = default;
  Interval(float first, float second) : first_{first}, second_{second} { assert(first_ <= second_); }
  [[nodiscard]] float length() const noexcept {
    return second_ - first_;
  }
};

} // namespace frozenca

#endif //__CLRS4_INTERVAL_H__
