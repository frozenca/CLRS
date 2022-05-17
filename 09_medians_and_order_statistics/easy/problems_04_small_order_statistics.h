#ifndef __CLRS4_SMALL_ORDER_STATISTICS_H__
#define __CLRS4_SMALL_ORDER_STATISTICS_H__

#include <07_quicksort/easy/03_randomized_quicksort.h>
#include <algorithm>
#include <common.h>
#include <limits>
#include <vector>

namespace frozenca {

using namespace std;

// carrying permuation version.

void five_sort_with_perm(vector<int> &A, index_t j, index_t g,
                         vector<index_t> &perm, index_t perm_offset) {
  // sort <A[j], A[j + g], A[j + 2g], A[j + 3g], A[j + 4g]> in place
  index_t i = 1;
  while (i < 5) {
    index_t k = j + i * g;
    while (k > j && A[k - g] > A[k]) {
      swap(A[k - g], A[k]);
      swap(perm[k - g - perm_offset], perm[k - perm_offset]);
      k -= g;
    }
    ++i;
  }
}

index_t partition_around_with_perm(vector<int> &A, index_t p, index_t r,
                                   int pivot, vector<index_t> &perm,
                                   index_t perm_offset) {
  index_t l = p;
  index_t m = p;
  index_t h = r;
  while (m < h) {
    if (A[m] < pivot) {
      swap(A[l], A[m]);
      swap(perm[l - perm_offset], perm[m - perm_offset]);
      ++l;
      ++m;
    } else if (A[m] > pivot) {
      --h;
      swap(A[m], A[h]);
      swap(perm[m - perm_offset], perm[h - perm_offset]);
    } else {
      ++m;
    }
  }
  return l;
}

int select_with_perm(vector<int> &A, index_t p, index_t r, index_t i,
                     vector<index_t> &perm, index_t perm_offset) {
  while ((r - p) % 5 != 0) {
    for (index_t j = p + 1; j < r; ++j) { // put the minimum into A[p]
      if (A[p] > A[j]) {
        swap(A[p], A[j]);
        swap(perm[p - perm_offset], perm[j - perm_offset]);
      }
    }
    // if we want the minimum of A[p : r), we're done.
    if (i == 0) {
      return A[p];
    }
    // otherwise, we want the (i - 1)st element of A[p + 1 : r).
    ++p;
    --i;
  }
  auto g = (r - p) / 5;                 // number of 5-element groups
  for (index_t j = p; j < p + g; ++j) { // sort each group
    five_sort_with_perm(A, j, g, perm, perm_offset);
  }
  // all group medians now lie in the middle fifth of A[p : r)
  // find the pivot x recursively as the median of the group medians.
  auto x = select_with_perm(A, p + 2 * g, p + 3 * g, g / 2, perm, perm_offset);
  auto q = partition_around_with_perm(
      A, p, r, x, perm, perm_offset); // partition around the pivot
  // the rest is just like lines 3-9 of randomized_select
  auto k = q - p;
  if (i == k) {
    return A[q]; // the pivot value is the answer.
  } else if (i < k) {
    return select_with_perm(A, p, q, i, perm, perm_offset);
  } else {
    return select_with_perm(A, q + 1, r, i - 1 - k, perm, perm_offset);
  }
}

} // namespace frozenca

#endif //__CLRS4_SMALL_ORDER_STATISTICS_H__
