#ifndef __CLRS4_SELECT_WITH_GROUPS_OF_3_H__
#define __CLRS4_SELECT_WITH_GROUPS_OF_3_H__

#include <09_medians_and_order_statistics/easy/03_selection_in_worst-case_linear_time.h>
#include <algorithm>
#include <common.h>
#include <limits>
#include <utility>
#include <vector>

namespace frozenca {

using namespace std;

namespace {

void three_sort(vector<int> &A, index_t j, index_t g) {
  // sort <A[j], A[j + g], A[j + 2g]> in place
  index_t i = 1;
  while (i < 3) {
    index_t k = j + i * g;
    while (k > j && A[k - g] > A[k]) {
      swap(A[k - g], A[k]);
      k -= g;
    }
    ++i;
  }
}

} // anonymous namespace

int select3_impl(vector<int> &A, index_t p, index_t r, index_t i) {
  while ((r - p) % 9 != 0) {
    for (index_t j = p + 1; j < r; ++j) { // put the minimum into A[p]
      if (A[p] > A[j]) {
        swap(A[p], A[j]);
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
  auto g = (r - p) / 3;                 // number of 3-element groups
  for (index_t j = p; j < p + g; ++j) { // run through the groups
    three_sort(A, j, g);
  }
  auto g_ = g / 3; // number of 3-element subgroups
  for (index_t j = p + g; j < p + g + g_; ++j) { // sort the subgroups
    three_sort(A, j, g_);
  }
  // all group medians now lie in the middle ninth of A[p : r)
  // find the pivot x recursively as the median of the group medians.
  auto x = select3_impl(A, p + 4 * g_, p + 5 * g_, g_ / 2);
  auto q = partition_around(A, p, r, x); // partition around the pivot
  // the rest is just like lines 3-9 of randomized_select
  auto k = q - p;
  if (i == k) {
    return A[q]; // the pivot value is the answer.
  } else if (i < k) {
    return select3_impl(A, p, q, i);
  } else {
    return select3_impl(A, q + 1, r, i - 1 - k);
  }
}

int select3(vector<int> &A, index_t i) {
  return select3_impl(A, 0, ssize(A), i);
}

} // namespace frozenca

#endif //__CLRS4_SELECT_WITH_GROUPS_OF_3_H__
