#ifndef __CLRS4_KTH_QUANTILES_H__
#define __CLRS4_KTH_QUANTILES_H__

#include <09_medians_and_order_statistics/easy/02_selection_in_expected_linear_time.h>
#include <cassert>
#include <common.h>

namespace frozenca {

using namespace std;

namespace {

void kth_quantiles_helper(vector<int> &A, index_t p, index_t r,
                          vector<index_t> &pos, index_t b, index_t e,
                          vector<int> &quantiles) {
  if (b >= e) {
    return;
  }
  auto mid = b + (e - b) / 2;
  auto [q, val] = randomized_select_iterative_pair(A, p, r, pos[mid]);
  quantiles[mid] = val;
  kth_quantiles_helper(A, p, q, pos, b, mid, quantiles);
  auto offset = q + 1 - p;
  for (index_t i = mid + 1; i < e; ++i) {
    pos[i] -= offset;
  }
  kth_quantiles_helper(A, q + 1, r, pos, mid + 1, e, quantiles);
}

} // anonymous namespace

vector<int> kth_quantiles(vector<int> &A, index_t k) {
  assert(k > 0);
  const auto n = ssize(A);
  vector<index_t> pos;
  for (index_t i = 0; i < k; ++i) {
    pos.push_back(i * n / k);
  }
  vector<int> quantiles(k);
  kth_quantiles_helper(A, 0, n, pos, 0, k, quantiles);
  return quantiles;
}

} // namespace frozenca

#endif //__CLRS4_KTH_QUANTILES_H__
