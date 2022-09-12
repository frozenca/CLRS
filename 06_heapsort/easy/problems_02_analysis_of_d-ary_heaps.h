#ifndef __CLRS4_D_ARY_HEAPS_H__
#define __CLRS4_D_ARY_HEAPS_H__

#include <cassert>
#include <common.h>
#include <stdexcept>
#include <utility>
#include <vector>

namespace frozenca {

using namespace std;

namespace {

inline constexpr index_t h_d = 4;

} // anonymous namespace

inline constexpr index_t dary_heap_parent(index_t i) {
  if (i == 0) {
    return 0;
  } else {
    assert(h_d > 0);
    return (i - 1) / h_d;
  }
}

inline constexpr index_t dary_heap_child(index_t i, index_t j) {
  return h_d * i + j + 1;
}

void dary_max_heapify(vector<int> &A, index_t i) {
  assert(i >= 0 && i < ssize(A));
  index_t largest = i;
  for (index_t j = 0; j < h_d; ++j) {
    auto child = dary_heap_child(i, j);
    if (child < ssize(A) && A[child] > A[largest]) {
      largest = child;
    }
  }
  if (largest != i) {
    swap(A[i], A[largest]);
    dary_max_heapify(A, largest);
  }
}

void dary_build_max_heap(vector<int> &A) {
  for (index_t i = ssize(A) / h_d; i >= 0; i--) {
    dary_max_heapify(A, i);
  }
}

int dary_heap_extract_max(vector<int> &A) {
  if (ssize(A) < 1) {
    throw underflow_error("heap underflow");
  }
  auto max_elem = A.front();
  A.front() = A.back();
  A.pop_back();
  if (!A.empty()) {
    dary_max_heapify(A, 0);
  }
  return max_elem;
}

void dary_max_heap_increase_key(vector<int> &A, index_t i, int key) {
  if (key < A[i]) {
    throw invalid_argument("new key is smaller than current key");
  }
  A[i] = key;
  while (i > 0 && A[dary_heap_parent(i)] < A[i]) {
    swap(A[i], A[dary_heap_parent(i)]);
    i = dary_heap_parent(i);
  }
}

void dary_max_heap_insert(vector<int> &A, int key) {
  A.push_back(numeric_limits<int>::lowest());
  dary_max_heap_increase_key(A, ssize(A) - 1, key);
}

template <typename T> void dary_min_heapify(vector<T> &A, index_t i) {
  assert(i >= 0 && i < ssize(A));
  index_t largest = i;
  for (index_t j = 0; j < h_d; ++j) {
    auto child = dary_heap_child(i, j);
    if (child < ssize(A) && A[child] < A[largest]) {
      largest = child;
    }
  }
  if (largest != i) {
    swap(A[i], A[largest]);
    dary_min_heapify(A, largest);
  }
}

template <typename T> void dary_build_min_heap(vector<T> &A) {
  for (index_t i = ssize(A) / h_d; i >= 0; i--) {
    dary_min_heapify(A, i);
  }
}

template <typename T> T dary_heap_extract_min(vector<T> &A) {
  if (ssize(A) < 1) {
    throw underflow_error("heap underflow");
  }
  auto min_elem = A.front();
  A.front() = A.back();
  A.pop_back();
  if (!A.empty()) {
    dary_min_heapify(A, 0);
  }
  return min_elem;
}

template <typename T>
void dary_min_heap_decrease_key(vector<T> &A, index_t i, const T &key) {
  if (key > A[i]) {
    throw invalid_argument("new key is larger than current key");
  }
  A[i] = key;
  while (i > 0 && A[dary_heap_parent(i)] > A[i]) {
    swap(A[i], A[dary_heap_parent(i)]);
    i = dary_heap_parent(i);
  }
}

template <Descriptor V, Arithmetic F>
void dary_min_heap_insert(vector<pair<F, V>> &A, const pair<F, V> &key) {
  A.emplace_back(numeric_limits<int>::max(), V{});
  dary_min_heap_decrease_key(A, ssize(A) - 1, key);
}

} // namespace frozenca

#endif //__CLRS4_D_ARY_HEAPS_H__
