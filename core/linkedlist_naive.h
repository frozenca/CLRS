#ifndef __CLRS4_LINKED_LIST_NAIVE_H__
#define __CLRS4_LINKED_LIST_NAIVE_H__

#include <common.h>
#include <iostream>
#include <type_traits>

namespace frozenca {

using namespace std;

template <typename T> class ListNode {
public:
  T key_;
  ListNode *prev_ = nullptr;
  ListNode *next_ = nullptr;
  ListNode(const T &key) : key_{key} {}
  ListNode(const ListNode& other) = delete;
  ListNode& operator=(const ListNode& other) = delete;
};

template <typename T> class LinkedList {
  using Node = ListNode<T>;

  Node *head_ = nullptr;

public:
  LinkedList() = default;
  LinkedList(const LinkedList &l) = delete;
  LinkedList &operator=(const LinkedList &l) = delete;

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
    delete x;
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

template <typename K, typename V> class ListPairNode {
public:
  K key_;
  V value_;
  ListPairNode *prev_ = nullptr;
  ListPairNode *next_ = nullptr;
  ListPairNode(const pair<K, V> &x) : key_{x.first}, value_{x.second} {}
};

template <typename K, typename V, typename Node = ListPairNode<K, V>,
          bool Own = true, bool Delete = true>
class LinkedPairList {
  Node *head_ = nullptr;

public:
  LinkedPairList() = default;

  ~LinkedPairList() {
    if constexpr (Own) {
      auto node = head_;
      while (node) {
        auto next = node->next_;
        delete node;
        node = next;
      }
    }
  }

  int length() const {
    int len = 0;
    auto node = head_;
    while (node) {
      node = node->next_;
      len++;
    }
    return len;
  }

  V get_nth(int n) const {
    auto x = head_;
    for (int i = 0; i < n; ++i) {
      x = x ? x->next_ : nullptr;
    }
    return x ? x->value_ : V{};
  }

  Node *head() { return head_; }

  const Node *head() const { return head_; }

  V list_search(const K &key) const {
    auto x = head_;
    while (x && x->key_ != key) {
      x = x->next_;
    }
    return x ? x->value_ : V{};
  }

  Node *list_search_node(const K &key) const {
    auto x = head_;
    while (x && x->key_ != key) {
      x = x->next_;
    }
    return x;
  }

  bool list_contains(const K &key) const {
    auto x = head_;
    while (x && x->key_ != key) {
      x = x->next_;
    }
    return x ? true : false;
  }

  void list_prepend(Node *node) {
    if (!node) {
      return;
    }
    if constexpr (Own) {
      node->next_ = head_;
      node->prev_ = nullptr;
    }
    if (head_) {
      head_->prev_ = node;
    }
    head_ = node;
  }

  void list_prepend(const pair<K, V> &x) {
    auto node = new Node(x);
    list_prepend(node);
  }

  void list_delete(Node *node) {
    if (!node) {
      return;
    }
    if (node->prev_) {
      node->prev_->next_ = node->next_;
    } else {
      head_ = node->next_;
    }
    if (node->next_) {
      node->next_->prev_ = node->prev_;
    }
    if constexpr (Own) {
      node->prev_ = nullptr;
      node->next_ = nullptr;
    }
    if constexpr (Delete) {
      delete node;
    }
  }

  void list_delete(const K &key) {
    auto node = head_;
    while (node && node->key_ != key) {
      node = node->next_;
    }
    list_delete(node);
  }

  friend ostream &operator<<(ostream &os, const LinkedPairList &l) {
    auto x = l.head_;
    while (x) {
      os << '[' << x->key_ << ", " << x->value_ << "] ";
      x = x->next_;
      if (x == l.head_) {
        break;
      }
    }
    return os;
  }
};

template <typename K, typename V> class LinkedPairSortedList {
  using Node = ListPairNode<K, V>;

  Node *head_ = nullptr;

public:
  LinkedPairSortedList() = default;

  ~LinkedPairSortedList() {
    auto node = head_;
    while (node) {
      auto next = node->next_;
      delete node;
      node = next;
    }
  }

  V list_search(const K &key) const {
    auto x = head_;
    while (x && x->key_ != key) {
      x = x->next_;
    }
    return x ? x->value_ : V{};
  }

  void list_prepend(Node *node) {
    if (!node) {
      return;
    }
    auto x = head_;
    while (x && x->key_ < node->key_) {
      x = x->next_;
    }
    node->next_ = x;
    if (x) {
      node->prev_ = x->prev_;
      x->prev_ = node;
      if (x == head_) {
        head_ = node;
      }
    } else {
      head_ = node;
    }
  }

  void list_prepend(const pair<K, V> &x) {
    auto node = new Node(x);
    list_prepend(node);
  }

  void list_delete(Node *node) {
    if (!node) {
      return;
    }
    if (node->prev_) {
      node->prev_->next_ = node->next_;
    } else {
      head_ = node->next_;
    }
    if (node->next_) {
      node->next_->prev_ = node->prev_;
    }
    delete node;
  }

  void list_delete(const K &key) {
    auto node = head_;
    while (node && node->key_ != key) {
      node = node->next_;
    }
    list_delete(node);
  }

  friend ostream &operator<<(ostream &os, const LinkedPairSortedList &l) {
    auto x = l.head_;
    while (x) {
      os << '[' << x->key_ << ", " << x->value_ << "] ";
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
    delete x;
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

#endif //__CLRS4_LINKED_LIST_NAIVE_H__
