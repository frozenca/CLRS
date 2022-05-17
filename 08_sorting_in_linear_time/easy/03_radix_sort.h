#ifndef __CLRS4_RADIX_SORT_H__
#define __CLRS4_RADIX_SORT_H__

#include <algorithm>
#include <cassert>
#include <cmath>
#include <common.h>

namespace frozenca {

using namespace std;

void radix_sort(vector<uint16_t> &A, int d) {
  // use base of 2
  const auto n = ssize(A);
  for (int i = 0; i < d; ++i) {
    vector<uint16_t> B(n);
    vector<int> C(3);
    // count i-th bit
    for (auto num : A) {
      C[(num >> i) & 1]++;
    }
    C[1] += C[0];
    C[2] += C[1];

    // Copy A to B, starting from the end of A.
    for (index_t j = n - 1; j >= 0; --j) {
      auto num = A[j];
      auto mask = (num >> i) & 1;
      B[C[mask] - 1] = num;
      C[mask]--; // to handle duplicate values
    }
    swap(A, B);
  }
}

void radix_sort_cubic(vector<int> &A) {
  // use base of n
  const auto n = ssize(A);
  for (int i = 0; i < 3; ++i) {
    vector<int> B(n);
    vector<int> C(n + 1);
    int npow = static_cast<int>(pow(n, i));
    // count i-th digit
    for (auto num : A) {
      C[(num / npow) % n]++;
    }
    for (int j = 1; j <= n; ++j) {
      C[j] += C[j - 1];
    }
    for (index_t j = n - 1; j >= 0; --j) {
      auto num = A[j];
      auto digit = (num / npow) % n;
      B[C[digit] - 1] = num;
      C[digit]--;
    }

    swap(A, B);
  }
}

} // namespace frozenca

#endif //__CLRS4_RADIX_SORT_H__
