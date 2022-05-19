#include <10_elementary_data_structures/inc/easy.h>
#include <iostream>

namespace fc = frozenca;
using namespace std;

int main() {
  {
    using MHeap = fc::MergeableHeapSortedList<int>;
    MHeap h1;
    h1.heap_insert(3);
    h1.heap_insert(6);
    h1.heap_insert(2);
    h1.heap_insert(4);
    h1.heap_insert(5);

    cout << h1.heap_extract_minimum() << '\n';
    cout << h1.heap_extract_minimum() << '\n';
    cout << h1 << '\n';

    MHeap h2;
    h2.heap_insert(1);
    h2.heap_insert(1);
    h2.heap_insert(8);

    auto h = fc::heap_union(h1, h2);
    cout << h << '\n';
    cout << h.heap_extract_minimum() << '\n';
    cout << h << '\n';
  }
  {
    using MHeap = fc::MergeableHeapUnsortedList<int>;
    MHeap h1;
    h1.heap_insert(3);
    h1.heap_insert(6);
    h1.heap_insert(2);
    h1.heap_insert(4);
    h1.heap_insert(5);

    cout << h1.heap_extract_minimum() << '\n';
    cout << h1.heap_extract_minimum() << '\n';
    cout << h1 << '\n';

    MHeap h2;
    h2.heap_insert(1);
    h2.heap_insert(1);
    h2.heap_insert(8);

    auto h = fc::heap_union(h1, h2);
    cout << h << '\n';
    cout << h.heap_extract_minimum() << '\n';
    cout << h << '\n';
  }
  {
    using MHeap = fc::MergeableHeapUnsortedListDisjoint<int>;
    MHeap h1;
    h1.heap_insert(3);
    h1.heap_insert(6);
    h1.heap_insert(2);
    h1.heap_insert(4);
    h1.heap_insert(5);

    cout << h1.heap_extract_minimum() << '\n';
    cout << h1.heap_extract_minimum() << '\n';
    cout << h1 << '\n';

    MHeap h2;
    h2.heap_insert(1);
    h2.heap_insert(1);
    h2.heap_insert(8);

    auto h = fc::heap_union(h1, h2);
    cout << h << '\n';
    cout << h.heap_extract_minimum() << '\n';
    cout << h << '\n';
  }
}