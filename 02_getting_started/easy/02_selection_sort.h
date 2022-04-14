#ifndef __CLRS4_SELECTION_SORT_H__
#define __CLRS4_SELECTION_SORT_H__

#include <core/common.h>
#include <utility>
#include <vector>

namespace frozenca {

using namespace std;

void selection_sort(vector<int> &A) {
  for (index_t i = 0; i < ssize(A) - 1; ++i) {
    int min_value = A[i];
    index_t min_index = i;
    for (index_t j = i + 1; j < ssize(A); ++j) {
      if (min_value > A[j]) {
        min_value = A[j];
        min_index = j;
      }
    }
    swap(A[i], A[min_index]);
  }
}

} // namespace frozenca

#endif //__CLRS4_SELECTION_SORT_H__
