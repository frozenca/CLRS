#ifndef __CLRS4_MEDIAN_THREE_PARTITION_H__
#define __CLRS4_MEDIAN_THREE_PARTITION_H__

#include <cassert>
#include <common.h>
#include <random>

namespace frozenca {

using namespace std;

index_t partition_median_three(vector<int> &A, index_t p, index_t r) {
  mt19937 gen(random_device{}());
  uniform_int_distribution<index_t> dist(p, r - 1);
  vector<index_t> temp{dist(gen), dist(gen), dist(gen)};
  ranges::sort(temp);
  auto x = A[temp[1]];
  swap(A[temp[1]], A[r - 1]);
  index_t i = p;
  for (index_t j = p; j < r - 1; j++) {
    if (A[j] <= x) {
      swap(A[i], A[j]);
      ++i;
    }
  }
  swap(A[i], A[r - 1]);
  return i;
}

namespace {

void quicksort_median_three_impl(vector<int> &A, index_t p, index_t r) {
  if (r - p > 1) {
    assert(p >= 0 && r <= ssize(A));
    // partition the subarray around the pivot, which ends up in A[q]
    auto q = partition_median_three(A, p, r);
    quicksort_median_three_impl(A, p, q);     // recursively sort the low side
    quicksort_median_three_impl(A, q + 1, r); // recursively sort the high side
  }
}

} // anonymous namespace

void quicksort_median_three(vector<int> &A) {
  quicksort_median_three_impl(A, 0, ssize(A));
}

} // namespace frozenca

#endif //__CLRS4_MEDIAN_THREE_PARTITION_H__
