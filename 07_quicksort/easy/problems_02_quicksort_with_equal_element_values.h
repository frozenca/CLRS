#ifndef __CLRS4_QUICKSORT_WITH_EQUAL_H__
#define __CLRS4_QUICKSORT_WITH_EQUAL_H__

#include <cassert>
#include <common.h>
#include <iostream>

namespace frozenca {

using namespace std;

pair<index_t, index_t> partition_threeway(vector<int> &A, index_t p,
                                          index_t r) {
  assert(p >= 0 && r <= ssize(A));
  auto x = A[r - 1];
  index_t l = p;
  index_t m = p;
  index_t h = r;
  while (m < h) {
    if (A[m] < x) {
      swap(A[l], A[m]);
      ++l;
      ++m;
    } else if (A[m] > x) {
      --h;
      swap(A[m], A[h]);
    } else {
      ++m;
    }
  }
  return {l, m};
}

namespace {

void quicksort_threeway_impl(vector<int> &A, index_t p, index_t r) {
  if (r - p > 1) {
    assert(p >= 0 && r <= ssize(A));
    // partition the subarray around the pivot, which ends up in A[q]
    auto [l, m] = partition_threeway(A, p, r);
    quicksort_threeway_impl(A, p, l); // recursively sort the low side
    quicksort_threeway_impl(A, m, r); // recursively sort the high side
  }
}

} // anonymous namespace

void quicksort_threeway(vector<int> &A) {
  quicksort_threeway_impl(A, 0, ssize(A));
}

} // namespace frozenca

#endif //__CLRS4_QUICKSORT_WITH_EQUAL_H__
