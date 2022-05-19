#ifndef __CLRS4_BIT_VECTOR_H__
#define __CLRS4_BIT_VECTOR_H__

#include <bitset>
#include <common.h>


namespace frozenca {

using namespace std;

template <size_t M = arr_default_length> class BitVector {
  bitset<M> T = {0};

public:
  bool direct_address_search(index_t k) const { return T[k]; }

  void direct_address_insert(index_t k) { T[k] = true; }

  void direct_address_delete(index_t k) { T[k] = false; }
};

} // namespace frozenca

#endif //__CLRS4_BIT_VECTOR_H__
