#ifndef __CLRS4_DIRECT_ADDRESS_BIG_TABLE_H__
#define __CLRS4_DIRECT_ADDRESS_BIG_TABLE_H__

#include <algorithm>
#include <array>
#include <common.h>
#include <utility>
#include <vector>

namespace frozenca {

using namespace std;

template <size_t M = arr_default_length> class DirectAddressBigTable {
  static constexpr index_t NIL = -1;

  array<index_t, M> T;
  vector<index_t> S;
  vector<index_t> S_ptr;

public:
  index_t direct_address_search(index_t k) const {
    if (!S.empty() && 0 <= T[k] && T[k] <= S.back() && S[T[k]] == k) {
      return S_ptr[T[k]];
    } else {
      return NIL;
    }
  }

  void direct_address_insert(const pair<index_t, index_t> &x) {
    T[x.first] = ssize(S);
    S.push_back(x.first);
    S_ptr.push_back(x.second);
  }

  void direct_address_delete(index_t k) {
    S[T[k]] = S.back();
    S_ptr[T[k]] = S_ptr.back();
    T[S[T[k]]] = T[k];
    T[k] = 0;
    S.pop_back();
    S_ptr.pop_back();
  }
};

} // namespace frozenca

#endif //__CLRS4_DIRECT_ADDRESS_BIG_TABLE_H__
