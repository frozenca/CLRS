#ifndef __CLRS4_JOSEPHUS_H__
#define __CLRS4_JOSEPHUS_H__

#include <17_augmenting_data_structures/easy/01_order_statistic_tree.h>
#include <cassert>
#include <vector>

namespace frozenca {

using namespace std;

template <index_t m>
requires(m > 0) vector<index_t> josephus(index_t n) {
  assert(n >= m);
  vector<index_t> res;
  vector<index_t> init_set;
  for (index_t i = 1; i <= n; ++i) {
    init_set.push_back(i);
  }
  index_t idx = -1;
  for (index_t i = 1; i <= n; ++i) {
    for (index_t cnt = 0; cnt < m; ++cnt) {
      idx = (idx + 1) % n;
      while (init_set[idx] == 0) {
        idx = (idx + 1) % n;
      }
    }
    res.push_back(init_set[idx]);
    init_set[idx] = 0;
  }

  return res;
}

vector<index_t> josephus(index_t n, index_t m) {
  assert(m > 0 && n >= m);
  vector<index_t> res;
  OrderStatisticTree<index_t> tree;
  for (index_t i = 1; i <= n; i++) {
    tree.insert(i);
  }
  index_t j = 1;
  for (index_t k = n; k >= 1; k--) {
    j = 1 + ((j + m - 2) % k);
    auto key = tree[j - 1];
    res.push_back(key);
    tree.erase(key);
  }
  return res;
}

} // namespace frozenca

#endif //__CLRS4_JOSEPHUS_H__
