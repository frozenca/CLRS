#ifndef __CLRS4_MERGEABLE_HEAPS_LINKED_LIST_H__
#define __CLRS4_MERGEABLE_HEAPS_LINKED_LIST_H__

#include <algorithm>
#include <common.h>
#include <iostream>
#include <list>

namespace frozenca {

using namespace std;

template <typename T> class MergeableHeapSortedList {
  list<T> l;

public:
  void make_heap() {}

  void heap_insert(const T &key) {
    l.insert(ranges::find_if(l, [&key](const auto &k) { return k > key; }),
             key);
  }

  T heap_minimum() const { return l.front(); }

  T heap_extract_minimum() {
    auto min_elem = l.front();
    l.pop_front();
    return min_elem;
  }

  template <typename T>
  friend MergeableHeapSortedList<T>
  heap_union(MergeableHeapSortedList<T> &heap1,
             MergeableHeapSortedList<T> &heap2);

  friend ostream &operator<<(ostream &os, MergeableHeapSortedList &heap) {
    for (const auto &elem : heap.l) {
      os << elem << ' ';
    }
    os << '\n';
    return os;
  }
};

template <typename T>
MergeableHeapSortedList<T> heap_union(MergeableHeapSortedList<T> &heap1,
                                      MergeableHeapSortedList<T> &heap2) {
  auto heap = heap1;
  heap.l.merge(heap2.l);
  return heap;
}

template <typename T> class MergeableHeapUnsortedList {
  list<T> l;

public:
  void make_heap() {}

  void heap_insert(const T &key) { l.push_back(key); }

  T heap_minimum() const { return *ranges::min_element(l); }

  T heap_extract_minimum() {
    auto min_iter = ranges::min_element(l);
    auto min_elem = *min_iter;
    erase(l, min_elem); // since C++20
    return min_elem;
  }

  template <typename T>
  friend MergeableHeapUnsortedList<T>
  heap_union(MergeableHeapUnsortedList<T> &heap1,
             MergeableHeapUnsortedList<T> &heap2);

  friend ostream &operator<<(ostream &os, MergeableHeapUnsortedList &heap) {
    for (const auto &elem : heap.l) {
      os << elem << ' ';
    }
    os << '\n';
    return os;
  }
};

template <typename T>
MergeableHeapUnsortedList<T> heap_union(MergeableHeapUnsortedList<T> &heap1,
                                        MergeableHeapUnsortedList<T> &heap2) {
  auto heap = heap1;
  heap.l.splice(heap.l.end(), heap2.l);
  return heap;
}


template <typename T> class MergeableHeapUnsortedListDisjoint {
  list<T> l;

public:
  void make_heap() {}

  void heap_insert(const T &key) { l.push_back(key); }

  T heap_minimum() const { return *ranges::min_element(l); }

  T heap_extract_minimum() {
    auto min_iter = ranges::min_element(l);
    auto min_elem = *min_iter;
    l.erase(min_iter);
    return min_elem;
  }

  template <typename T>
  friend MergeableHeapUnsortedListDisjoint<T>
  heap_union(MergeableHeapUnsortedListDisjoint<T> &heap1,
             MergeableHeapUnsortedListDisjoint<T> &heap2);

  friend ostream &operator<<(ostream &os, MergeableHeapUnsortedListDisjoint &heap) {
    for (const auto &elem : heap.l) {
      os << elem << ' ';
    }
    os << '\n';
    return os;
  }
};

template <typename T>
MergeableHeapUnsortedListDisjoint<T> heap_union(MergeableHeapUnsortedListDisjoint<T> &heap1,
                                        MergeableHeapUnsortedListDisjoint<T> &heap2) {
  auto heap = heap1;
  heap.l.splice(heap.l.end(), heap2.l);
  return heap;
}

} // namespace frozenca

#endif //__CLRS4_MERGEABLE_HEAPS_LINKED_LIST_H__
