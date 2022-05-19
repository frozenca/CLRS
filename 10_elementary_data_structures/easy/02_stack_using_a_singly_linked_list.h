#ifndef __CLRS4_STACK_SINGLE_LINKED_LIST_H__
#define __CLRS4_STACK_SINGLE_LINKED_LIST_H__

#include <common.h>
#include <forward_list>

namespace frozenca {

using namespace std;

template <typename T> class StackSinglyLinkedList {
  forward_list<T> l;

public:
  void push(const T &x) { l.push_front(x); }

  T pop() {
    auto x = l.front();
    l.pop_front();
    return x;
  }
};

} // namespace frozenca

#endif //__CLRS4_STACK_SINGLE_LINKED_LIST_H__
