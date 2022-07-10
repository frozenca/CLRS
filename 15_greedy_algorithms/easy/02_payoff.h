#ifndef __CLRS4_REORDER_PAYOFF_H__
#define __CLRS4_REORDER_PAYOFF_H__

#include <algorithm>
#include <cassert>
#include <cmath>
#include <common.h>
#include <ranges>
#include <vector>

namespace frozenca {

using namespace std;

int64_t maximize_payoff(vector<index_t> &A, vector<index_t> &B) {
  assert(ranges::all_of(A, [](auto n) { return n > 0; }));
  assert(ranges::all_of(B, [](auto n) { return n > 0; }));
  assert(ssize(A) == ssize(B));
  ranges::sort(A);
  ranges::sort(B);
  const auto n = ssize(A);
  int64_t prod = 1;
  for (index_t i = 0; i < n; ++i) {
    prod *= static_cast<int64_t>(pow(A[i], B[i]));
  }
  return prod;
}

} // namespace frozenca

#endif //__CLRS4_REORDER_PAYOFF_H__
