#ifndef __CLRS4_PROBLEMS_02_02_H__
#define __CLRS4_PROBLEMS_02_02_H__

#include <core/common.h>
#include <utility>
#include <vector>

namespace frozenca {

using namespace std;

void bubble_sort(vector<int> &A) {
  for (index_t i = 0; i < ssize(A) - 1; ++i) {
    for (index_t j = ssize(A) - 1; j > i; --j) {
      if (A[j] < A[j - 1]) {
        swap(A[j], A[j - 1]);
      }
    }
  }
}

} // namespace frozenca

#endif //__CLRS4_PROBLEMS_02_02_H__
