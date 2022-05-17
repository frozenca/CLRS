#include <algorithm>
#include <cassert>
#include <common.h>
#include <iostream>
#include <iterator>
#include <numeric>
#include <ranges>
#include <utility>
#include <vector>

namespace fc = frozenca;
using namespace std;

float weighted_median1(const vector<pair<float, float>> &A) {
  auto B = A;
  ranges::sort(B);
  float w = 0.0f;
  fc::index_t i = 0;
  for (; i < ssize(B); i++) {
    w += B[i].second;
    if (w > 0.5f) {
      break;
    }
  }
  return B[i].first;
}

float weighted_median2(const vector<pair<float, float>> &A) {
  if (ssize(A) == 1) {
    return A[0].first;
  } else if (ssize(A) == 2) {
    if (A[0].second >= A[1].second) {
      return A[0].first;
    } else {
      return A[1].first;
    }
  } else {
    auto B = A;
    const auto m = ssize(B);
    ranges::nth_element(B, B.begin() + m / 2);
    auto median = B[m / 2].first;
    auto w_L = accumulate(B.begin(), B.begin() + m / 2, 0.0f,
                          [](auto a, auto &p) { return a + p.second; });
    auto w_R = accumulate(B.begin() + 1 + m / 2, B.end(), 0.0f,
                          [](auto a, auto &p) { return a + p.second; });
    if (w_L < 0.5f && w_R < 0.5f) {
      return median;
    } else if (w_L > 0.5f) {
      B[m / 2].second += w_R;
      vector<pair<float, float>> C(make_move_iterator(B.begin()),
                                   make_move_iterator(B.begin() + 1 + m / 2));
      return weighted_median2(C);
    } else {
      B[m / 2].second += w_L;
      vector<pair<float, float>> C(make_move_iterator(B.begin() + m / 2),
                                   make_move_iterator(B.end()));
      return weighted_median2(C);
    }
  }
}

pair<double, double>
weighted_median_2d(const vector<tuple<float, float, float>> &A) {
  vector<pair<float, float>> X, Y;
  for (const auto &[x, y, w] : A) {
    X.emplace_back(x, w);
    Y.emplace_back(y, w);
  }
  auto x_m = weighted_median2(X);
  auto y_m = weighted_median2(Y);
  return {x_m, y_m};
}

int main() {
  vector<pair<float, float>> vw{
      {1.f, 0.1f}, {3.f, 0.2f}, {5.f, 0.3f}, {7.f, 0.4f}};
  cout << weighted_median1(vw) << '\n';
  cout << weighted_median2(vw) << '\n';

  vector<tuple<float, float, float>> xyw{
      {1.f, 7.f, 0.1f}, {3.f, 5.f, 0.2f}, {5.f, 3.f, 0.3f}, {7.f, 1.f, 0.4f}};
  auto [x_m, y_m] = weighted_median_2d(xyw);
  cout << x_m << ' ' << y_m;
}