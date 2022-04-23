#ifndef __CLRS4_STACK_DEPTH_QUICKSORT_H__
#define __CLRS4_STACK_DEPTH_QUICKSORT_H__

#include "01_quicksort.h"
#include <cassert>
#include <common.h>

namespace frozenca {

using namespace std;

namespace {

void quicksort_tre_impl(vector<int> &A, index_t p, index_t r) {
  assert(p >= 0 && r <= ssize(A));
  while (r - p > 1) {
    auto q = partition_impl(A, p, r);
    auto m = p + (r - p) / 2;
    if (q < m) {
      quicksort_tre_impl(A, p, q);
      p = q + 1;
    } else {
      quicksort_tre_impl(A, q, r);
      r = q;
    }
  }
}

} // anonymous namespace

void quicksort_tre(vector<int> &A) { quicksort_tre_impl(A, 0, ssize(A)); }

} // namespace frozenca

#endif //__CLRS4_STACK_DEPTH_QUICKSORT_H__
