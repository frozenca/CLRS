#ifndef __CLRS4_LINKED_LIST_H__
#define __CLRS4_LINKED_LIST_H__

#include <common.h>
#include <iostream>

namespace frozenca {

using namespace std;

template <typename T> class ListNode {
public:
  T key_;
  ListNode *prev_ = nullptr;
  ListNode *next_ = nullptr;
  ListNode(const T &key) : key_{key} {}
};

template <typename T> class LinkedList {
  using Node = ListNode<T>;

  Node *head_ = nullptr;

public:
  LinkedList() = default;
  LinkedList(const LinkedList &l) = default;
  LinkedList &operator=(const LinkedList &l) = default;
  LinkedList(LinkedList &&l) noexcept = default;
  LinkedList &operator=(LinkedList &&l) noexcept = default;

  ~LinkedList() {
    auto node = head_;
    while (node) {
      auto next = node->next_;
      delete node;
      node = next;
    }
  }

  Node *list_search(const T &key) const {
    auto x = head_;
    while (x && x->key_ != key) {
      x = x->next_;
    }
    return x;
  }

  void list_prepend(Node *x) {
    if (!x) {
      return;
    }
    x->next_ = head_;
    x->prev_ = nullptr;
    if (head_) {
      head_->prev_ = x;
    }
    head_ = x;
  }

  void list_prepend(const T &key) {
    auto x = new Node(key);
    list_prepend(x);
  }

  static void list_insert(Node *x, Node *y) {
    if (!x || !y) {
      return;
    }
    x->next_ = y->next_;
    x->prev_ = y;
    if (y->next_) {
      y->next_->prev_ = x;
    }
    y->next_ = x;
  }

  void list_insert(Node *y, const T &key) {
    auto x = new Node(key);
    list_insert(x, y);
  }

  void list_delete(Node *x) {
    if (!x) {
      return;
    }
    if (x->prev_) {
      x->prev_->next_ = x->next_;
    } else {
      head_ = x->next_;
    }
    if (x->next_) {
      x->next_->prev_ = x->prev_;
    }
  }

  friend ostream &operator<<(ostream &os, const LinkedList &l) {
    auto x = l.head_;
    while (x) {
      os << x->key_ << ' ';
      x = x->next_;
      if (x == l.head_) {
        break;
      }
    }
    return os;
  }
};

template <typename T> class LinkedListSentinel {
  using Node = ListNode<T>;

  Node *nil_ = nullptr;

public:
  LinkedListSentinel() : nil_{new Node(T{})} {
    nil_->prev_ = nil_;
    nil_->next_ = nil_;
  }

  LinkedListSentinel(const LinkedListSentinel &l) {
    nil_ = new Node(T{});
    if (l.nil_->prev_ != l.nil_) {
      nil_->prev_ = l.nil_->prev_;
      nil_->prev_->next_ = nil_;
    }
    if (l.nil_->next_ != l.nil_) {
      nil_->next_ = l.nil_->next_;
      nil_->next_->prev_ = nil_;
    }
  }
  LinkedListSentinel &operator=(const LinkedListSentinel &l) {
    if (l.nil_->prev_ != l.nil_) {
      nil_->prev_ = l.nil_->prev_;
      nil_->prev_->next_ = nil_;
    }
    if (l.nil_->next_ != l.nil_) {
      nil_->next_ = l.nil_->next_;
      nil_->next_->prev_ = nil_;
    }
    return *this;
  }
  LinkedListSentinel(LinkedListSentinel &&l) noexcept {
    nil_ = new Node(T{});
    if (l.nil_->prev_ != l.nil_) {
      nil_->prev_ = l.nil_->prev_;
      nil_->prev_->next_ = nil_;
    }
    if (l.nil_->next_ != l.nil_) {
      nil_->next_ = l.nil_->next_;
      nil_->next_->prev_ = nil_;
    }
    l.nil_->next_ = l.nil_;
    l.nil_->prev_ = l.nil_;
  }
  LinkedListSentinel &operator=(LinkedListSentinel &&l) noexcept {
    if (l.nil_->prev_ != l.nil_) {
      nil_->prev_ = l.nil_->prev_;
      nil_->prev_->next_ = nil_;
    }
    if (l.nil_->next_ != l.nil_) {
      nil_->next_ = l.nil_->next_;
      nil_->next_->prev_ = nil_;
    }
    l.nil_->next_ = l.nil_;
    l.nil_->prev_ = l.nil_;
    return *this;
  }

  ~LinkedListSentinel() {
    auto node = nil_->next_;
    while (node != nil_) {
      auto next = node->next_;
      delete node;
      node = next;
    }
    delete nil_;
  }

  void list_prepend(Node *x) {
    if (!x) {
      return;
    }
    x->next_ = nil_->next_;
    x->prev_ = nil_;
    nil_->next_->prev_ = x;
    nil_->next_ = x;
  }

  void list_prepend(const T &key) {
    auto x = new Node(key);
    list_prepend(x);
  }

  Node *list_search(const T &key) const {
    nil_->key_ =
        key; // store the key in the sentinel to guarantee it is in list
    auto x = nil_->next_; // start at the head of the list
    while (x->key_ != key) {
      x = x->next_;
    }
    if (x == nil_) {  // found k in the sentinel
      return nullptr; // k was not really in the list
    } else {
      return x; // found k in element x
    }
  }

  static void list_insert(Node *x, Node *y) {
    if (!x || !y) {
      return;
    }
    x->next_ = y->next_;
    x->prev_ = y;
    y->next_->prev_ = x;
    y->next_ = x;
  }

  void list_insert(Node *y, const T &key) {
    auto x = new Node(key);
    list_insert(x, y);
  }

  void list_delete(Node *x) {
    if (!x) {
      return;
    }
    x->prev_->next_ = x->next_;
    x->next_->prev_ = x->prev_;
  }

  friend ostream &operator<<(ostream &os, const LinkedListSentinel &l) {
    auto x = l.nil_->next_;
    while (x) {
      os << x->key_ << ' ';
      x = x->next_;
      if (x == l.nil_) {
        break;
      }
    }
    return os;
  }

  void list_reverse() {
    {
      auto a = nil_;
      auto b = nil_->next_;
      while (b != nil_) {
        auto c = b->next_;
        b->next_ = a;
        a = b;
        b = c;
      }
      nil_->next_ = a;
    }
    {
      auto a = nil_;
      auto b = nil_->prev_;
      while (b != nil_) {
        auto c = b->prev_;
        b->prev_ = a;
        a = b;
        b = c;
      }
      nil_->prev_ = a;
    }
  }

  template <typename T>
  friend LinkedListSentinel<T> list_union(LinkedListSentinel<T> &l1,
                                          LinkedListSentinel<T> &l2);
};

template <typename T>
LinkedListSentinel<T> list_union(LinkedListSentinel<T> &l1,
                                 LinkedListSentinel<T> &l2) {
  LinkedListSentinel<T> l;
  auto head1 = l1.nil_->next_;
  auto tail1 = l1.nil_->prev_;
  auto head2 = l2.nil_->next_;
  auto tail2 = l2.nil_->prev_;

  l1.nil_->next_ = l1.nil_;
  l1.nil_->prev_ = l1.nil_;

  l2.nil_->next_ = l2.nil_;
  l2.nil_->prev_ = l2.nil_;

  if (head1 != l1.nil_) {
    l.nil_->next_ = head1;
    head1->prev_ = l.nil_;
  }

  if (tail1 != l1.nil_) {
    tail1->next_ = (head2 != l2.nil_) ? head2 : l.nil_;
  }

  if (head2 != l2.nil_) {
    head2->prev_ = (tail1 != l1.nil_) ? tail1 : l.nil_;
  }

  if (tail2 != l2.nil_) {
    tail2->next_ = l.nil_;
    l.nil_->prev_ = tail2;
  }

  return l;
}

} // namespace frozenca

#endif //__CLRS4_LINKED_LIST_H__
