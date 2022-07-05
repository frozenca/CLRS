#ifndef __CLRS4_LONGEST_COMMON_SUBSEQUENCE_H__
#define __CLRS4_LONGEST_COMMON_SUBSEQUENCE_H__

#include <cassert>
#include <common.h>
#include <iostream>
#include <vector>

namespace frozenca {

using namespace std;

enum class DirectionLCS {
  None,
  Up,
  Left,
  Upleft,
};

pair<vector<vector<index_t>>, vector<vector<DirectionLCS>>>
lcs_length(const vector<char> &X, const vector<char> &Y) {
  auto m = ssize(X);
  auto n = ssize(Y);
  assert(m > 0 && n > 0);
  vector<vector<index_t>> c(m + 1, vector<index_t>(n + 1));
  vector<vector<DirectionLCS>> b(m, vector<DirectionLCS>(n, DirectionLCS::None));
  // compute table entries in row-major order
  for (index_t i = 1; i <= m; ++i) {
    for (index_t j = 1; j <= n; ++j) {
      if (X[i - 1] == Y[j - 1]) {
        c[i][j] = c[i - 1][j - 1] + 1;
        b[i - 1][j - 1] = DirectionLCS::Upleft;
      } else if (c[i - 1][j] >= c[i][j - 1]) {
        c[i][j] = c[i - 1][j];
        b[i - 1][j - 1] = DirectionLCS::Up;
      } else {
        c[i][j] = c[i][j - 1];
        b[i - 1][j - 1] = DirectionLCS::Left;
      }
    }
  }
  return {c, b};
}

void print_lcs_impl(const vector<vector<DirectionLCS>> &b, const vector<char> &X,
                    index_t i, index_t j) {
  if (i == 0 || j == 0) {
    return; // the LCS has length 0
  }
  if (b[i - 1][j - 1] == DirectionLCS::Upleft) {
    print_lcs_impl(b, X, i - 1, j - 1);
    cout << X[i - 1];
  } else if (b[i - 1][j - 1] == DirectionLCS::Up) {
    print_lcs_impl(b, X, i - 1, j);
  } else {
    print_lcs_impl(b, X, i, j - 1);
  }
}

void print_lcs(const vector<char> &X, const vector<char> &Y) {
  auto m = ssize(X);
  auto n = ssize(Y);
  assert(m > 0 && n > 0);
  auto [c, b] = lcs_length(X, Y);
  print_lcs_impl(b, X, m, n);
}

void print_lcs_impl2(const vector<vector<index_t>> &c, const vector<char> &X,
                     const vector<char> &Y, index_t i, index_t j) {
  if (c[i][j] == 0) {
    return;
  }
  if (X[i - 1] == Y[j - 1]) {
    print_lcs_impl2(c, X, Y, i - 1, j - 1);
    cout << X[i - 1];
  } else if (c[i - 1][j] > c[i][j - 1]) {
    print_lcs_impl2(c, X, Y, i - 1, j);
  } else {
    print_lcs_impl2(c, X, Y, i, j - 1);
  }
}

void print_lcs2(const vector<char> &X, const vector<char> &Y) {
  auto m = ssize(X);
  auto n = ssize(Y);
  assert(m > 0 && n > 0);
  auto [c, b] = lcs_length(X, Y);
  print_lcs_impl2(c, X, Y, m, n);
}

index_t lcs_length_memoized_impl(const vector<char> &X, const vector<char> &Y,
                                 vector<vector<index_t>> &c, index_t i,
                                 index_t j) {
  index_t &q = c[i][j];
  if (q != numeric_limits<index_t>::max()) {
    return q;
  }
  if (i == 0 || j == 0) {
    q = 0;
  } else {
    if (X[i - 1] == Y[j - 1]) {
      q = lcs_length_memoized_impl(X, Y, c, i - 1, j - 1) + 1;
    } else {
      q = max(lcs_length_memoized_impl(X, Y, c, i - 1, j),
              lcs_length_memoized_impl(X, Y, c, i, j - 1));
    }
  }
  return q;
}

index_t lcs_length_memoized(const vector<char> &X, const vector<char> &Y) {
  auto m = ssize(X);
  auto n = ssize(Y);
  assert(m > 0 && n > 0);
  vector<vector<index_t>> c(
      m + 1, vector<index_t>(n + 1, numeric_limits<index_t>::max()));
  return lcs_length_memoized_impl(X, Y, c, m, n);
}

index_t lcs_length_efficient(const vector<char> &X, const vector<char> &Y) {
  auto m = ssize(X);
  auto n = ssize(Y);
  assert(m > 0 && n > 0);
  vector<index_t> c(n + 1);
  // compute table entries in row-major order
  for (index_t i = 1; i <= m; ++i) {
    for (index_t j = 1; j <= n; ++j) {
      if (X[i - 1] == Y[j - 1]) {
        c[j] = c[j - 1] + 1;
      } else if (c[j] < c[j - 1]) {
        c[j] = c[j - 1];
      }
    }
  }
  return c[n];
}

} // namespace frozenca

#endif //__CLRS4_LONGEST_COMMON_SUBSEQUENCE_H__
