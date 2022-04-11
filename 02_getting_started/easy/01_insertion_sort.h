#ifndef __CLRS4_INSERTION_SORT_H__
#define __CLRS4_INSERTION_SORT_H__

#include <vector>

namespace frozenca {

using namespace std;

void insertion_sort(vector<int> &A) {
  for (long i = 1; i < ssize(A); ++i) {
    int key = A[i];
    long j = i - 1;
    while (j >= 0 && A[j] > key) {
      A[j + 1] = A[j];
      --j;
    }
    A[j + 1] = key;
  }
}

void insertion_sort_decreasing(vector<int> &A) {
  for (long i = 1; i < ssize(A); ++i) {
    int key = A[i];
    long j = i - 1;
    while (j >= 0 && A[j] < key) {
      A[j + 1] = A[j];
      --j;
    }
    A[j + 1] = key;
  }
}

} // namespace frozenca

#endif //__CLRS4_INSERTION_SORT_H__
