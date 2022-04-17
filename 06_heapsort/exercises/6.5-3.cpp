#include <06_heapsort/inc/easy.h>
#include <algorithm>
#include <iostream>

int main() {
  namespace fc = frozenca;
  using namespace std;

  {
    vector<int> A{27, 17, 3, 16, 13, 10, 1, 5, 7, 12, 4, 8, 9, 0};
    fc::build_max_heap(A);
    fc::max_heap_insert(A, 20);
    fc::print(A);
    if (ranges::is_heap(A)) {
      cout << "Max heap property maintained\n";
    } else {
      cout << "Max heap property is broken\n";
    }
  }

  {
    vector<int> A{27, 17, 3, 16, 13, 10, 1, 5, 7, 12, 4, 8, 9, 0};
    fc::build_min_heap(A);
    fc::min_heap_insert(A, 20);
    fc::print(A);
    if (ranges::is_heap(A, ranges::greater{})) {
      cout << "Min heap property maintained\n";
    } else {
      cout << "Min heap property is broken\n";
    }
  }
}