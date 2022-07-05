#ifndef __CLRS4_BITONIC_EUCLIDEAN_TSP_H__
#define __CLRS4_BITONIC_EUCLIDEAN_TSP_H__

#include <algorithm>
#include <cassert>
#include <common.h>
#include <limits>
#include <point.h>
#include <vector>

namespace frozenca {

using namespace std;

pair<vector<vector<float>>, vector<vector<index_t>>>
bitonic_euclidean_tsp_impl(vector<Point2d> &p) {
  ranges::sort(p, [](const auto& p1, const auto& p2){
    return p1 < p2;
  });
  const auto n = ssize(p);
  assert(n >= 2);
  vector<vector<float>> b(n, vector<float>(n));
  vector<vector<index_t>> r(n, vector<index_t>(n, -1));
  b[0][1] = dist(p[0], p[1]);
  r[0][1] = 0;
  for (index_t j = 2; j < n; j++) {
    for (index_t i = 0; i < j - 1; i++) {
      b[i][j] = b[i][j - 1] + dist(p[j - 1], p[j]);
      r[i][j] = j - 1;
    }
    b[j - 1][j] = numeric_limits<float>::max();
    for (index_t k = 0; k < j - 1; k++) {
      auto q = b[k][j - 1] + dist(p[k], p[j]);
      if (q < b[j - 1][j]) {
        b[j - 1][j] = q;
        r[j - 1][j] = k;
      }
    }
  }
  b[n - 1][n - 1] = b[n - 2][n - 1] + dist(p[n - 2], p[n - 1]);
  r[n - 1][n - 1] = n - 2;
  return {move(b), move(r)};
}

void bitonic_euclidean_tsp(vector<Point2d> &p) {
  index_t n = ssize(p);
  assert(n >= 2);
  auto [b, r] = bitonic_euclidean_tsp_impl(p);
  cout << "Shorted tour length: " << b.back().back() << '\n';

  vector<index_t> path1;
  vector<index_t> path2;

  {
    index_t i = n - 2;
    index_t j = n - 1;
    path1.push_back(n - 2);
    path2.push_back(n - 1);
    while (i > 0 && j > 0) {
      auto next = r[i][j];
      if (path2.back() == j) {
        path2.push_back(next);
      } else if (path1.back() == j) {
        path1.push_back(next);
      } else {
        assert(false);
      }
      j = i;
      i = next;
      if (i > j) {
        swap(i, j);
      }
    }
    if (path1.back() != 0) {
      path1.push_back(0);
    }
    if (path2.back() != 0) {
      path2.push_back(0);
    }
  }
  ranges::reverse(path1);
  ranges::move(path2, back_inserter(path1));
  cout << "Shorted tour path:\n";
  for (index_t i = 0; i < ssize(path1) - 1; ++i) {
    cout << p[path1[i]] << "->";
  }
  cout << p[path1[ssize(path1) - 1]] << '\n';
}

} // namespace frozenca

#endif //__CLRS4_BITONIC_EUCLIDEAN_TSP_H__
