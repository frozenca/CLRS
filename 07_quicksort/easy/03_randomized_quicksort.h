#ifndef __CLRS4_RANDOMIZED_QUICKSORT_H__
#define __CLRS4_RANDOMIZED_QUICKSORT_H__

#include <cassert>
#include <common.h>
#include "01_quicksort.h"
#include <05_probabilistic_analysis_and_randomized_algorithms/easy/01_random_gen.h>

namespace frozenca {

using namespace std;

index_t randomized_partition_impl(vector<int>& A, index_t p, index_t r) {
  assert(p >= 0 && r - p > 1 && r <= ssize(A));
  auto i = random_gen(p, r - 1);
  swap(A[i], A[r - 1]);
  return partition_impl(A, p, r);
}

namespace {

void randomized_quicksort_impl(vector<int>& A, index_t p, index_t r) {
  if (r - p > 1) {
    assert(p >= 0 && r <= ssize(A));
    // partition the subarray around the pivot, which ends up in A[q]
    auto q = randomized_partition_impl(A, p, r);
    randomized_quicksort_impl(A, p, q);                // recursively sort the low side
    randomized_quicksort_impl(A, q + 1, r);            // recursively sort the high side
  }
}

} // anonymous namespace

void randomized_quicksort(vector<int>& A) {
  randomized_quicksort_impl(A, 0, ssize(A));
}

} // namespace frozenca

#endif //__CLRS4_RANDOMIZED_QUICKSORT_H__
