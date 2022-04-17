#ifndef __CLRS4_THE_HEAPSORT_ALGORITHM_H__
#define __CLRS4_THE_HEAPSORT_ALGORITHM_H__

#include "01_heaps.h"
#include "02_maintaining_the_heap_property.h"
#include "03_building_a_heap.h"
#include <cassert>
#include <common.h>

namespace frozenca {

using namespace std;

void heapsort(vector<int> &A) {
  build_max_heap(A);
  auto heap_size = ssize(A);
  for (index_t i = ssize(A) - 1; i >= 1; --i) {
    swap(A[0], A[i]);
    --heap_size;
    max_heapify(A, 0, heap_size);
  }
}

} // namespace frozenca

#endif //__CLRS4_THE_HEAPSORT_ALGORITHM_H__
