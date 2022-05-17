#ifndef __CLRS4_SELECTION_IN_WORST_CASE_LINEAR_TIME_H__
#define __CLRS4_SELECTION_IN_WORST_CASE_LINEAR_TIME_H__

#include <algorithm>
#include <common.h>
#include <limits>
#include <utility>
#include <vector>

namespace frozenca {

using namespace std;

index_t partition_around(vector<int> &A, index_t p, index_t r, int pivot) {
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

namespace {

void five_sort(vector<int> &A, index_t j, index_t g) {
  // sort <A[j], A[j + g], A[j + 2g], A[j + 3g], A[j + 4g]> in place
  index_t i = 1;
  while (i < 5) {
    index_t k = j + i * g;
    while (k > j && A[k - g] > A[k]) {
      swap(A[k - g], A[k]);
      k -= g;
    }
    ++i;
  }
}

} // anonymous namespace

int select_impl(vector<int> &A, index_t p, index_t r, index_t i) {
  while ((r - p) % 5 != 0) {
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
  auto g = (r - p) / 5;                 // number of 5-element groups
  for (index_t j = p; j < p + g; ++j) { // sort each group
    five_sort(A, j, g);
  }
  // all group medians now lie in the middle fifth of A[p : r)
  // find the pivot x recursively as the median of the group medians.
  auto x = select_impl(A, p + 2 * g, p + 3 * g, g / 2);
  auto q = partition_around(A, p, r, x); // partition around the pivot
  // the rest is just like lines 3-9 of randomized_select
  auto k = q - p;
  if (i == k) {
    return A[q]; // the pivot value is the answer.
  } else if (i < k) {
    return select_impl(A, p, q, i);
  } else {
    return select_impl(A, q + 1, r, i - 1 - k);
  }
}

int select(vector<int> &A, index_t i) { return select_impl(A, 0, ssize(A), i); }

// no preprocessing

int select_simple_impl(vector<int> &A, index_t p, index_t r, index_t i) {
  if (r - p < 5) {
    nth_element(A.begin() + p, A.begin() + p + i, A.begin() + r);
    return A[p + i];
  }
  auto g = (r - p) / 5;                 // number of 5-element groups
  for (index_t j = p; j < p + g; ++j) { // sort each group
    five_sort(A, j, g);
  }
  // all group medians now lie in the middle fifth of A[p : r)
  // find the pivot x recursively as the median of the group medians.
  auto x = select_simple_impl(A, p + 2 * g, p + 3 * g, g / 2);
  auto q = partition_around(A, p, r, x); // partition around the pivot
  // the rest is just like lines 3-9 of randomized_select
  auto k = q - p;
  if (i == k) {
    return A[q]; // the pivot value is the answer.
  } else if (i < k) {
    return select_simple_impl(A, p, q, i);
  } else {
    return select_simple_impl(A, q + 1, r, i - 1 - k);
  }
}

int select_simple(vector<int> &A, index_t i) {
  return select_simple_impl(A, 0, ssize(A), i);
}

// group of 7

void seven_sort(vector<int> &A, index_t j, index_t g) {
  // sort <A[j], A[j + g], A[j + 2g], A[j + 3g], A[j + 4g], A[j + 5g], A[j +
  // 6g]> in place
  index_t i = 1;
  while (i < 7) {
    index_t k = j + i * g;
    while (k > j && A[k - g] > A[k]) {
      swap(A[k - g], A[k]);
      k -= g;
    }
    ++i;
  }
}

int select7_impl(vector<int> &A, index_t p, index_t r, index_t i) {
  while ((r - p) % 7 != 0) {
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
  auto g = (r - p) / 7;                 // number of 5-element groups
  for (index_t j = p; j < p + g; ++j) { // sort each group
    seven_sort(A, j, g);
  }
  // all group medians now lie in the middle fifth of A[p : r)
  // find the pivot x recursively as the median of the group medians.
  auto x = select7_impl(A, p + 3 * g, p + 4 * g, g / 2);
  auto q = partition_around(A, p, r, x); // partition around the pivot
  // the rest is just like lines 3-9 of randomized_select
  auto k = q - p;
  if (i == k) {
    return A[q]; // the pivot value is the answer.
  } else if (i < k) {
    return select7_impl(A, p, q, i);
  } else {
    return select7_impl(A, q + 1, r, i - 1 - k);
  }
}

int select7(vector<int> &A, index_t i) {
  return select7_impl(A, 0, ssize(A), i);
}

} // namespace frozenca

#endif //__CLRS4_SELECTION_IN_WORST_CASE_LINEAR_TIME_H__
