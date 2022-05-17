#ifndef __CLRS4_SELECTION_IN_EXPECTED_LINEAR_TIME_H__
#define __CLRS4_SELECTION_IN_EXPECTED_LINEAR_TIME_H__

#include <07_quicksort/easy/03_randomized_quicksort.h>
#include <algorithm>
#include <common.h>
#include <limits>
#include <vector>

namespace frozenca {

using namespace std;

namespace {

int randomized_select_impl(vector<int> &A, index_t p, index_t r, index_t i) {
  if (p + 1 == r) {
    return A[p]; // 0 <= i <= r - p - 1 when p + 1 == r means i = 0
  } else if (p == r) {
    return -1;
  }
  auto q = randomized_partition_impl(A, p, r);
  auto k = q - p;
  if (i == k) {
    return A[q]; // the pivot value is the answer.
  } else if (i < k) {
    return randomized_select_impl(A, p, q, i);
  } else {
    return randomized_select_impl(A, q + 1, r, i - 1 - k);
  }
}

} // anonymous namespace

int randomized_select(vector<int> &A, index_t i) {
  return randomized_select_impl(A, 0, ssize(A), i);
}

int randomized_select_iterative(vector<int> &A, index_t i) {
  index_t p = 0;
  index_t r = ssize(A);
  while (true) {
    if (p + 1 == r) {
      return A[p];
    }
    auto q = randomized_partition_impl(A, p, r);
    auto k = q - p;
    if (i == k) {
      return A[q];
    } else if (i < k) {
      r = q;
    } else {
      p = q + 1;
      i -= (k + 1);
    }
  }
}

pair<index_t, int> randomized_select_iterative_pair(vector<int> &A, index_t p, index_t r, index_t i) {
  while (true) {
    if (p + 1 == r) {
      return {p, A[p]};
    }
    auto q = randomized_partition_impl(A, p, r);
    auto k = q - p;
    if (i == k) {
      return {q, A[q]};
    } else if (i < k) {
      r = q;
    } else {
      p = q + 1;
      i -= (k + 1);
    }
  }
}

} // namespace frozenca

#endif //__CLRS4_SELECTION_IN_EXPECTED_LINEAR_TIME_H__
