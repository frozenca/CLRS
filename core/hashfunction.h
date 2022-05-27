#ifndef __CLRS4_HASH_FUNCTION_H__
#define __CLRS4_HASH_FUNCTION_H__

#include <bit>
#include <common.h>

namespace frozenca::hard {

using namespace std;


template <size_t a> struct HashBase {
  constexpr size_t operator()(size_t k) const {
    return 2 * k * k + a * k;
  }
};

template <size_t a, size_t r> struct HashF {
  constexpr size_t operator()(size_t k) const {
    if constexpr (r > 0) {
      return HashF<a, r - 1>{}(k);
    } else {
      return HashBase<a>{}(k);
    }
  }
};

template <Scalar T, size_t a = 123UL, size_t r = 4UL> 
struct Hash {
  constexpr size_t operator()(const T& key) const {
    if constexpr (sizeof(T) == sizeof(size_t)) {
      return HashF<a + 128, r>{}(bit_cast<size_t>(key));
    } else if constexpr (sizeof(T) == sizeof(uint8_t)) {
      return HashF<a + 16, r>{}(static_cast<size_t>(bit_cast<uint8_t>(key)));
    } else if constexpr (sizeof(T) == sizeof(uint16_t)) {
      return HashF<a + 32, r>{}(static_cast<size_t>(bit_cast<uint16_t>(key)));
    } else if constexpr (sizeof(T) == sizeof(uint32_t)) {
      return HashF<a + 64, r>{}(static_cast<size_t>(bit_cast<uint32_t>(key)));
    } else {
      []<bool flag = false>() {
        static_assert(flag, "Not supported sizeof(T)");
      }();
    }
  }    
};


} // namespace frozenca::hard

#endif //__CLRS4_HASH_FUNCTION_H__
