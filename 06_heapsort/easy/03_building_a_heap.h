#ifndef __CLRS4_BUILDING_A_HEAP_H__
#define __CLRS4_BUILDING_A_HEAP_H__

#include "01_heaps.h"
#include "02_maintaining_the_heap_property.h"
#include <cassert>
#include <common.h>


namespace frozenca {

using namespace std;

void build_max_heap(vector<int> &A) {
  for (index_t i = ssize(A) / 2 - 1; i >= 0; --i) {
    max_heapify(A, i, ssize(A));
  }
}

void build_min_heap(vector<int> &A) {
  for (index_t i = ssize(A) / 2 - 1; i >= 0; --i) {
    min_heapify(A, i, ssize(A));
  }
}

} // namespace frozenca

#endif //__CLRS4_HEAPS_H__
