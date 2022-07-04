#ifndef __CLRS4_OPTIMAL_BINARY_SEARCH_TREES_H__
#define __CLRS4_OPTIMAL_BINARY_SEARCH_TREES_H__

#include <cassert>
#include <common.h>
#include <iostream>
#include <limits>
#include <vector>

namespace frozenca {

using namespace std;

// O(n^2) (refer exercise 14.5-4)
pair<vector<vector<float>>, vector<vector<index_t>>>
optimal_bst(const vector<float> &p, const vector<float> &q) {
  auto n = ssize(p);
  assert(n > 0 && ssize(q) == n + 1);
  // e[i][j] expected cost for keys in range [i : j)
  vector<vector<float>> e(n + 1, vector<float>(n + 1));
  // w[i][j] sum of probs for keys in range [i : j)
  vector<vector<float>> w(n + 1, vector<float>(n + 1));
  // root for keys in range [i : j]
  vector<vector<index_t>> root(n, vector<index_t>(n));
  for (index_t i = 0; i <= n; ++i) { // base cases: 14.14
    e[i][i] = q[i];
    w[i][i] = q[i];
  }
  for (index_t i = 0; i <= n - 1; ++i) {
    w[i][i + 1] = p[i] + q[i] + q[i + 1];
    e[i][i + 1] = q[i] + q[i + 1] + w[i][i + 1];
    root[i][i] = i;
  }

  for (index_t l = 2; l <= n; ++l) {
    for (index_t i = 0; i <= n - l; ++i) {
      index_t j = i + l - 1;
      e[i][j + 1] = numeric_limits<float>::max();
      w[i][j + 1] = w[i][j] + p[j] + q[j + 1];
      for (index_t r = root[i][j - 1]; r <= root[i + 1][j]; ++r) {
        float v = e[i][r] + e[r + 1][j + 1] + w[i][j + 1];
        if (v < e[i][j + 1]) {
          e[i][j + 1] = v;
          root[i][j] = r;
        }
      }
    }
  }
  return {e, root};
}

void construct_optimal_bst_impl(const vector<vector<index_t>> &root, index_t i,
                                index_t j, index_t last, index_t &d_index) {
  if (i > j) {
    if (j < last) {
      cout << "d_" << d_index++ << " is the left child of k_" << last << "\n";
    } else {
      cout << "d_" << d_index++ << " is the right child of k_" << last << "\n";
    }
    return;
  }
  if (last == 0) {
    cout << "k_" << root[i][j] << " is the root\n";
  } else if (j < last) {
    cout << "k_" << root[i][j] << " is the left child of k_" << last << "\n";
  } else {
    cout << "k_" << root[i][j] << " is the right child of k_" << last << "\n";
  }
  construct_optimal_bst_impl(root, i, root[i][j] - 1, root[i][j], d_index);
  construct_optimal_bst_impl(root, root[i][j] + 1, j, root[i][j], d_index);
}

// exercise 14.5-1
void construct_optimal_bst(const vector<float> &p, const vector<float> &q) {
  auto n = ssize(p);
  assert(n > 0 && ssize(q) == n + 1);
  auto [e, root] = optimal_bst(p, q);
  index_t d_index = 0;
  construct_optimal_bst_impl(root, 0, n - 1, 0, d_index);
}

} // namespace frozenca

#endif //__CLRS4_OPTIMAL_BINARY_SEARCH_TREES_H__
