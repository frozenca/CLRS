#ifndef __CLRS4_QUICKSORT_WORST_NLOGN_H__
#define __CLRS4_QUICKSORT_WORST_NLOGN_H__

#include <09_medians_and_order_statistics/easy/03_selection_in_worst-case_linear_time.h>
#include <common.h>

namespace frozenca {

using namespace std;

namespace {

index_t partition_worst_nlogn_impl(vector<int> &A, index_t p, index_t r) {
  assert(p >= 0 && r <= ssize(A));
  auto pivot = select_impl(A, p, r, (r - p) / 2);
  index_t l = p;
  index_t m = p;
  index_t h = r;
  while (m < h) {
    if (A[m] < pivot) {
      swap(A[l], A[m]);
      ++l;
      ++m;
    } else if (A[m] > pivot) {
      --h;
      swap(A[m], A[h]);
    } else {
      ++m;
    }
  }
  return l;
}

void quicksort_worst_nlogn_impl(vector<int> &A, index_t p, index_t r) {
  if (r - p > 1) {
    assert(p >= 0 && r <= ssize(A));
    // partition the subarray around the pivot, which ends up in A[q]
    auto q = partition_worst_nlogn_impl(A, p, r);
    quicksort_worst_nlogn_impl(A, p, q);     // recursively sort the low side
    quicksort_worst_nlogn_impl(A, q + 1, r); // recursively sort the high side
  }
}

} // namespace

void quicksort_worst_nlogn(vector<int> &A) {
  quicksort_worst_nlogn_impl(A, 0, ssize(A));
}

} // namespace frozenca

#endif //__CLRS4_QUICKSORT_WORST_NLOGN_H__
