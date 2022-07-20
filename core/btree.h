#ifndef __CLRS4_BTREE_H__
#define __CLRS4_BTREE_H__

#include <algorithm>
#include <cassert>
#include <common.h>
#include <ranges>
#include <iostream>
#include <vector>

namespace frozenca {

using namespace std;

namespace detail {

template <Containable K, typename V, index_t t, typename Comp, bool AllowDup,
          typename Alloc>
requires(t >= 2) class BTreeBase {
  static_assert(is_same_v<Alloc::value_type, V>,
                "Allocator value type is not V");

  // invariant: V is either K or pair<const K, Value> for some Value type.
  static constexpr bool is_set_ = is_same_v<K, V>;

  struct Proj {
    const K &operator()(const V &value) const noexcept {
      if constexpr (is_set_) {
        return value;
      } else {
        return value.first;
      }
    }
  };

  static_assert(indirect_strict_weak_order<
                Comp, projected<ranges::iterator_t<vector<V, Alloc>>, Proj>>);

  struct Node {
    // invariant: except root, t - 1 <= #(key) <= 2 * t - 1
    // invariant: for root, 0 <= #(key) <= 2 * t - 1
    // invariant: keys are sorted
    // invariant: for internal nodes, t <= #(child) == (#(key) + 1)) <= 2 * t
    // invariant: for root, 0 <= #(child) == (#(key) + 1)) <= 2 * t
    // invariant: for leaves, 0 == #(child)
    // invariant: child_0 <= key_0 <= child_1 <= ... <=  key_(N - 1) <= child_N
    vector<V, Alloc> keys_ = nullptr;
    vector<unique_ptr<Node>> children_;
    Node *parent_ = nullptr;
    index_t index_ = 0;

    // can throw bad_alloc
    explicit Node(const Alloc &alloc) : keys_(alloc) {
      keys_.reserve(2 * t - 1);
    }

    Node(const Node &node) = delete;
    Node &operator=(const Node &node) = delete;
    Node(Node &&node) = delete;
    Node &operator=(Node &&node) = delete;

    void clone(const Node &other) {
      keys_ = other.keys_;
      if (!other.children_.empty()) {
        assert(children_.empty());
        children_.resize(other.children_.size());
        for (index_t i = 0; i < ssize(other.children_); ++i) {
          children_[i] = make_unique<Node>(alloc_);
          children_[i]->clone(other.children_[i]);
          children_[i]->parent_ = this;
          children_[i]->index_ = i;
        }
      }
      index_ = other.index_;
    }

    [[nodiscard]] bool is_full() const noexcept {
      return ssize(keys_) == 2 * t - 1;
    }

    [[nodiscard]] bool can_take_key() const noexcept {
      return ssize(keys_) > t - 1;
    }

    [[nodiscard]] bool has_deficient_keys() const noexcept {
      // the root is allowed to has deficient keys
      return parent_ && ssize(keys_) < t - 1;
    }

    [[nodiscard]] bool has_overfull_keys() const noexcept {
      // the root is allowed to has deficient keys
      assert(ssize(keys_) <= 2 * t &&
             (children_.empty() || ssize(keys_) + 1 == ssize(children_)));
      return ssize(keys_) >= 2 * t;
    }
  };

  template <bool Const> struct BTreeIterator {
    using difference_type = ptrdiff_t;
    using value_type = V;
    using pointer = conditional_t<Const, const V *, V *>;
    using reference = conditional_t<Const, const V &, V &>;
    using node_type = conditional_t<Const, const Node *, Node *>;
    using iterator_category = bidirectional_iterator_tag;
    using iterator_concept = iterator_category;

    node_type node_ = nullptr;
    index_t index_;

    BTreeIterator() noexcept = default;

    BTreeIterator(node_type node, index_t i) noexcept : node_{node}, index_{i} {
      assert(node_ && i >= 0 && i <= ssize(node_->keys_));
    }

    BTreeIterator(const BTreeIterator<!Const> &other) noexcept
        : BTreeIterator(const_cast<node_type>(other.node_), other.index_) {}

    reference operator*() const noexcept { return node_->keys_[index_]; }

    pointer operator->() const noexcept { return &(node_->keys_[index_]); }

    // useful remark:
    // incrementing/decrementing iterator in an internal node will always
    // produce an iterator in a leaf node,
    // incrementing/decrementing iterator in a leaf node will always produce
    // an iterator in a leaf node for non-boundary keys,
    // an iterator in an internal node for boundary keys

    void climb() noexcept {
      while (node_->parent_ && index_ == ssize(node_->keys_)) {
        index_ = node_->index_;
        node_ = node_->parent_;
      }
    }

    void increment() noexcept {
      // we don't do past to end() check for efficiency
      if (!node_->children_.empty()) {
        node_ = leftmost_leaf(node_->children_[index_ + 1].get());
        index_ = 0;
      } else {
        ++index_;
        while (node_->parent_ && index_ == ssize(node_->keys_)) {
          index_ = node_->index_;
          node_ = node_->parent_;
        }
      }
    }

    void decrement() noexcept {
      if (!node_->children_.empty()) {
        node_ = rightmost_leaf(node_->children_[index_].get());
        index_ = ssize(node_->keys_) - 1;
      } else if (index_ > 0) {
        --index_;
      } else {
        while (node_->parent_ && node_->index_ == 0) {
          node_ = node_->parent_;
        }
        if (node_->index_ > 0) {
          index_ = node_->index_ - 1;
          node_ = node_->parent_;
        }
      }
    }

    bool verify() noexcept {
      return !node_->parent_ || (index_ < ssize(node_->keys_));
    }

    BTreeIterator &operator++() noexcept {
      increment();
      assert(verify());
      return *this;
    }

    BTreeIterator operator++(int) noexcept {
      BTreeIterator temp = *this;
      increment();
      assert(verify());
      return temp;
    }

    BTreeIterator &operator--() noexcept {
      decrement();
      assert(verify());
      return *this;
    }

    BTreeIterator operator--(int) noexcept {
      BTreeIterator temp = *this;
      decrement();
      assert(verify());
      return temp;
    }

    friend bool operator==(const BTreeIterator &x,
                           const BTreeIterator &y) noexcept {
      return x.node_ == y.node_ && x.index_ == y.index_;
    }

    friend bool operator!=(const BTreeIterator &x,
                           const BTreeIterator &y) noexcept {
      return !(x == y);
    }
  };

private:
  Alloc alloc_;
  unique_ptr<Node> root_;

public:
  using key_type = K;
  using value_type = V;
  using reference_type = V &;
  using const_reference_type = const V &;
  using node_type = Node;
  using size_type = size_t;
  using difference_type = ptrdiff_t;
  // invariant: K cannot be mutated
  // so if V is K, uses const iterator.
  // if V is pair<const K, value>, uses non-const iterator (but only value can
  // be mutated)

  // invariant: K cannot be mutated
  // so if V is K, uses const iterator.
  // if V is pair<const K, value>, uses non-const iterator (but only value can
  // be mutated)
  using nonconst_iterator_type = BTreeIterator<false>;
  using iterator_type = BTreeIterator<is_set_>;
  using const_iterator_type = BTreeIterator<true>;
  using reverse_iterator_type = reverse_iterator<iterator_type>;
  using const_reverse_iterator_type = reverse_iterator<const_iterator_type>;

  iterator_type begin_;
  size_type size_ = 0;

public:
  BTreeBase() : root_{make_unique<Node>(alloc_)}, begin_{root_.get(), 0} {}
  BTreeBase(const BTreeBase &other) {
    alloc_ = other.alloc_;
    if (other.root_) {
      root_ = make_unique<Node>(alloc_);
      root_->clone(*(other.root_));
      root_->parent_ = nullptr;
    }
    begin_ = iterator_type(leftmost_leaf(root_.get()), 0);
    size_ = other.size_;
  }
  BTreeBase &operator=(const BTreeBase &other) {
    BTreeBase tree(other);
    swap(*this, tree);
    return *this;
  }
  BTreeBase(BTreeBase &&other) noexcept = default;
  BTreeBase &operator=(BTreeBase &&other) noexcept = default;

  bool verify(const Node *node) const {
    // invariant: node never null
    assert(node);

    // invariant: except root, t - 1 <= #(key) <= 2 * t - 1
    assert(!node->parent_ ||
           (ssize(node->keys_) >= t - 1 && ssize(node->keys_) <= 2 * t - 1));

    // invariant: keys are sorted
    assert(ranges::is_sorted(node->keys_, Comp{}, Proj{}));

    // invariant: for internal nodes, t <= #(child) == (#(key) + 1)) <= 2 * t
    assert(!node->parent_ || node->children_.empty() ||
           (ssize(node->children_) >= t &&
            ssize(node->children_) == ssize(node->keys_) + 1 &&
            ssize(node->children_) <= 2 * t));

    // index check
    assert(!node->parent_ ||
           node == node->parent_->children_[node->index_].get());

    // invariant: child_0 <= key_0 <= child_1 <= ... <=  key_(N - 1) <=
    // child_N
    if (!node->children_.empty()) {
      for (index_t i = 0; i < ssize(node->keys_); ++i) {
        assert(!Comp{}(Proj{}(node->keys_[i]),
                       Proj{}(node->children_[i]->keys_.back())));
        assert(!Comp{}(Proj{}(node->children_[i + 1]->keys_.front()),
                       Proj{}(node->keys_[i])));
        // parent check
        assert(node->children_[i]->parent_ == node);
        // recursive check
        assert(verify(node->children_[i].get()));
      }
      assert(verify(node->children_.back().get()));
    }

    return true;
  }

  bool verify() const {
    assert(verify(root_.get()));
    return true;
  }

  [[nodiscard]] iterator_type begin() noexcept { return begin_; }

  [[nodiscard]] const_iterator_type begin() const noexcept {
    return const_iterator_type(begin_);
  }

  [[nodiscard]] const_iterator_type cbegin() const noexcept {
    return const_iterator_type(begin_);
  }

  [[nodiscard]] iterator_type end() noexcept {
    return iterator_type(root_.get(), ssize(root_->keys_));
  }

  [[nodiscard]] const_iterator_type end() const noexcept {
    return const_iterator_type(root_.get(), ssize(root_->keys_));
  }

  [[nodiscard]] const_iterator_type cend() const noexcept {
    return const_iterator_type(root_.get(), ssize(root_->keys_));
  }

  [[nodiscard]] bool empty() const noexcept { return size_ == 0; }

  [[nodiscard]] size_t size() const noexcept { return size_; }

  void clear() {
    root_ = make_unique<Node>(alloc_);
    begin_ = iterator_type(root_.get(), 0);
    size_ = 0;
  }

protected:
  static Node *rightmost_leaf(Node *curr) noexcept {
    while (curr && !curr->children_.empty()) {
      curr = curr->children_[ssize(curr->children_) - 1].get();
    }
    return curr;
  }

  static const Node *rightmost_leaf(const Node *curr) noexcept {
    while (curr && !curr->children_.empty()) {
      curr = curr->children_[ssize(curr->children_) - 1].get();
    }
    return curr;
  }

  static Node *leftmost_leaf(Node *curr) noexcept {
    while (curr && !curr->children_.empty()) {
      curr = curr->children_[0].get();
    }
    return curr;
  }

  static const Node *leftmost_leaf(const Node *curr) noexcept {
    while (curr && !curr->children_.empty()) {
      curr = curr->children_[0].get();
    }
    return curr;
  }

  // node brings a key from parent
  // parent brings a key from right sibling
  void left_rotate(Node *node, Node *parent, nonconst_iterator_type &iter) {
    assert(node && parent && node->parent_ == parent &&
           parent->children_[node->index_].get() == node &&
           node->index_ + 1 < ssize(parent->children_) &&
           parent->children_[node->index_ + 1]->can_take_key());
    auto sibling = parent->children_[node->index_ + 1].get();

    if (iter.node_ == sibling) {
      if (iter.index_ == 0) {
        iter.node_ = parent;
        iter.index_ = node->index_;
      } else {
        iter.index_--;
      }
    } else if (iter.node_ == parent && iter.index_ == node->index_) {
      iter.node_ = node;
      iter.index_ = ssize(node->keys_);
    }

    node->keys_.push_back(move(parent->keys_[node->index_]));
    parent->keys_[node->index_] = move(sibling->keys_.front());
    shift_left(sibling->keys_.begin(), sibling->keys_.end(), 1);
    sibling->keys_.pop_back();

    if (!node->children_.empty()) {
      sibling->children_.front()->parent_ = node;
      sibling->children_.front()->index_ = ssize(node->children_);
      node->children_.push_back(move(sibling->children_.front()));
      shift_left(sibling->children_.begin(), sibling->children_.end(), 1);
      sibling->children_.pop_back();
      for (auto &&child : sibling->children_) {
        child->index_--;
      }
    }
  }

  // node brings a key from parent
  // parent brings a key from left sibling
  void right_rotate(Node *node, Node *parent, nonconst_iterator_type &iter) {
    assert(node && parent && node->parent_ == parent &&
           parent->children_[node->index_].get() == node &&
           node->index_ - 1 >= 0 &&
           parent->children_[node->index_ - 1]->can_take_key());
    auto sibling = parent->children_[node->index_ - 1].get();

    if (iter.node_ == node) {
      iter.index_++;
    } else if (iter.node_ == sibling &&
               iter.index_ == ssize(sibling->keys_) - 1) {
      iter.node_ = parent;
      iter.index_ = node->index_ - 1;
    } else if (iter.node_ == parent && iter.index_ == node->index_ - 1) {
      iter.node_ = node;
      iter.index_ = ssize(node->keys_);
    }

    node->keys_.push_back(move(parent->keys_[node->index_ - 1]));
    rotate(node->keys_.rbegin(), node->keys_.rbegin() + 1, node->keys_.rend());
    parent->keys_[node->index_ - 1] = move(sibling->keys_.back());
    sibling->keys_.pop_back();

    if (!node->children_.empty()) {
      sibling->children_.back()->parent_ = node;
      sibling->children_.back()->index_ = 0;
      node->children_.push_back(move(sibling->children_.back()));
      sibling->children_.pop_back();
      rotate(node->children_.rbegin(), node->children_.rbegin() + 1,
             node->children_.rend());
      for (auto &&child : node->children_ | views::drop(1)) {
        child->index_++;
      }
    }
  }

  const_iterator_type search(const Node *x, const K &key) const {
    assert(x);
    auto i = distance(x->keys_.begin(),
                      ranges::lower_bound(x->keys_, key, Comp{}, Proj{}));
    if (i < ssize(x->keys_) &&
        Proj{}(key) == Proj{}(x->keys_[i])) { // equal? key found
      return const_iterator_type(x, i);
    } else if (x->children_.empty()) { // no child, key is not in the tree
      return cend();
    } else { // search on child between range
      return search(x->children_[i].get(), key);
    }
  }

  nonconst_iterator_type find_lower_bound(Node *x, const K &key) {
    auto i = distance(x->keys_.begin(),
                      ranges::lower_bound(x->keys_, key, Comp{}, Proj{}));
    if (x->children_.empty()) {
      auto it = nonconst_iterator_type(x, i);
      it.climb();
      return it;
    } else if (i < ssize(x->keys_) && Proj{}(key) == Proj{}(x->keys_[i])) {
      return nonconst_iterator_type(x, i);
    } else {
      return find_lower_bound(x->children_[i].get(), key);
    }
  }

  nonconst_iterator_type find_upper_bound(Node *x, const K &key) {
    auto i = distance(x->keys_.begin(),
                      ranges::upper_bound(x->keys_, key, Comp{}, Proj{}));
    if (i < ssize(x->keys_)) {
      return nonconst_iterator_type(x, i);
    } else if (x->children_.empty()) {
      return nonconst_iterator_type(end());
    } else {
      return find_upper_bound(x->children_[i].get(), key);
    }
  }
  
  // split child[i] to child[i], child[i + 1]
  void split_child(Node *x, index_t i, nonconst_iterator_type &iter) {
    assert(x);
    auto y = x->children_[i].get();
    assert(y && y->has_overfull_keys() && !x->has_overfull_keys());

    // split y's 2 * t keys
    // y will have left t - 1 keys
    // y->keys_[t - 1] will be a key of y->parent_
    // right t keys of y will be taken by y's right sibling

    auto z = make_unique<Node>(alloc_); // will be y's right sibling
    z->parent_ = x;
    z->index_ = i + 1;

    if (iter.node_ == y && iter.index_ > t - 1) {
      if (iter.index_ == t - 1) {
        iter.node_ = x;
        iter.index_ = i;
      } else {
        iter.node_ = z.get();
        iter.index_ -= t;
      }
    }

    // bring right t keys from y
    ranges::move(y->keys_ | views::drop(t), back_inserter(z->keys_));
    if (!y->children_.empty()) {
      // bring right half children from y
      ranges::move(y->children_ | views::drop(t), back_inserter(z->children_));
      for (auto &&child : z->children_) {
        child->parent_ = z.get();
        child->index_ -= t;
      }
      y->children_.resize(t);
    }

    x->children_.push_back(move(z));
    rotate(x->children_.rbegin(), x->children_.rbegin() + 1,
           x->children_.rend() - (i + 1));
    for (auto &&child : x->children_ | views::drop(i + 2)) {
      child->index_++;
    }

    // shift keys of x right by 1 from i
    x->keys_.push_back(move(y->keys_[t - 1]));
    rotate(x->keys_.rbegin(), x->keys_.rbegin() + 1, x->keys_.rend() - i);
    y->keys_.resize(t - 1);
  }

  // merge child[i + 1] and key[i] into child[i]
  void merge_child(Node *x, index_t i, nonconst_iterator_type &iter) {
    assert(x && !x->children_.empty() && i >= 0 &&
           i + 1 < ssize(x->children_) &&
           (x->children_[i]->has_deficient_keys() ||
            x->children_[i + 1]->has_deficient_keys()));

    index_t child_keys = ssize(x->children_[i]->keys_);

    if (iter.node_ == x->children_[i + 1].get()) {
      iter.node_ = x->children_[i].get();
      iter.index_ += child_keys + 1;
    } else if (iter.node_ == x && iter.index_ == i) {
      iter.node_ = x->children_[i].get();
      iter.index_ = child_keys;
    }

    x->children_[i]->keys_.push_back(move(x->keys_[i]));
    // bring keys of child[i + 1]
    ranges::move(x->children_[i + 1]->keys_,
                 back_inserter(x->children_[i]->keys_));
    // bring children of child[i + 1]
    if (!x->children_[i]->children_.empty()) {
      for (auto &&child : x->children_[i + 1]->children_) {
        child->parent_ = x->children_[i].get();
        child->index_ += child_keys + 1;
      }
      ranges::move(x->children_[i + 1]->children_,
                   back_inserter(x->children_[i]->children_));
    }
    // shift children from i + 1 left by 1 (because child[i + 1] is merged)
    shift_left(x->children_.begin() + i + 1, x->children_.end(), 1);
    // shift keys from i left by 1 (because key[i] is merged)
    shift_left(x->keys_.begin() + i, x->keys_.end(), 1);
    x->children_.pop_back();
    x->keys_.pop_back();
    // adjust index
    for (auto &&child : x->children_ | views::drop(i + 1)) {
      child->index_--;
    }
  }

  template <typename T>
  requires is_same_v<remove_cvref_t<T>, V>
      iterator_type insert_leaf_single(Node *node, index_t i, T &&value) {
    assert(node && node->children_.empty() && !node->has_overfull_keys());
    bool update_begin = (size_ == 0 || Comp{}(Proj{}(value), Proj{}(*begin_)));
    node->keys_.push_back(forward<T>(value));
    rotate(node->keys_.rbegin(), node->keys_.rbegin() + 1,
           node->keys_.rend() - i);
    if (update_begin) {
      assert(node == leftmost_leaf(root_.get()) && i == 0);
      begin_ = iterator_type(nonconst_iterator_type(node, i));
    }
    nonconst_iterator_type iter(node, i);
    insert_fixup_single(node, iter);
    size_++;
    assert(verify());
    return iterator_type(iter);
  }

  void insert_fixup_single(Node *node, nonconst_iterator_type &iter) {
    assert(node);
    while (node->has_overfull_keys()) {
      auto i = node->index_;
      assert(!node->parent_ || node == node->parent_->children_[i].get());

      auto parent = node->parent_;
      if (!parent) {
        assert(node == root_.get());
        // if root is full then make it as a child of the new root
        auto new_root = make_unique<Node>(alloc_);
        node->parent_ = new_root.get();
        new_root->children_.push_back(move(root_));
        root_ = move(new_root);
        // and split
        split_child(root_.get(), 0, iter);
        break;
      } else {
        split_child(parent, i, iter);
        // recursively fixup
        node = parent;
      }
    }
  }

  template <typename T>
  requires is_same_v<remove_cvref_t<T>, V>
      iterator_type insert_single(nonconst_iterator_type iter, T &&value) {
    // invariant: iter is a lower_bound for value
    auto [node, i] = iter;

    // useful remark:
    // incrementing/decrementing iterator in an internal node will always
    // produce an iterator in a leaf node,
    // incrementing/decrementing iterator in a leaf node will always produce
    // an iterator in a leaf node for non-boundary keys,
    // an iterator in an internal node for boundary keys

    if (node->children_.empty()) {
      return insert_leaf_single(node, i, forward<T>(value));
    } else {
      // pred is always leaf
      auto pred_node = prev(iter).node_;
      return insert_leaf_single(pred_node, ssize(pred_node->keys_),
                                forward<T>(value));
    }
  }

  iterator_type erase_leaf_single(Node *node, index_t i) {
    assert(node && i >= 0 && i < ssize(node->keys_) &&
           node->children_.empty() && !node->has_deficient_keys());
    nonconst_iterator_type iter(node, i);
    rotate(node->keys_.begin() + i, node->keys_.begin() + i + 1,
           node->keys_.end());
    node->keys_.pop_back();
    iter.climb();
    erase_fixup_single(node, iter);
    assert(size_ > 0);
    size_--;
    assert(verify());
    return iterator_type(iter);
  }

  void erase_fixup_single(Node *node, nonconst_iterator_type &iter) {
    while (node && node->has_deficient_keys()) {
      auto i = node->index_;
      assert(node->parent_ && node == node->parent_->children_[i].get());
      auto parent = node->parent_;
      if (i - 1 >= 0) {
        // can take from left sibling?
        if (parent->children_[i - 1]->can_take_key()) {
          right_rotate(node, parent, iter);
          assert(!node->has_deficient_keys() &&
                 !parent->children_[i - 1]->has_deficient_keys());
          break;
        } else {
          i--;
          merge_child(parent, i, iter);
          if (parent == root_.get() && parent->keys_.empty()) {
            if (iter.node_ == root_.get()) { // this was end
              assert(iter.index_ == 1);
              iter.node_ = parent->children_[i].get();
              iter.index_ = ssize(parent->children_[i]->keys_);
            }
            root_ = move(parent->children_[i]);
            root_->index_ = 0;
            root_->parent_ = nullptr;
            break;
          }
          node = parent;
        }
      } else if (i + 1 < ssize(parent->children_)) {
        // can take from right sibling?
        if (parent->children_[i + 1]->can_take_key()) {
          left_rotate(node, parent, iter);
          assert(!node->has_deficient_keys() &&
                 !parent->children_[i + 1]->has_deficient_keys());
          break;
        } else {
          merge_child(parent, i, iter);
          if (parent == root_.get() && parent->keys_.empty()) {
            if (iter.node_ == root_.get()) { // this was end
              assert(iter.index_ == 1);
              iter.node_ = parent->children_[i].get();
              iter.index_ = ssize(parent->children_[i]->keys_);
            }
            root_ = move(parent->children_[i]);
            root_->index_ = 0;
            root_->parent_ = nullptr;
            break;
          }
          node = parent;
        }
      }
    }
  }

  size_t erase_range(nonconst_iterator_type first,
                     nonconst_iterator_type last) {
    // TODO
    return 0;
  }

  iterator_type erase_single(nonconst_iterator_type iter) {
    auto [node, i] = iter;

    // useful remark:
    // incrementing/decrementing iterator in an internal node will always
    // produce an iterator in a leaf node,
    // incrementing/decrementing iterator in a leaf node will always produce
    // an iterator in a leaf node for non-boundary keys,
    // an iterator in an internal node for boundary keys

    if (node->children_.empty()) {
      bool update_begin = (begin_ == iterator_type(iter));
      auto res = erase_leaf_single(node, i);
      if (update_begin) {
        begin_ = res;
      }
      return res;
    } else {
      // this iter is never begin() because iter's node is not a leaf node
      assert(begin_ != iterator_type(iter));
      // and pred is always leaf.
      auto pred = prev(iter);
      iter_swap(pred, iter);
      return erase_leaf_single(pred.node_, pred.index_);
    }
  }

public:
  iterator_type find(const K &key) {
    return iterator_type(search(root_.get(), key));
  }

  const_iterator_type find(const K &key) const {
    return const_iterator_type(search(root_.get(), key));
  }

  bool contains(const K &key) const {
    return search(root_.get(), key) != cend();
  }

protected:
  template <typename T>
  conditional_t<AllowDup, iterator_type, pair<iterator_type, bool>>
  insert_value(T &&key) requires is_same_v<remove_cvref_t<T>, V> {
    nonconst_iterator_type it;
    if constexpr (AllowDup) {
      it = find_upper_bound(root_.get(), Proj{}(key));
    } else {
      it = find_lower_bound(root_.get(), Proj{}(key));
      if (it != nonconst_iterator_type(end()) && Proj{}(key) == Proj{}(*it)) {
        return {iterator_type(it), false};
      }
    }

    auto res = insert_single(it, forward<T>(key));
    if constexpr (AllowDup) {
      return res;
    } else {
      return {res, true};
    }
  }

public:
  conditional_t<AllowDup, iterator_type, pair<iterator_type, bool>>
  insert(const V &key) {
    return insert_value(key);
  }

  conditional_t<AllowDup, iterator_type, pair<iterator_type, bool>>
  insert(V &&key) {
    return insert_value(move(key));
  }

  template <typename T>
  auto &operator[](T &&raw_key) requires(!is_set_ && !AllowDup) {
    K key{forward<T>(raw_key)};
    auto it = find_lower_bound(root_.get(), key);
    if (Proj{}(key) == Proj{}(*it)) {
      return it->key_.second;
    }

    V val{move(key), {}};
    it = insert_single(it, forward<T>(val));
    return it->key_.second;
  }

protected:
  size_t erase_key(const K &key) {
    if constexpr (AllowDup) {
      return erase_range(find_lower_bound(root_.get(), key),
                         find_upper_bound(root_.get(), key));
    } else {
      auto it = find_lower_bound(root_.get(), Proj{}(key));
      if (Proj{}(key) == Proj{}(*it)) {
        erase_single(it);
        return 1;
      } else {
        return 0;
      }
    }
  }

public:
  iterator_type erase(iterator_type iter) {
    if (iter == end()) {
      throw invalid_argument("attempt to erase end()");
    }
    return erase_single(iter);
  }

  size_t erase(const K &key) { return erase_key(key); }
};

} // namespace detail

template <Containable K, index_t t = 2, typename Comp = ranges::less,
          typename Alloc = allocator<K>>
using BTreeSet = detail::BTreeBase<K, K, t, Comp, false, Alloc>;

template <Containable K, index_t t = 2, typename Comp = ranges::less,
          typename Alloc = allocator<K>>
using BTreeMultiSet = detail::BTreeBase<K, K, t, Comp, true, Alloc>;

template <Containable K, Containable V, index_t t = 2,
          typename Comp = ranges::less,
          typename Alloc = allocator<pair<const K, V>>>
using BTreeMap = detail::BTreeBase<K, pair<const K, V>, t, Comp, false, Alloc>;

template <Containable K, Containable V, index_t t = 2,
          typename Comp = ranges::less,
          typename Alloc = allocator<pair<const K, V>>>
using BTreeMultiMap =
    detail::BTreeBase<K, pair<const K, V>, t, Comp, true, Alloc>;

} // namespace frozenca

#endif //__CLRS4_BTREE_H__
