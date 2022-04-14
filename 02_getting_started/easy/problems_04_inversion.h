#ifndef __CLRS4_PROBLEMS_02_04_H__
#define __CLRS4_PROBLEMS_02_04_H__

#include <core/common.h>
#include <vector>

namespace frozenca {

using namespace std;

namespace {

void merge_impl(vector<int> &A, index_t p, index_t q, index_t r, index_t &count) {
  index_t n_l = q - p; // length of A[p : q)
  index_t n_r = r - q; // length of A[q : r)
  vector<int> L(n_l);
  vector<int> R(n_r);
  for (index_t i = 0; i < ssize(L); ++i) { // copy A[p : q) into L
    L[i] = A[p + i];
  }
  for (index_t j = 0; j < ssize(R); ++j) { // copy A[q : r) into R
    R[j] = A[q + j];
  }
  index_t i = 0; // index of the smallest remaining in L
  index_t j = 0; // index of the smallest remaining in R
  index_t k = p; // indexes the location in A to fill
  // As index_t as each of the arrays L and R contains an unmerged element,
  // copy the smallest unmerged element back into A[p : r)
  while (i < n_l && j < n_r) {
    if (L[i] < R[j]) {
      A[k] = L[i];
      ++i;
    } else {
      A[k] = R[j];
      ++j;
      count += n_l - i;
    }
    ++k;
  }
  // Having gone through one of L and R entirely, copy the
  // remainder of the other to the end of A[p : r)
  while (i < n_l) {
    A[k] = L[i];
    ++i;
    ++k;
  }
  while (j < n_r) {
    A[k] = R[j];
    ++j;
    ++k;
  }
}

void count_inversion_impl(vector<int> &A, index_t p, index_t r, index_t &count) {
  if (r - p < 2) { // zero or one element?
    return;
  }
  index_t q = p + (r - p) / 2;             // midpoint of A[p : r)
  count_inversion_impl(A, p, q, count); // recursively sort A[p : q)
  count_inversion_impl(A, q, r, count); // recursively sort A[q : r)
  // merge A[p : q) and A[q : r) into A[p : r).
  merge_impl(A, p, q, r, count);
}

} // anonymous namespace

index_t count_inversion(vector<int> &A) {
  index_t count = 0;
  count_inversion_impl(A, 0, ssize(A), count);
  return count;
}

} // namespace frozenca

#endif //__CLRS4_PROBLEMS_02_04_H__
