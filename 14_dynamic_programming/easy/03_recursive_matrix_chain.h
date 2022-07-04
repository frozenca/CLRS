#ifndef __CLRS4_RECURSIVE_MATRIX_CHAIN_H__
#define __CLRS4_RECURSIVE_MATRIX_CHAIN_H__

#include <cassert>
#include <common.h>
#include <iostream>
#include <limits>
#include <vector>

namespace frozenca {

using namespace std;

index_t lookup_chain(const vector<index_t>& p, vector<vector<index_t>>& m, index_t i, index_t j) {
  index_t& q = m[i][j];
  if (q != numeric_limits<index_t>::max()) {
    return q;
  }
  if (i == j) {
    q = 0;
  } else {
    for (index_t k = i; k < j; ++k) {
      auto val = lookup_chain(p, m, i, k) + lookup_chain(p, m, k + 1, j) + p[i] * p[k + 1] * p[j + 1];
      if (val < q) {
        q = val;
      }
    }
  }
  return q;
}

index_t matrix_chain_memoized(const vector<index_t>& p) {
  auto n = ssize(p) - 1;
  assert(n > 1);
  vector<vector<index_t>> m(n, vector<index_t>(n, numeric_limits<index_t>::max()));
  return lookup_chain(p, m, 0, n - 1);
}

} // namespace frozenca

#endif //__CLRS4_RECURSIVE_MATRIX_CHAIN_H__
