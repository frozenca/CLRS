#ifndef __CLRS4_MATRIX_CHAIN_MULTIPLICATION_H__
#define __CLRS4_MATRIX_CHAIN_MULTIPLICATION_H__

#include <cassert>
#include <common.h>
#include <iostream>
#include <limits>
#include <matrix.h>
#include <vector>
#include <stdexcept>

namespace frozenca {

using namespace std;

pair<vector<vector<index_t>>, vector<vector<index_t>>>
matrix_chain_order(const vector<index_t> &p) {
  auto n = ssize(p) - 1;
  assert(n > 1);
  vector<vector<index_t>> m(n, vector<index_t>(n));
  // zero-initialize for chain length 1
  vector<vector<index_t>> s(n - 1, vector<index_t>(n - 1));
  for (index_t l = 2; l <= n; ++l) {       // l is the chain length
    for (index_t i = 0; i <= n - l; ++i) { // chain begins at A_i
      index_t j = i + l - 1;               // chain ends at A_j
      index_t &q = m[i][j];
      q = numeric_limits<index_t>::max();
      for (index_t k = i; k <= j - 1; ++k) { // try A(i : k) A(k + 1 : j)
        auto val = m[i][k] + m[k + 1][j] + p[i] * p[k + 1] * p[j + 1];
        if (val < q) {
          q = val;         // remember this cost
          s[i][j - 1] = k; // remember this index
        }
      }
    }
  }
  return {move(m), move(s)};
}

void print_optimal_parens_impl(const vector<vector<index_t>> &s, index_t i,
                               index_t j) {
  if (i == j) {
    cout << "A_" << i;
  } else {
    assert(i < j);
    cout << '(';
    print_optimal_parens_impl(s, i, s[i][j - 1]);
    print_optimal_parens_impl(s, s[i][j - 1] + 1, j);
    cout << ')';
  }
}

void print_optimal_parens(const vector<index_t> &p) {
  auto [m, s] = matrix_chain_order(p);
  auto n = ssize(p) - 1;
  assert(n > 1);
  print_optimal_parens_impl(s, 0, n - 1);
}

template <semiregular T>
Matrix<T> matrix_chain_multiply_impl(const vector<Matrix<T>>& A, const vector<vector<index_t>>& s, index_t i, index_t j) {
  if (i == j) {
    return A[i];
  } else {
    assert(i < j);
    auto M1 = matrix_chain_multiply_impl(A, s, i, s[i][j - 1]);
    auto M2 = matrix_chain_multiply_impl(A, s, s[i][j - 1] + 1, j);
    return M1 * M2;
  }
}

template <semiregular T>
Matrix<T> matrix_chain_multiply(const vector<Matrix<T>>& A) {
  auto n = ssize(A);
  assert(n > 0);
  vector<index_t> p(n + 1);
  p[0] = A[0].rows();
  for (index_t i = 0; i < n - 1; ++i) {
    if (A[i].cols() != A[i + 1].rows()) {
      throw invalid_argument("dimensions don't match\n");
    }
    p[i] = A[i].cols();
  }
  auto [m, s] = matrix_chain_order(p);
  return matrix_chain_multiply_impl(A, s, 0, n - 1);
}

} // namespace frozenca

#endif //__CLRS4_MATRIX_CHAIN_MULTIPLICATION_H__
