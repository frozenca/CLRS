#ifndef __CLRS4_INSERTION_SORT_H__
#define __CLRS4_INSERTION_SORT_H__

#include <02_getting_started/easy/03_binary_search.h>
#include <cassert>
#include <vector>
namespace frozenca {

using namespace std;

void insertion_sort(vector<int> &A) {
  for (long i = 1; i < ssize(A); ++i) {
    int key = A[i];
    long j = i - 1;
    // insert A[i] to the sorted subarray A[:i-1)
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

void insertion_sort_recursive(vector<int> &A, const long n) {
  if (n <= 1) {
    return;
  }
  assert(n <= ssize(A));
  insertion_sort_recursive(A, n - 1);
  long key_index = n - 1;
  long j = key_index - 1;
    while (j >= 0 && A[j] > A[key_index]) {
      swap(A[j], A[key_index]);
      key_index = j;
      --j;
  }
}

void insertion_sort_recursive(vector<int> &A) {
  insertion_sort_recursive(A, ssize(A));
}

void insertion_sort_recursive_binary_search(vector<int> &A, const long n) {
  if (n <= 1) {
    return;
  }
  assert(n <= ssize(A));
  insertion_sort_recursive_binary_search(A, n - 1);
  int key = A[n - 1];
  auto l = binary_search_lower_bound(A, key, 0, n - 1);
  for (long i = n - 1; i > l; --i) {
    A[i] = A[i - 1];
  }
  A[l] = key;
}

void insertion_sort_recursive_binary_search(vector<int> &A) {
  insertion_sort_recursive_binary_search(A, ssize(A));
}

} // namespace frozenca

#endif //__CLRS4_INSERTION_SORT_H__
