#ifndef __CLRS4_COMMON_H__
#define __CLRS4_COMMON_H__

#include <algorithm>
#include <cassert>
#include <concepts>
#include <cstdint>
#include <functional>
#include <iterator>
#include <limits>
#include <memory>
#include <ranges>
#include <stdexcept>
#include <type_traits>
#include <utility>


namespace frozenca {

using namespace std;

using index_t = ptrdiff_t;
using uindex_t = make_unsigned_t<index_t>;

namespace {

constexpr size_t arr_default_length = 128;

inline constexpr size_t size_bytes() {
  if constexpr (numeric_limits<size_t>::max() == 0xFFFF) {
    return 16;
  } else if constexpr (numeric_limits<size_t>::max() == 0xFFFFFFFF) {
    return 32;
  } else {
    return 64;
  }
}

inline constexpr size_t size_half() {
  if constexpr (numeric_limits<size_t>::max() == 0xFFFF) {
    return 8;
  } else if constexpr (numeric_limits<size_t>::max() == 0xFFFFFFFF) {
    return 16;
  } else {
    return 32;
  }
}

inline constexpr size_t halfswap(size_t k) {
  return (k >> size_half()) + (k << size_half());
}

template <typename T>
concept Scalar = is_scalar_v<T>;

constexpr size_t hashtable_default_width = 14u;
constexpr size_t hashtable_default_length = (1u << hashtable_default_width);

} // anonymous namespace
} // namespace frozenca

#endif //__CLRS4_COMMON_H__
