#ifndef __CLRS4_MAINTAINING_THE_HEAP_PROPERTY_H__
#define __CLRS4_MAINTAINING_THE_HEAP_PROPERTY_H__

#include "01_heaps.h"
#include <common.h>

namespace frozenca {

using namespace std;

void max_heapify(vector<int> &A, index_t i, index_t heap_size) {
  assert(i >= 0 && i < heap_size);
  auto l = heap_left(i);
  auto r = heap_right(i);
  index_t largest = i;
  if (l < heap_size && A[l] > A[i]) {
    largest = l;
  }
  if (r < heap_size && A[r] > A[largest]) {
    largest = r;
  }
  if (largest != i) {
    swap(A[i], A[largest]);
    max_heapify(A, largest, heap_size);
  }
}

void min_heapify(vector<int> &A, index_t i, index_t heap_size) {
  assert(i >= 0 && i < heap_size);
  auto l = heap_left(i);
  auto r = heap_right(i);
  index_t smallest = i;
  if (l < heap_size && A[l] < A[i]) {
    smallest = l;
  }
  if (r < heap_size && A[r] < A[smallest]) {
    smallest = r;
  }
  if (smallest != i) {
    swap(A[i], A[smallest]);
    min_heapify(A, smallest, heap_size);
  }
}

void max_heapify_iterative(vector<int> &A, index_t i, index_t heap_size) {
  while (i <= heap_size / 2) {
    auto l = heap_left(i);
    auto r = heap_right(i);
    index_t largest = i;
    if (l < heap_size && A[l] > A[i]) {
      largest = l;
    }
    if (r < heap_size && A[r] > A[largest]) {
      largest = r;
    }
    if (largest != i) {
      swap(A[i], A[largest]);
      i = largest;
    } else {
      return;
    }
  }
}

} // namespace frozenca

#endif //__CLRS4_MAINTAINING_THE_HEAP_PROPERTY_H__
