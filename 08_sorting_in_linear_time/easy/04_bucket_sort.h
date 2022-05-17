#ifndef __CLRS4_BUCKET_SORT_H__
#define __CLRS4_BUCKET_SORT_H__

#include <02_getting_started/hard/01_insertion_sort_hard.h>
#include <cmath>
#include <iterator>
#include <common.h>
#include <vector>

namespace frozenca {

using namespace std;

void bucket_sort(vector<float> &A) {
  const auto n = ssize(A);
  vector<vector<float>> B(n + 1);
  for (auto num : A) {
    B[static_cast<int>(floor(n * num))].push_back(num);
  }
  for (auto& b : B) {
    hard::insertion_sort(b);
  }
  A.clear();
  for (auto& b : B) {
    ranges::move(b, back_inserter(A));
  }
}

void bucket_sort2(vector<float> &A) {
  const auto n = ssize(A);
  vector<vector<float>> B(n + 1);
  for (auto num : A) {
    B[static_cast<int>(floor(n * num))].push_back(num);
  }
  for (auto& b : B) {
    ranges::sort(b);
  }
  A.clear();
  for (auto& b : B) {
    ranges::move(b, back_inserter(A));
  }
}

void bucket_sort_two_rv(vector<float> &A) {
  const auto n = ssize(A);
  vector<vector<float>> B(n + 2);
  for (auto num : A) {
    B[static_cast<int>(floor(n * num))].push_back(num);
  }
  for (auto& b : B) {
    hard::insertion_sort(b);
  }
  A.clear();
  for (auto& b : B) {
    ranges::move(b, back_inserter(A));
  }
}

void bucket_sort_2d_points(vector<pair<float, float>> &A) {
  const auto n = ssize(A);
  vector<vector<pair<float, float>>> B(n + 1);
  for (const auto& [x, y] : A) {
    auto r = hypot(x, y);
    B[static_cast<int>(floor(n * r * r))].emplace_back(x, y);
  }
  for (auto& b : B) {
    hard::insertion_sort(b, [](const auto& a, const auto& b) {
        return hypot(a.first, a.second) > hypot(b.first, b.second);
    });
  }
  A.clear();
  for (auto& b : B) {
    ranges::move(b, back_inserter(A));
  }
}

} // namespace frozenca

#endif //__CLRS4_BUCKET_SORT_H__
