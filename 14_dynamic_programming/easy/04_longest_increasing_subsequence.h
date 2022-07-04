#ifndef __CLRS4_LONGEST_INCREASING_SUBSEQUENCE_H__
#define __CLRS4_LONGEST_INCREASING_SUBSEQUENCE_H__

#include <algorithm>
#include <cassert>
#include <common.h>
#include <iterator>
#include <ranges>
#include <iostream>
#include <vector>

namespace frozenca {

using namespace std;

vector<int> longest_increasing_subsequence(const vector<int>& X) {
  auto n = ssize(X);
  vector<vector<int>> LIS;
  vector<int> endpoints;
  LIS.emplace_back(vector<int>({X[0]}));
  endpoints.push_back(X[0]);

  for (index_t i = 1; i < n; ++i) {
    auto insertion_point = ranges::lower_bound(endpoints, X[i]);
    if (insertion_point == endpoints.end()) {
      LIS.push_back(LIS.back());
      LIS.back().push_back(X[i]);
      endpoints.push_back(X[i]);
    } else {
      *insertion_point = X[i];
      auto index = distance(endpoints.begin(), insertion_point);
      LIS[index].back() = X[i];
    }
  }
  return LIS.back();
}

} // namespace frozenca

#endif //__CLRS4_LONGEST_INCREASING_SUBSEQUENCE_H__
