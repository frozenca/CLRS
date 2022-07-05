#ifndef __CLRS4_INVEST_STRATEGY_H__
#define __CLRS4_INVEST_STRATEGY_H__

#include <cassert>
#include <common.h>
#include <iostream>
#include <vector>


namespace frozenca {

using namespace std;

pair<float, vector<index_t>>
optimal_invest_strategy(float d, const vector<vector<float>> &r, float f_1,
                        float f_2) {
  auto n = ssize(r);
  assert(n > 0);
  auto yr = ssize(r[0]);
  assert(yr > 0);
  vector<index_t> I(yr + 1);
  vector<float> R(yr + 1);
  for (index_t k = 1; k <= yr; ++k) {
    index_t best_this_year = 0;
    for (index_t i = 1; i < n; i++) {
      if (r[i][k - 1] > r[best_this_year][k - 1]) {
        best_this_year = i;
      }
    }
    auto keep = R[k - 1] + d * r[I[k - 1]][k - 1] - f_1;
    auto change = R[k - 1] + d * r[best_this_year][k - 1] - f_2;
    if (keep > change) {
      R[k] = keep;
      I[k] = I[k - 1];
    } else {
      R[k] = change;
      I[k] = best_this_year;
    }
  }
  return {R[yr], move(I)};
}

void invest(float d, const vector<vector<float>> &r, float f_1, float f_2) {
  auto [gain, strategy] = optimal_invest_strategy(d, r, f_1, f_2);
  cout << "Optimal gain: " << gain << '\n';
  cout << "Invest strategy: " << '\n';
  for (index_t i = 1; i < ssize(strategy); ++i) {
    cout << strategy[i] << ' ';
  }
  cout << '\n';
}

} // namespace frozenca

#endif //__CLRS4_INVEST_STRATEGY_H__
