#ifndef __CLRS4_PRIORITY_QUEUES_H__
#define __CLRS4_PRIORITY_QUEUES_H__

#include "01_heaps.h"
#include "02_maintaining_the_heap_property.h"
#include "03_building_a_heap.h"
#include <cassert>
#include <common.h>
#include <limits>
#include <stdexcept>

namespace frozenca {

using namespace std;

int max_heap_maximum(const vector<int> &A) {
  if (ssize(A) < 1) {
    throw runtime_error("heap underflow\n");
  }
  return A.front();
}

int max_heap_extract_max(vector<int> &A) {
  auto max_elem = max_heap_maximum(A);
  A.front() = A.back();
  A.pop_back();
  max_heapify(A, 0, ssize(A));
  return max_elem;
}

void max_heap_increase_key(vector<int> &A, index_t i, int key) {
  if (key < A[i]) {
    throw invalid_argument("new key is smaller than current key\n");
  }
  A[i] = key;
  while (i > 0 && A[heap_parent(i)] < A[i]) {
    swap(A[i], A[heap_parent(i)]);
    i = heap_parent(i);
  }
}

void max_heap_insert(vector<int> &A, int key) {
  A.push_back(numeric_limits<int>::min());
  max_heap_increase_key(A, ssize(A) - 1, key);
}

int min_heap_minimum(const vector<int> &A) {
  if (ssize(A) < 1) {
    throw runtime_error("heap underflow\n");
  }
  return A.front();
}

int min_heap_extract_min(vector<int> &A) {
  auto min_elem = min_heap_minimum(A);
  A.front() = A.back();
  A.pop_back();
  min_heapify(A, 0, ssize(A));
  return min_elem;
}

void min_heap_decrease_key(vector<int> &A, index_t i, int key) {
  if (key > A[i]) {
    throw invalid_argument("new key is larger than current key\n");
  }
  A[i] = key;
  while (i > 0 && A[heap_parent(i)] > A[i]) {
    swap(A[i], A[heap_parent(i)]);
    i = heap_parent(i);
  }
}

void min_heap_insert(vector<int> &A, int key) {
  A.push_back(numeric_limits<int>::max());
  min_heap_decrease_key(A, ssize(A) - 1, key);
}

void max_heap_decrease_key(vector<int> &A, index_t i, int key) {
  if (key > A[i]) {
    throw invalid_argument("new key is larger than current key\n");
  }
  A[i] = key;
  max_heapify(A, i, ssize(A));
}

void max_heap_delete(vector<int> &A, index_t i) {
  assert(i < ssize(A));
  if (A[i] > A.back()) {
    A[i] = A.back();
    max_heapify(A, i, ssize(A));
  } else {
    max_heap_increase_key(A, i, A.back());
  }
  A.pop_back();
}


void build_max_heap2(vector<int> &A) {
  auto B = A;
  A.clear();
  for (index_t i = 0; i < ssize(B); ++i) {
    max_heap_insert(A, B[i]);
  }
}

} // namespace frozenca

#endif //__CLRS4_PRIORITY_QUEUES_H__
