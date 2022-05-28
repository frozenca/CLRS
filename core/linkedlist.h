#ifndef __CLRS4_LINKED_LIST_H__
#define __CLRS4_LINKED_LIST_H__

#include <common.h>

namespace frozenca::hard {

using namespace std;

namespace detail {
template <Containable T> class ListNode {
public:
  T key_;
  ListNode *prev_ = nullptr;
  ListNode *next_ = nullptr;
  ListNode(T key) : key_{move(key)} {}
  ListNode(const ListNode &other) = delete;
  ListNode &operator=(const ListNode &other) = delete;
};

template <Containable T, bool Const> class ListIterator {
public:
  using difference_type = ptrdiff_t;
  using value_type = T;
  using pointer = conditional_t<Const, const T *, T *>;
  using reference = conditional_t<Const, const T &, T &>;
  using iterator_category = bidirectional_iterator_tag;
  using iterator_concept = iterator_category;
  using Node = ListNode<T>;

public:
  Node *node_ = nullptr;

  ListIterator() = default;
  ListIterator(Node *node) : node_{node} {}

  ListIterator(const ListIterator<T, !Const> &other) : node_{other.node_} {}

  ListIterator(const ListIterator &other) = default;
  ListIterator &operator=(const ListIterator &other) = default;
  ListIterator(ListIterator &&other) noexcept = default;
  ListIterator &operator=(ListIterator &&other) noexcept = default;

  reference operator*() const noexcept { return node_->key_; }

  pointer operator->() const noexcept { return &(node_->key_); }

  ListIterator &operator++() {
    node_ = node_->next_;
    return *this;
  }

  ListIterator operator++(int) {
    ListIterator temp = *this;
    node_ = node_->next_;
    return temp;
  }

  ListIterator &operator--() {
    node_ = node_->prev_;
    return *this;
  }

  ListIterator operator--(int) {
    ListIterator temp = *this;
    node_ = node_->prev_;
    return temp;
  }

  [[nodiscard]] friend bool operator==(const ListIterator &lhs,
                                       const ListIterator &rhs) {
    return lhs.node_ == rhs.node_;
  }
};

} // namespace detail

template <Containable T> class LinkedList {
  using Node = detail::ListNode<T>;

  Node *head_; // sentinel node
  ptrdiff_t size_ = 0;

public:
  using value_type = T;
  using reference_type = T &;
  using const_reference_type = const T &;
  using iterator_type = detail::ListIterator<T, false>;
  using const_iterator_type = detail::ListIterator<T, true>;
  using reverse_iterator_type = reverse_iterator<iterator_type>;
  using const_reverse_iterator_type = reverse_iterator<const_iterator_type>;

  LinkedList() { construct(); }
  LinkedList(const LinkedList &l) {
    construct();
    try {
      for (const auto &val : l) {
        push_back(val);
      }
    } catch (exception& e){ 
      clear();
      throw e;
    }
  }
  LinkedList &operator=(const LinkedList &l) {
    clear();
    try {
      for (const auto &val : l) {
        push_back(val);
      }
    } catch (exception& e){ 
      clear();
      throw e;
    }
    return *this;
  }
  LinkedList(LinkedList &&l) noexcept : head_{move(l.head_)}, size_{l.size_} {
    l.construct();
  }
  LinkedList &operator=(LinkedList &&l) noexcept {
    destroy();
    head_ = move(l.head_);
    size_ = l.size_;
    l.construct();
    return *this;
  }

private:
  void construct() {
    head_ = allocator<Node>{}.allocate(1);
    head_->next_ = head_;
    head_->prev_ = head_;
    size_ = 0;
  }

  void destroy() {
    head_->prev_->next_ = nullptr;
    auto curr = head_->next_;
    while (curr) {
      auto next = curr->next_;
      delete curr;
      curr = next;
    }
    allocator<Node>{}.deallocate(head_, 1);
    size_ = 0;
  }

public:
  ~LinkedList() { destroy(); }

  [[nodiscard]] reference_type front() {
    if (head_->next_ == head_) {
      throw runtime_error("List is empty\n");
    }
    return head_->next_->key_;
  }

  [[nodiscard]] const_reference_type front() const {
    if (head_->next_ == head_) {
      throw runtime_error("List is empty\n");
    }
    return head_->next_->key_;
  }

  [[nodiscard]] reference_type back() {
    if (head_->prev_ == head_) {
      throw runtime_error("List is empty\n");
    }
    return head_->prev_->key_;
  }

  [[nodiscard]] const_reference_type back() const {
    if (head_->prev_ == head_) {
      throw runtime_error("List is empty\n");
    }
    return head_->prev_->key_;
  }

  [[nodiscard]] iterator_type begin() noexcept {
    return iterator_type(head_->next_);
  }

  [[nodiscard]] const_iterator_type begin() const noexcept {
    return const_iterator_type(head_->next_);
  }

  [[nodiscard]] const_iterator_type cbegin() const noexcept {
    return const_iterator_type(head_->next_);
  }

  [[nodiscard]] iterator_type end() noexcept { return iterator_type(head_); }

  [[nodiscard]] const_iterator_type end() const noexcept {
    return const_iterator_type(head_);
  }

  [[nodiscard]] const_iterator_type cend() const noexcept {
    return const_iterator_type(head_);
  }

  [[nodiscard]] reverse_iterator_type rbegin() noexcept {
    return reverse_iterator_type(end());
  }

  [[nodiscard]] const_reverse_iterator_type rbegin() const noexcept {
    return const_reverse_iterator_type(end());
  }

  [[nodiscard]] const_reverse_iterator_type crbegin() const noexcept {
    return const_reverse_iterator_type(end());
  }

  [[nodiscard]] reverse_iterator_type rend() noexcept {
    return reverse_iterator_type(begin());
  }

  [[nodiscard]] const_reverse_iterator_type rend() const noexcept {
    return const_reverse_iterator_type(begin());
  }

  [[nodiscard]] const_reverse_iterator_type crend() const noexcept {
    return const_reverse_iterator_type(begin());
  }

  [[nodiscard]] bool empty() const noexcept { return size_ == 0; }

  [[nodiscard]] ptrdiff_t size() const noexcept { return size_; }

  void clear() noexcept {
    head_->prev_->next_ = nullptr;
    auto curr = head_->next_;
    while (curr && curr != head_) {
      auto next = curr->next_;
      delete curr;
      curr = next;
    }
    head_->next_ = head_;
    head_->prev_ = head_;
    size_ = 0;
  }

private:
  void insert_before(Node *where, const T &value) {
    assert(where);
    auto node = new Node(value);
    node->next_ = where;
    node->prev_ = where->prev_;
    where->prev_->next_ = node;
    where->prev_ = node;
    ++size_;
  }

  void erase_at(Node *where) {
    assert(where && where->prev_ && where->next_);
    if (where == head_) {
      throw invalid_argument("attempt to erase at end()\n");
    }
    where->prev_->next_ = where->next_;
    where->next_->prev_ = where->prev_;
    delete where;
    --size_;
  }

  void erase_range(Node* first, Node* last) {
    assert(first && first->prev_ && first->next_);
    assert(last && last->prev_ && last->next_);
    if (first == head_) {
      throw invalid_argument("attempt to erase from end()\n");
    }
    first->prev_->next_ = last;
    last->prev_ = first->prev_;
    auto curr = first;
    while (curr != last) {
      auto next = curr->next_;
      delete curr;
      curr = next;
      --size_;
    }
  }

public:
  iterator_type insert(const_iterator_type pos, const T &value) {
    auto where = pos.node_;
    insert_before(where, value);
    return iterator_type(where->prev_);
  }

  iterator_type erase(iterator_type pos) {
    auto where = pos.node_;
    auto next = where->next_;
    erase_at(where);
    return iterator_type(next);
  }

  iterator_type erase(iterator_type first, iterator_type last) {
    erase_range(first.node_, last.node_);
    return last;
  }

  void splice(const_iterator_type pos, const_iterator_type first_iter, const_iterator_type last_iter) {
    auto where = pos.node_;
    auto first = first_iter.node_;
    auto last = last_iter.node_;

    first->prev_->next_ = last;
    last->prev_->next_ = where;
    where->prev_->next_ = first;

    auto prev = where->prev_;
    where->prev_ = last->prev_;
    last->prev_ = first->prev_;
    first->prev_ = prev;
  }

  void push_back(const T &value) { insert_before(head_, value); }

  void push_front(const T &value) { insert_before(head_->next_, value); }

  void pop_back() { erase_at(head_->prev_); }

  void pop_front() { erase_at(head_->next_); }
};

} // namespace frozenca::hard

#endif //__CLRS4_LINKED_LIST_H__
