#ifndef __CLRS4_COIN_CHANGING_H__
#define __CLRS4_COIN_CHANGING_H__

#include <algorithm>
#include <cassert>
#include <cmath>
#include <common.h>
#include <limits>
#include <tuple>
#include <vector>

namespace frozenca {

using namespace std;

tuple<index_t, index_t, index_t, index_t> coin_change_us(index_t n) {
  assert(n > 0);
  auto q = n / 25;
  n -= q * 25;
  auto d = n / 10;
  n -= d * 10;
  auto k = n / 5;
  n -= k * 5;
  auto p = n;
  return {p, k, d, q};
}

vector<index_t> coin_change_powers(index_t n, index_t c, index_t k) {
  assert(n > 0);
  auto c_k = static_cast<index_t>(pow(c, k));
  vector<index_t> v;
  while (n > 0) {
    auto q = n / c_k;
    n -= q * c_k;
    v.push_back(q);
    c_k /= k;
  }
  ranges::reverse(v);
  return v;
}

pair<index_t, vector<index_t>> coin_change_general(index_t n,
                                                   const vector<index_t> &d) {
  const auto k = ssize(d);
  vector<index_t> c(n + 1);
  vector<index_t> denom(n + 1, -1);
  for (index_t j = 1; j <= n; j++) {
    c[j] = numeric_limits<index_t>::max();
    for (index_t i = 0; i < k; i++) {
      if (j >= d[i] && 1 + c[j - d[i]] < c[j]) {
        c[j] = 1 + c[j - d[i]];
        denom[j] = i;
      }
    }
  }
  vector<index_t> coins(k);
  index_t curr = n;
  while (curr > 0) {
    auto curr_denom = denom[curr];
    coins[curr_denom]++;
    curr -= d[curr_denom];
  }
  return {c[n], move(coins)};
}

} // namespace frozenca

#endif //__CLRS4_COIN_CHANGING_H__
