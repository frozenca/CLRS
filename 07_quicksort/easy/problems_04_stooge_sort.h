#ifndef __CLRS4_STOOGE_SORT_H__
#define __CLRS4_STOOGE_SORT_H__

#include <cassert>
#include <common.h>

namespace frozenca {

using namespace std;

namespace {

void stooge_sort_impl(vector<int> &A, index_t p, index_t r) {
  assert(p >= 0 && r <= ssize(A));
  if (A[p] > A[r - 1]) {
    swap(A[p], A[r - 1]);
  }
  if (r - p > 2) {
    auto k = (r - p + 1) / 3;
    stooge_sort_impl(A, p, r - k);
    stooge_sort_impl(A, p + k, r);
    stooge_sort_impl(A, p, r - k);
  }
}

} // anonymous namespace

void stooge_sort(vector<int> &A) { stooge_sort_impl(A, 0, ssize(A)); }

} // namespace frozenca

#endif //__CLRS4_STOOGE_SORT_H__
