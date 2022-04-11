#ifndef __CLRS4_SELECTION_SORT_H__
#define __CLRS4_SELECTION_SORT_H__

#include <utility>
#include <vector>

namespace frozenca {

using namespace std;

void selection_sort(vector<int> &A) {
  for (long i = 0; i < ssize(A) - 1; ++i) {
    int min_value = A[i];
    long min_index = i;
    for (long j = i + 1; j < ssize(A); ++j) {
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
