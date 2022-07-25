#include <06_heapsort/inc/easy.h>
#include <06_heapsort/inc/hard.h>
#include <algorithm>
#include <iostream>

int main() {
  namespace fc = frozenca;
  using namespace std;

  {
    vector<int> A{27, 17, 3, 16, 13, 10, 1, 5, 7, 12, 4, 8, 9, 0};
    fc::build_max_heap(A);
    fc::print(A);
    if (ranges::is_heap(A)) {
      cout << "Built max heap\n";
    } else {
      cout << "Building max heap failed\n";
    }
  }

  {
    vector<int> A{27, 17, 3, 16, 13, 10, 1, 5, 7, 12, 4, 8, 9, 0};
    fc::heapify(A);
    fc::print(A);
    if (ranges::is_heap(A)) {
      cout << "Built max heap\n";
    } else {
      cout << "Building max heap failed\n";
    }
  }

  {
    vector<int> A{27, 17, 3, 16, 13, 10, 1, 5, 7, 12, 4, 8, 9, 0};
    ranges::make_heap(A);
    fc::print(A);
    if (ranges::is_heap(A)) {
      cout << "Built max heap\n";
    } else {
      cout << "Building max heap failed\n";
    }
  }

  {
    vector<int> A{27, 17, 3, 16, 13, 10, 1, 5, 7, 12, 4, 8, 9, 0};
    fc::heapify(A);
    fc::print(A);
    if (ranges::is_heap(A)) {
      cout << "Built max heap\n";
    } else {
      cout << "Building max heap failed\n";
    }
  }

  {
    vector<int> A{27, 17, 3, 16, 13, 10, 1, 5, 7, 12, 4, 8, 9, 0};
    fc::heapify(A);
    fc::heap_sort(A);
    fc::print(A);
    if (ranges::is_sorted(A)) {
      cout << "Heap sort complete\n";
    } else {
      cout << "Heap sort failed\n";
    }
  }

  {
    vector<int> A{27, 17, 3, 16, 13, 10, 1, 5, 7, 12, 4, 8, 9, 0};
    ranges::make_heap(A);
    ranges::sort_heap(A);
    fc::print(A);
    if (ranges::is_sorted(A)) {
      cout << "Heap sort complete\n";
    } else {
      cout << "Heap sort failed\n";
    }
  }

  fc::verify_heapify(ranges::make_heap);
  fc::verify_heapify(fc::heapify);
  fc::perf_check_sorting(ranges::make_heap);
  fc::perf_check_sorting(fc::heapify);
  fc::verify_sorting(fc::make_heap_and_sort);
  fc::perf_check_sorting(ranges::sort);
  fc::perf_check_sorting(fc::make_heap_and_sort);
}