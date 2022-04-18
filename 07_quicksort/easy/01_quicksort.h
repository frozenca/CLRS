#ifndef __CLRS4_QUICKSORT_H__
#define __CLRS4_QUICKSORT_H__

#include <cassert>
#include <common.h>

namespace frozenca {

using namespace std;

index_t partition_impl(vector<int>& A, index_t p, index_t r) {
  assert(p >= 0 && r <= ssize(A));
  auto x = A[r - 1];                      // the pivot
  auto i = p - 1;                         // highest index into the low side
  for (index_t j = p; j < r - 1; ++j) {   // process each element other than the pivot
    if (A[j] <= x) {                      // does this element belong on the low side?
      ++i;                                // index of a new slot in the low side
      swap(A[i], A[j]);                   // put this element there
    }
  }
  swap(A[i + 1], A[r - 1]);               // pivot goes just to the right of the low side
  return i + 1;                           // new index of the pivot
}

namespace {

void quicksort_impl(vector<int>& A, index_t p, index_t r) {
  if (r - p > 1) {
    assert(p >= 0 && r <= ssize(A));
    // partition the subarray around the pivot, which ends up in A[q]
    auto q = partition_impl(A, p, r);
    quicksort_impl(A, p, q);                // recursively sort the low side
    quicksort_impl(A, q + 1, r);            // recursively sort the high side
  }
}

index_t partition_impl_decreasing(vector<int>& A, index_t p, index_t r) {
  assert(p >= 0 && r <= ssize(A));
  auto x = A[r - 1];                      // the pivot
  auto i = p - 1;                         // highest index into the low side
  for (index_t j = p; j < r - 1; ++j) {   // process each element other than the pivot
    if (A[j] >= x) {                      // does this element belong on the low side?
      ++i;                                // index of a new slot in the low side
      swap(A[i], A[j]);                   // put this element there
    }
  }
  swap(A[i + 1], A[r - 1]);               // pivot goes just to the right of the low side
  return i + 1;                           // new index of the pivot
}

void quicksort_impl_decreasing(vector<int>& A, index_t p, index_t r) {
  if (r - p > 1) {
    assert(p >= 0 && r <= ssize(A));
    // partition the subarray around the pivot, which ends up in A[q]
    auto q = partition_impl_decreasing(A, p, r);
    quicksort_impl_decreasing(A, p, q);                // recursively sort the low side
    quicksort_impl_decreasing(A, q + 1, r);            // recursively sort the high side
  }
}

} // anonymous namespace

void quicksort(vector<int>& A) {
  quicksort_impl(A, 0, ssize(A));
}

void quicksort_decreasing(vector<int>& A) {
  quicksort_impl_decreasing(A, 0, ssize(A));
}

} // namespace frozenca

#endif //__CLRS4_QUICKSORT_H__
