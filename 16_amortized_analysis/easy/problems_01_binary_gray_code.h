#ifndef __CLRS4_BINARY_GRAY_CODE_H__
#define __CLRS4_BINARY_GRAY_CODE_H__

#include <cassert>
#include <common.h>
#include <bitset>
#include <vector>

namespace frozenca {

using namespace std;

index_t get_flipped(index_t i, index_t k) {
  assert(i >= 0 && k >= 1 && i < (1 << k) - 1);
  auto two_k = 1 << k;
  if (k == 1) {
    return 0;
  }
  auto half_k = two_k >> 1;
  if (i < half_k - 1) {
    return get_flipped(i, k - 1);
  } else if (i == half_k - 1) {
    return k - 1;
  } else {
    return get_flipped(two_k - 2 - i, k - 1);
  }
}

template <size_t k> 
vector<bitset<k>> get_gray_codes() requires (k > 0) {
  auto two_k = 1 << k;
  vector<bitset<k>> codes;
  codes.emplace_back(0);
  for (index_t i = 0; i < two_k - 1; ++i) {
    auto to_flip = codes.back();
    to_flip.flip(get_flipped(i, k));
    codes.push_back(move(to_flip));
  }
  return codes;
}

} // namespace frozenca

#endif //__CLRS4_BINARY_GRAY_CODE_H__
