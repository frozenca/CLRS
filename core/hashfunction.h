#ifndef __CLRS4_HASH_FUNCTION_H__
#define __CLRS4_HASH_FUNCTION_H__

#include <bit>
#include <cstring>
#include <common.h>
#include <span>

namespace frozenca::hard {

using namespace std;
namespace detail {

inline constexpr size_t hash_a_base = 123UL;

template <size_t a> struct HashFunc {
  constexpr size_t operator()(size_t k) const noexcept {
    return 2 * k * k + a * k;
  }
};

template <unsigned_integral T, size_t a> struct HashBase {
  constexpr size_t operator()(T k) const noexcept {
    return HashFunc<a + sizeof(T) * 16>{}(static_cast<size_t>(k));
  }
};

template <Scalar T, size_t a> struct HashImpl {
  constexpr size_t operator()(const T &key) const noexcept {
    if constexpr (sizeof(T) == sizeof(uint64_t)) {
      return HashBase<uint64_t, a>{}(bit_cast<uint64_t>(key));
    } else if constexpr (sizeof(T) == sizeof(uint8_t)) {
      return HashBase<uint8_t, a>{}(bit_cast<uint8_t>(key));
    } else if constexpr (sizeof(T) == sizeof(uint16_t)) {
      return HashBase<uint16_t, a>{}(bit_cast<uint16_t>(key));
    } else if constexpr (sizeof(T) == sizeof(uint32_t)) {
      return HashBase<uint32_t, a>{}(bit_cast<uint32_t>(key));
    } else {
      []<bool flag = false>() {
        static_assert(flag, "Not supported sizeof(T)");
      }
      ();
    }
  }
};

template <SpanType T>
using SpanBaseType = remove_pointer_t<decltype(data<T>(declval<T>()))>;
template <size_t a> struct HashSpan {
  size_t operator()(span<const size_t> arr) const noexcept {
    size_t val = arr.size();
    for (auto num : arr) {
      val = detail::HashBase<size_t, a>{}(num + val);
    }
    return val;
  }
};

} // namespace detail

template <typename T, size_t a = detail::hash_a_base> struct Hash {
  constexpr size_t operator()(const T &key) const noexcept requires(Scalar<T>) {
    // negative zero -> positive zero for floating points
    if constexpr (is_same_v<T, float>) {
      return detail::HashImpl<T, a>{}(key == 0.0f ? 0.0f : key);
    } else if constexpr (is_same_v<T, double>) {
      return detail::HashImpl<T, a>{}(key == 0.0 ? 0.0 : key);
    } else if constexpr (is_same_v<T, long double>) {
      return detail::HashImpl<T, a>{}(key == 0.0l ? 0.0l : key);
    } else {
      return detail::HashImpl<T, a>{}(key);
    }
  }

  constexpr size_t operator()(const T &key) const noexcept requires(SpanType<T>) {
    using base_type = detail::SpanBaseType<T>;

    auto num_bytes = key.size() * sizeof(base_type);

    if (num_bytes == 0) {
      return 0UL;
    } else {
      auto num_words = num_bytes / sizeof(size_t);
      size_t val = 0;
      if (num_words) {
        val = detail::HashSpan<a>{}(span{reinterpret_cast<const size_t *>(key.data()), num_words});
      }
      auto last_bytes = num_bytes % sizeof(size_t);
      if (last_bytes) {
        size_t last_word = 0;
        memcpy(&last_word, key.data() + num_words, last_bytes);
        return detail::HashBase<size_t, a>{}(last_word + val);
      } else {
        return val;
      }
    }
  }
};

} // namespace frozenca::hard

#endif //__CLRS4_HASH_FUNCTION_H__
