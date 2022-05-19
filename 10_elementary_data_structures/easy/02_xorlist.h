#ifndef __CLRS4_XORLIST_H__
#define __CLRS4_XORLIST_H__

#include <common.h>
#include <bit>
#include <iostream>

namespace frozenca {

using namespace std;

template <typename T> struct XORNode {
  T key;
  ptrdiff_t np = 0;
  XORNode(const T &key) : key{key} {}
};

template <typename T> class XORList {
  using Node = XORNode<T>;
  Node *head = nullptr;
  Node *tail = nullptr;

public:
  ~XORList() {
    ptrdiff_t prev = 0;
    auto y = head;
    while (bit_cast<ptrdiff_t>(y)) {
      ptrdiff_t next = y->np;
      head = bit_cast<Node *>(next);
      if (next) {
        auto next_ptr = bit_cast<Node *>(next);
        next_ptr->np = next_ptr->np ^ bit_cast<ptrdiff_t>(y) ^ prev;
      } else {
        tail = bit_cast<Node *>(prev);
      }
      auto temp = y;
      delete temp;
      y = bit_cast<Node *>(next);
    }
  }

  Node *list_search(const T &k) {
    ptrdiff_t prev = 0;
    auto x = head;
    while (bit_cast<ptrdiff_t>(x) && x->key != k) {
      ptrdiff_t next = prev ^ x->np;
      prev = bit_cast<ptrdiff_t>(x);
      x = bit_cast<Node *>(next);
    }
    return x;
  }

  void list_insert(const T &k) {
    auto x = new Node(k);
    if (!head) {
      head = x;
      tail = x;
    } else if (head == tail) {
      tail = x;
      head->np = bit_cast<ptrdiff_t>(x);
      tail->np = bit_cast<ptrdiff_t>(head);
    } else {
      tail->np = tail->np ^ bit_cast<ptrdiff_t>(x);
      x->np = bit_cast<ptrdiff_t>(tail);
      tail = x;
    }
  }

  void list_delete(const T &k) {
    auto y = head;
    ptrdiff_t prev = 0;
    while (bit_cast<ptrdiff_t>(y)) {
      ptrdiff_t next = prev ^ y->np;
      if (y->key != k) {
        prev = bit_cast<ptrdiff_t>(y);
        y = bit_cast<Node *>(next);
      } else {
        if (prev) {
          auto prev_ptr = bit_cast<Node *>(prev);
          prev_ptr->np = prev_ptr->np ^ bit_cast<ptrdiff_t>(y) ^ next;
        } else {
          head = bit_cast<Node *>(next);
        }
        if (next) {
          auto next_ptr = bit_cast<Node *>(next);
          next_ptr->np = next_ptr->np ^ bit_cast<ptrdiff_t>(y) ^ prev;
        } else {
          tail = bit_cast<Node *>(prev);
        }
        auto temp = y;
        delete temp;
        y = bit_cast<Node *>(next);
      }
    }
  }

  void list_reverse() { swap(head, tail); }

  friend ostream &operator<<(ostream &os, const XORList &l) {
    ptrdiff_t prev = 0;
    Node* x = l.head;
    while (bit_cast<ptrdiff_t>(x)) {
      os << x->key << ' ';
      ptrdiff_t next = prev ^ x->np;
      prev = bit_cast<ptrdiff_t>(x);
      x = bit_cast<Node *>(next);
    }
    return os;
  }
};

} // namespace frozenca

#endif //__CLRS4_XORLIST_H__
