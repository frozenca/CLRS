#ifndef __CLRS4_KNAPSACK_H__
#define __CLRS4_KNAPSACK_H__

#include <algorithm>
#include <cassert>
#include <cmath>
#include <common.h>
#include <ranges>
#include <vector>

namespace frozenca {

using namespace std;

pair<vector<index_t>, float>
knapsack_0_1(const vector<pair<index_t, float>> &items, index_t W) {
  const auto n = ssize(items);
  if (n == 0) {
    return {{}, 0.0f};
  }
  assert(W > 0);
  vector<vector<pair<float, index_t>>> K(
      n + 1, vector<pair<float, index_t>>(W + 1, {0.0f, -1}));
  for (index_t i = 1; i <= n; ++i) {
    for (index_t j = 1; j <= W; ++j) {
      auto diff = j - items[i - 1].first;
      if (diff < 0) {
        K[i][j] = {K[i - 1][j].first, -1};
      } else {
        auto curr = K[i - 1][diff].first + items[i - 1].second;
        if (curr > K[i - 1][j].first) {
          K[i][j] = {curr, i - 1};
        } else {
          K[i][j] = {K[i - 1][j].first, -1};
        }
      }
    }
  }
  float result_value = K[n][W].first;
  vector<index_t> indices;
  auto curr_i = n;
  auto curr_j = W;
  while (curr_i >= 0) {
    auto next_idx = K[curr_i][curr_j].second;
    curr_i--;
    if (next_idx != -1) {
      indices.push_back(next_idx);
    }
  }
  ranges::reverse(indices);
  return {move(indices), result_value};
}

pair<vector<index_t>, float>
knapsack_0_1_sorted(const vector<pair<index_t, float>> &items, index_t W) {
  const auto n = ssize(items);
  if (n == 0) {
    return {{}, 0.0f};
  }
  assert(W > 0);
  vector<index_t> indices;
  float result_value = 0.0f;
  for (index_t i = 0; i < n; ++i) {
    W -= items[i].first;
    if (W < 0) {
      break;
    }
    result_value += items[i].second;
    indices.push_back(i);
  }
  return {move(indices), result_value};
}

pair<vector<float>, float>
knapsack_fractional(vector<pair<float, float>> &items, float W) {
  assert(W > 0.0f);
  assert(ranges::all_of(
      items, [](const auto &p) { return p.first > 0.0f && p.second > 0.0f; }));
  ranges::sort(items, [](const auto &p1, const auto &p2) {
    return p1.first / p2.second > p2.first / p2.second;
  });
  float curr_weight = 0.0f;
  float total_value = 0.0f;
  vector<float> ratios;
  for (const auto &[value, weight] : items) {
    if (curr_weight + weight <= W) {
      curr_weight += weight;
      total_value += value;
      ratios.push_back(1.0f);
    } else {
      auto remain = W - weight;
      total_value += remain * value / weight;
      ratios.push_back(remain / weight);
    }
  }
  return {move(ratios), total_value};
}

} // namespace frozenca

#endif //__CLRS4_KNAPSACK_H__
