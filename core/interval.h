#ifndef __CLRS4_INTERVAL_H__
#define __CLRS4_INTERVAL_H__

#include <cassert>

namespace frozenca {

class Interval {
public:
  Interval(float a, float b) : a{a}, b{b} { assert(a <= b); }
  [[nodiscard]] float first() const { return a; }
  [[nodiscard]] float second() const { return b; }

private:
  float a = 0.0f;
  float b = 1.0f;
};

} // namespace frozenca

#endif //__CLRS4_INTERVAL_H__
