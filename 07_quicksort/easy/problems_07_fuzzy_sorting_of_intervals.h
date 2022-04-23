#ifndef __CLRS4_FUZZY_SORT_H__
#define __CLRS4_FUZZY_SORT_H__

#include <cassert>
#include <common.h>
#include <interval.h>
#include <random>

namespace frozenca {

using namespace std;

namespace {

index_t partition_interval(vector<Interval> &A, index_t p, index_t r) {
  assert(p >= 0 && p < r && r <= ssize(A));
  {
    mt19937 gen(random_device{}());
    uniform_int_distribution<index_t> dist(p, r - 1);
    auto j = dist(gen);
    swap(A[j], A[r - 1]);
  }

  // compute intersection interval, and set it as a pivot
  auto b = A[r - 1].first();
  auto e = A[r - 1].second();
  for (auto i = p; i < r - 1; ++i) {
    if (A[i].first() <= e && A[i].second() >= b) {
      b = max(A[i].first(), b);
      e = min(A[i].second(), e);
    }
  }

  index_t i = p;
  for (auto j = p; j < r - 1; ++j) {
    if (A[j].first() < b) {
      swap(A[i], A[j]);
      ++i;
    }
  }
  swap(A[i], A[r - 1]);
  return i;
}

void fuzzy_sort_impl(vector<Interval> &A, index_t p, index_t r) {
  if (r - p > 1) {
    auto t = partition_interval(A, p, r);
    fuzzy_sort_impl(A, p, t);
    fuzzy_sort_impl(A, t, r);
  }
}

} // anonymous namespace

void fuzzy_sort(vector<Interval> &A) { fuzzy_sort_impl(A, 0, ssize(A)); }

} // namespace frozenca

#endif //__CLRS4_FUZZY_SORT_H__
