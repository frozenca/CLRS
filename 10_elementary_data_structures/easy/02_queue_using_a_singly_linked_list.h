#ifndef __CLRS4_QUEUE_SINGLE_LINKED_LIST_H__
#define __CLRS4_QUEUE_SINGLE_LINKED_LIST_H__

#include <common.h>
#include <forward_list>

namespace frozenca {

using namespace std;

template <typename T> class QueueSinglyLinkedList {
  forward_list<T> l_;
  forward_list<T>::iterator it_;

public:
  QueueSinglyLinkedList() {
    it_ = l_.begin();
  }

  void enqueue(const T &x) { 
    if (l_.empty()) {
      l_.push_front(x);
      it_ = l_.begin();
    } else {
      it_ = l_.insert_after(it_, x);
    }
   }

  T dequeue() {
    auto x = l_.front();
    l_.pop_front();
    return x;
  }
};

} // namespace frozenca

#endif //__CLRS4_QUEUE_SINGLE_LINKED_LIST_H__
