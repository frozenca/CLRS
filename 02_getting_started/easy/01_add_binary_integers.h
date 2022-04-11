#ifndef __CLRS4_ADD_BINARY_INTEGERS_H__
#define __CLRS4_ADD_BINARY_INTEGERS_H__

#include <bitset>

namespace frozenca {

using namespace std;

template <size_t N>
bitset<N + 1> add_binary_integers(const bitset<N> &A, const bitset<N> &B) {
  bitset<N + 1> C;
  int carry = 0;
  for (size_t j = 0; j < N; j++) {
    auto temp = A[j] + B[j] + carry;
    C[j] = temp % 2;
    carry = temp / 2;
  }
  C[N] = carry;
  return C;
}

} // namespace frozenca

#endif //__CLRS4_ADD_BINARY_INTEGERS_H__
