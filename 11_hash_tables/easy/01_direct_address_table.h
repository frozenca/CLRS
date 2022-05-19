#ifndef __CLRS4_DIRECT_ADDRESS_TABLE_H__
#define __CLRS4_DIRECT_ADDRESS_TABLE_H__

#include <algorithm>
#include <array>
#include <common.h>
#include <utility>

namespace frozenca {

using namespace std;

template <size_t M = arr_default_length> class DirectAddressTable {
  static constexpr index_t NIL = -1;

  array<index_t, M> T;

public:
  index_t direct_address_search(index_t k) const { return T[k]; }

  void direct_address_insert(const pair<index_t, index_t> &x) {
    T[x.first] = x.second;
  }

  void direct_address_delete(index_t k) { T[k] = NIL; }

  index_t maximum() const {
    index_t val = NIL;
    for (auto num : T) {
      if (num != NIL) {
        val = max(num, val);
      }
    }
    return val;
  }
};

} // namespace frozenca

#endif //__CLRS4_DIRECT_ADDRESS_TABLE_H__
