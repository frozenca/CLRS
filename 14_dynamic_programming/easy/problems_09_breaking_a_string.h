#ifndef __CLRS4_BREAK_STRING_H__
#define __CLRS4_BREAK_STRING_H__

#include <algorithm>
#include <cassert>
#include <common.h>
#include <limits>
#include <vector>


namespace frozenca {

using namespace std;

index_t break_string_lowest_cost(index_t n, vector<index_t> &B) {
  const auto m = ssize(B);
  {
    vector<index_t> B_(m + 2);
    B_[m + 1] = n;
    for (index_t i = 1; i <= m; i++) {
      B_[i] = B[i - 1];
    }
    swap(B, B_);
  }
  vector<vector<index_t>> C(m + 1, vector<index_t>(m + 2));
  for (index_t i = m - 1; i >= 0; --i) {
    for (index_t j = i + 2; j <= m + 1; ++j) {
      C[i][j] = numeric_limits<index_t>::max();
      for (index_t k = i + 1; k <= j - 1; k++) {
        C[i][j] = min({C[i][j], C[i][k] + C[k][j] + B[j] - B[i]});
      }
    }
  }
  return C[0][m + 1];
}

} // namespace frozenca

#endif //__CLRS4_BREAK_STRING_H__
