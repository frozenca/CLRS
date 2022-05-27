#ifndef __CLRS4_WEE_HASH_FUNCTION_H__
#define __CLRS4_WEE_HASH_FUNCTION_H__

#include <common.h>

namespace frozenca::hard {

using namespace std;

template <size_t a> struct WeeFBase {
  constexpr size_t operator()(size_t k) const {
    return halfswap(2 * k * k + a * k);
  }
};

template <size_t a, size_t r> struct WeeF {
  constexpr size_t operator()(size_t k) const {
    if constexpr (r > 0) {
      return WeeF<a, r - 1>{}(k);
    } else {
      return WeeFBase<a>{}(k);
    }
  }
};

template <size_t a, size_t b, size_t t, size_t r> struct Wee {
  constexpr size_t operator()(size_t k) const {
    return WeeF<a + 2 * t, r>{}(k + b);
  }
};

} // namespace frozenca::hard

#endif //__CLRS4_WEE_HASH_FUNCTION_H__
