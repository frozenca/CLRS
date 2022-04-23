#ifndef __CLRS4_HOARE_PARTITION_H__
#define __CLRS4_HOARE_PARTITION_H__

#include <cassert>
#include <common.h>

namespace frozenca {

using namespace std;

index_t partition_hoare_impl(vector<int> &A, index_t p, index_t r) {
  assert(p >= 0 && r <= ssize(A));
  auto x = A[p];
  index_t i = p - 1;
  index_t j = r;
  while (true) {
    do {
      --j;
    } while (A[j] > x);
    do {
      ++i;
    } while (A[i] < x);
    if (i < j) {
      swap(A[i], A[j]);
    } else {
      return j + 1;
    }
  }
}

namespace {

void quicksort_hoare_impl(vector<int> &A, index_t p, index_t r) {
  if (r - p > 1) {
    assert(p >= 0 && r <= ssize(A));
    // partition the subarray around the pivot, which ends up in A[q]
    auto q = partition_hoare_impl(A, p, r);
    quicksort_hoare_impl(A, p, q); // recursively sort the low side
    quicksort_hoare_impl(A, q, r); // recursively sort the high side
  }
}

} // anonymous namespace

void quicksort_hoare(vector<int> &A) { quicksort_hoare_impl(A, 0, ssize(A)); }

} // namespace frozenca

#endif //__CLRS4_HOARE_PARTITION_H__
