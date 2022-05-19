#ifndef __CLRS4_HASH_FUNCTIONS_H__
#define __CLRS4_HASH_FUNCTIONS_H__

#include <bit>
#include <cmath>
#include <vector>
#include <string>
#include <common.h>

namespace frozenca {

using namespace std;

struct HashFunctionDivision {
  size_t m_ = 89;
  HashFunctionDivision() = default;
  HashFunctionDivision(size_t m) : m_{m} {}
  size_t operator()(size_t key) const { return key % m_; }
};

struct HashFunctionMultiplication {
  float A_ = 0.35724f;
  size_t m_ = 89;
  HashFunctionMultiplication() = default;
  HashFunctionMultiplication(float A, size_t m) : A_{A}, m_{m} {}
  size_t operator()(size_t key) const {
    return static_cast<size_t>(m_ * ((key * A_) - floor(key * A_)));
  }
};

struct HashFunctionMultiplyShift {
  size_t a_ = 2654435769;
  HashFunctionMultiplyShift() = default;
  HashFunctionMultiplyShift(size_t a) : a_{a} {}
  size_t operator()(size_t key) const {
    return (key * a_) >> (size_bytes() - hashtable_default_width);
  }
};

template <typename T> requires (sizeof(T) == sizeof(size_t))
struct HashFunctionVector {
  size_t m_ = 7919;
  HashFunctionMultiplyShift h = {};

  size_t operator()(const vector<T>& v) const {
    size_t key = 0;
    for (const auto& n : v) {
      key *= m_;
      auto n_repr = bit_cast<size_t>(n);
      key += n_repr;
    }
    return h(key);
  }
};

struct HashFunctionString {
  size_t m_ = 7919;
  HashFunctionMultiplyShift h = {};
  size_t operator()(const string& v) const {
    size_t key = 0;
    for (const auto& c : v) {
      key *= m_;
      auto n_repr = bit_cast<unsigned char>(c);
      key += n_repr;
    }
    return h(key);
  }
};

} // namespace frozenca

#endif //__CLRS4_HASH_FUNCTIONS_H__
