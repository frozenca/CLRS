#ifndef __CLRS4_VARIANT_OF_RANDOMIZED_SELECTION_H__
#define __CLRS4_VARIANT_OF_RANDOMIZED_SELECTION_H__

#include <07_quicksort/easy/03_randomized_quicksort.h>
#include <algorithm>
#include <common.h>
#include <limits>
#include <vector>

namespace frozenca {

using namespace std;

namespace {

int simpler_randomized_select_impl(vector<int> &A, index_t p, index_t r,
                                   index_t i) {
  if (p + 1 == r) {
    return A[p]; // 0 <= i <= r - p - 1 when p + 1 == r means i = 0
  }
  auto q = randomized_partition_impl(A, p, r);
  auto k = q - p + 1;
  if (i <= k) {
    return simpler_randomized_select_impl(A, p, q + 1, i);
  } else {
    return simpler_randomized_select_impl(A, q + 1, r, i - k);
  }
}

} // anonymous namespace

int simpler_randomized_select(vector<int> &A, index_t i) {
  return simpler_randomized_select_impl(A, 0, ssize(A), i);
}

} // namespace frozenca

#endif //__CLRS4_VARIANT_OF_RANDOMIZED_SELECTION_H__
