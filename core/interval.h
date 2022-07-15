#ifndef __CLRS4_INTERVAL_H__
#define __CLRS4_INTERVAL_H__

#include <cassert>
#include <common.h>
#include <compare>
#include <iostream>


namespace frozenca {

struct Interval {
  float first_ = 0.0f;
  float second_ = 0.0f;
  Interval() = default;
  Interval(float first, float second) : first_{first}, second_{second} {
    assert(first_ <= second_);
  }
  [[nodiscard]] float length() const noexcept { return second_ - first_; }

  friend ostream &operator<<(ostream &os, const Interval &itv) {
    os << '(' << itv.first_ << ',' << itv.second_ << ')';
    return os;
  }
};

struct Rectangle2d {
  float x_ = 0.0f;
  float y_ = 0.0f;
  float w_ = 0.0f;
  float h_ = 0.0f;
  Rectangle2d() = default;
  Rectangle2d(float x, float y, float w, float h) : x_{x}, y_{y}, w_{w}, h_{h} {
    assert(w_ >= 0.0f && h_ >= 0.0f);
  }
  [[nodiscard]] float area() const noexcept { return w_ * h_; }
};

bool operator==(const Interval &itv1, const Interval &itv2) {
  return itv1.first_ == itv2.first_ && itv1.second_ == itv2.second_;
}

struct IntervalComp {
  partial_ordering operator()(const Interval &itv1, const Interval &itv2) {
    if (itv1.second_ < itv2.first_) {
      return partial_ordering::less;
    } else if (itv2.second_ < itv1.first_) {
      return partial_ordering::greater;
    } else {
      return partial_ordering::equivalent;
    }
  }
};

struct IntervalStartComp {
  partial_ordering operator()(const Interval &itv1, const Interval &itv2) {
    if (auto cmp = itv1.first_ <=> itv2.first_;
        cmp != partial_ordering::equivalent) {
      return cmp;
    }
    return itv1.second_ <=> itv2.second_;
  }
};

} // namespace frozenca

#endif //__CLRS4_INTERVAL_H__
