#ifndef __CLRS4_BTREE_H__
#define __CLRS4_BTREE_H__

#include <algorithm>
#include <cassert>
#include <common.h>
#include <ranges>
#include <vector>

namespace frozenca {

using namespace std;

namespace detail {

template <typename T> struct TreePairRef { using type = T; };

template <typename T, typename U> struct TreePairRef<pair<T, U>> {
  using type = pair<const T &, U &>;
};

template <typename TreePair> using PairRefType = TreePairRef<TreePair>::type;

template <Containable K, typename V, index_t Fanout, index_t FanoutLeaf,
          typename Comp, bool AllowDup, typename Alloc>
requires(Fanout >= 2 && FanoutLeaf >= 2) class BTreeBase {
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
    explicit Node(const Alloc &alloc, bool is_leaf = true) : keys_(alloc) {
      keys_.reserve(2 * (is_leaf ? FanoutLeaf : Fanout) - 1);
    }

    Node(const Node &node) = delete;
    Node &operator=(const Node &node) = delete;
    Node(Node &&node) = delete;
    Node &operator=(Node &&node) = delete;

    void clone(const Node &other) {
      keys_ = other.keys_;
      index_ = other.index_;
      if (!other.is_leaf()) {
        assert(is_leaf());
        children_.resize(other.children_.size());
        for (index_t i = 0; i < ssize(other.children_); ++i) {
          children_[i] =
              make_unique<Node>(alloc_, other.children_[i]->is_leaf());
          children_[i]->clone(other.children_[i]);
          children_[i]->parent_ = this;
          children_[i]->index_ = i;
        }
      }
    }

    [[nodiscard]] index_t fanout() const noexcept {
      if (is_leaf()) {
        return FanoutLeaf;
      } else {
        return Fanout;
      }
    }

    [[nodiscard]] bool is_leaf() const noexcept { return children_.empty(); }

    [[nodiscard]] bool is_full() const noexcept {
      return ssize(keys_) == 2 * fanout() - 1;
    }

    [[nodiscard]] bool can_take_key() const noexcept {
      return ssize(keys_) > fanout() - 1;
    }

    [[nodiscard]] bool has_minimal_keys() const noexcept {
      return parent_ && ssize(keys_) == fanout() - 1;
    }
  };

  struct BTreeNonConstIterTraits {
    using difference_type = ptrdiff_t;
    using value_type = V;
    using pointer = V *;
    using reference = V &;
    using iterator_category = bidirectional_iterator_tag;
    using iterator_concept = iterator_category;

    static reference make_ref(value_type &val) noexcept { return ref(val); }
  };

  struct BTreeConstIterTraits {
    using difference_type = ptrdiff_t;
    using value_type = V;
    using pointer = const V *;
    using reference = const V &;
    using iterator_category = bidirectional_iterator_tag;
    using iterator_concept = iterator_category;

    static reference make_ref(const value_type &val) noexcept {
      return cref(val);
    }
  };

  struct BTreeRefIterTraits {
    using difference_type = ptrdiff_t;
    using value_type = V;
    using pointer = V *;
    using reference = PairRefType<V>;
    using iterator_category = bidirectional_iterator_tag;
    using iterator_concept = iterator_category;

    static reference make_ref(value_type &val) noexcept {
      return {cref(val.first), ref(val.second)};
    }
  };

  template <typename IterTraits> struct BTreeIterator {
    using difference_type = IterTraits::difference_type;
    using value_type = IterTraits::value_type;
    using pointer = IterTraits::pointer;
    using reference = IterTraits::reference;
    using iterator_category = IterTraits::iterator_category;
    using iterator_concept = IterTraits::iterator_concept;

    Node *node_ = nullptr;
    index_t index_;

    BTreeIterator() noexcept = default;

    BTreeIterator(Node *node, index_t i) noexcept : node_{node}, index_{i} {
      assert(node_ && i >= 0 && i <= ssize(node_->keys_));
    }

    template <typename IterTraitsOther>
    BTreeIterator(const BTreeIterator<IterTraitsOther> &other) noexcept
        : BTreeIterator(other.node_, other.index_) {}

    reference operator*() const noexcept {
      return IterTraits::make_ref(node_->keys_[index_]);
    }

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
      if (!node_->is_leaf()) {
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
      if (!node_->is_leaf()) {
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
  using reference_type = conditional_t<is_set_, const V &, PairRefType<V>>;
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
  using nonconst_iterator_type = BTreeIterator<BTreeNonConstIterTraits>;
  using iterator_type = BTreeIterator<
      conditional_t<is_set_, BTreeConstIterTraits, BTreeRefIterTraits>>;
  using const_iterator_type = BTreeIterator<BTreeConstIterTraits>;
  using reverse_iterator_type = reverse_iterator<iterator_type>;
  using const_reverse_iterator_type = reverse_iterator<const_iterator_type>;

  iterator_type begin_;
  size_type size_ = 0;

public:
  BTreeBase() : root_{make_unique<Node>(alloc_)}, begin_{root_.get(), 0} {}
  BTreeBase(const BTreeBase &other) {
    alloc_ = other.alloc_;
    if (other.root_) {
      root_ = make_unique<Node>(alloc_, other.root_.is_leaf());
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
    assert(!node->parent_ || (ssize(node->keys_) >= node->fanout() - 1 &&
                              ssize(node->keys_) <= 2 * node->fanout() - 1));

    // invariant: keys are sorted
    assert(ranges::is_sorted(node->keys_, Comp{}, Proj{}));

    // invariant: for internal nodes, t <= #(child) == (#(key) + 1)) <= 2 * t
    assert(!node->parent_ || node->is_leaf() ||
           (ssize(node->children_) >= node->fanout() &&
            ssize(node->children_) == ssize(node->keys_) + 1 &&
            ssize(node->children_) <= 2 * node->fanout()));

    // index check
    assert(!node->parent_ ||
           node == node->parent_->children_[node->index_].get());

    // invariant: child_0 <= key_0 <= child_1 <= ... <=  key_(N - 1) <=
    // child_N
    if (!node->is_leaf()) {
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
    while (curr && !curr->is_leaf()) {
      curr = curr->children_[ssize(curr->children_) - 1].get();
    }
    return curr;
  }

  static const Node *rightmost_leaf(const Node *curr) noexcept {
    while (curr && !curr->is_leaf()) {
      curr = curr->children_[ssize(curr->children_) - 1].get();
    }
    return curr;
  }

  static Node *leftmost_leaf(Node *curr) noexcept {
    while (curr && !curr->is_leaf()) {
      curr = curr->children_[0].get();
    }
    return curr;
  }

  static const Node *leftmost_leaf(const Node *curr) noexcept {
    while (curr && !curr->is_leaf()) {
      curr = curr->children_[0].get();
    }
    return curr;
  }

  // node brings a key from parent
  // parent brings a key from right sibling
  // node brings a child from right sibling
  void left_rotate(Node *node, Node *parent) {
    assert(node && parent && node->parent_ == parent &&
           parent->children_[node->index_].get() == node &&
           node->index_ + 1 < ssize(parent->children_) &&
           parent->children_[node->index_ + 1]->can_take_key());
    auto sibling = parent->children_[node->index_ + 1].get();

    node->keys_.push_back(move(parent->keys_[node->index_]));
    parent->keys_[node->index_] = move(sibling->keys_.front());
    shift_left(sibling->keys_.begin(), sibling->keys_.end(), 1);
    sibling->keys_.pop_back();

    if (!node->is_leaf()) {
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
  // node brings a child from left sibling
  void right_rotate(Node *node, Node *parent) {
    assert(node && parent && node->parent_ == parent &&
           parent->children_[node->index_].get() == node &&
           node->index_ - 1 >= 0 &&
           parent->children_[node->index_ - 1]->can_take_key());
    auto sibling = parent->children_[node->index_ - 1].get();

    node->keys_.emplace_back();
    shift_right(node->keys_.begin(), node->keys_.end(), 1);
    node->keys_.front() = move(parent->keys_[node->index_ - 1]);
    parent->keys_[node->index_ - 1] = move(sibling->keys_.back());
    sibling->keys_.pop_back();

    if (!node->is_leaf()) {
      node->children_.emplace_back();
      shift_right(node->children_.begin(), node->children_.end(), 1);
      sibling->children_.back()->parent_ = node;
      sibling->children_.back()->index_ = 0;
      node->children_.front() = move(sibling->children_.back());
      sibling->children_.pop_back();
      for (auto &&child : node->children_ | views::drop(1)) {
        child->index_++;
      }
    }
  }

  const_iterator_type search(const K &key) const {
    auto x = root_.get();
    while (x) {
      auto i = distance(x->keys_.begin(),
                        ranges::lower_bound(x->keys_, key, Comp{}, Proj{}));
      if (i < ssize(x->keys_) &&
          Proj{}(key) == Proj{}(x->keys_[i])) { // equal? key found
        return const_iterator_type(x, i);
      } else if (x->is_leaf()) { // no child, key is not in the tree
        return cend();
      } else { // search on child between range
        x = x->children_[i].get();
      }
    }
    return cend();
  }

  nonconst_iterator_type find_lower_bound(const K &key) {
    auto x = root_.get();
    while (x) {
      auto i = distance(x->keys_.begin(),
                        ranges::lower_bound(x->keys_, key, Comp{}, Proj{}));
      if (x->is_leaf()) {
        auto it = nonconst_iterator_type(x, i);
        it.climb();
        return it;
      } else {
        x = x->children_[i].get();
      }
    }
    return nonconst_iterator_type(end());
  }

  nonconst_iterator_type find_upper_bound(const K &key) {
    auto x = root_.get();
    while (x) {
      auto i = distance(x->keys_.begin(),
                        ranges::upper_bound(x->keys_, key, Comp{}, Proj{}));
      if (x->is_leaf()) {
        auto it = nonconst_iterator_type(x, i);
        it.climb();
        return it;
      } else {
        x = x->children_[i].get();
      }
    }
    return nonconst_iterator_type(end());
  }

  // split child[i] to child[i], child[i + 1]
  void split_child(Node *x, index_t i) {
    assert(x);
    auto y = x->children_[i].get();
    assert(y && y->is_full() && !x->is_full());

    // split y's 2 * t keys
    // y will have left t - 1 keys
    // y->keys_[t - 1] will be a key of y->parent_
    // right t keys of y will be taken by y's right sibling

    auto z =
        make_unique<Node>(alloc_, y->is_leaf()); // will be y's right sibling
    z->parent_ = x;
    z->index_ = i + 1;

    auto fanout = y->fanout();

    // bring right t keys from y
    ranges::move(y->keys_ | views::drop(fanout), back_inserter(z->keys_));
    if (!y->is_leaf()) {
      // bring right half children from y
      ranges::move(y->children_ | views::drop(fanout),
                   back_inserter(z->children_));
      for (auto &&child : z->children_) {
        child->parent_ = z.get();
        child->index_ -= fanout;
      }
      y->children_.resize(fanout);
    }

    x->children_.emplace_back();
    shift_right(x->children_.begin() + i + 1, x->children_.end(), 1);
    x->children_[i + 1] = move(z);
    for (auto &&child : x->children_ | views::drop(i + 2)) {
      child->index_++;
    }

    x->keys_.emplace_back();
    shift_right(x->keys_.begin() + i, x->keys_.end(), 1);
    x->keys_[i] = move(y->keys_[fanout - 1]);
    y->keys_.resize(fanout - 1);
  }

  // merge child[i + 1] and key[i] into child[i]
  void merge_child(Node *x, index_t i) {
    assert(x && !x->is_leaf() && i >= 0 && i + 1 < ssize(x->children_) &&
           x->children_[i]->has_minimal_keys() &&
           x->children_[i + 1]->has_minimal_keys());

    x->children_[i]->keys_.push_back(move(x->keys_[i]));
    // bring keys of child[i + 1]
    ranges::move(x->children_[i + 1]->keys_,
                 back_inserter(x->children_[i]->keys_));

    auto fanout = x->children_[i]->fanout();

    // bring children of child[i + 1]
    if (!x->children_[i]->is_leaf()) {
      for (auto &&child : x->children_[i + 1]->children_) {
        child->parent_ = x->children_[i].get();
        child->index_ += fanout;
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
  iterator_type
  insert_leaf(Node *node, index_t i,
              T &&value) requires is_same_v<remove_cvref_t<T>, V> {
    assert(node && node->is_leaf() && !node->is_full());
    bool update_begin = (size_ == 0 || Comp{}(Proj{}(value), Proj{}(*begin_)));
    node->keys_.emplace_back();
    shift_right(node->keys_.begin() + i, node->keys_.end(), 1);
    node->keys_[i] = forward<T>(value);
    iterator_type iter(node, i);
    if (update_begin) {
      assert(node == leftmost_leaf(root_.get()) && i == 0);
      begin_ = iter;
    }
    size_++;
    assert(verify());
    return iter;
  }

  template <typename T>
  iterator_type
  insert_ub(T &&key) requires(AllowDup &&is_same_v<remove_cvref_t<T>, V>) {
    auto x = root_.get();
    while (true) {
      auto i =
          distance(x->keys_.begin(),
                   ranges::upper_bound(x->keys_, Proj{}(key), Comp{}, Proj{}));
      if (x->is_leaf()) {
        return insert_leaf(x, i, forward<T>(key));
      } else {
        if (x->children_[i]->is_full()) {
          split_child(x, i);
          if (Comp{}(Proj{}(x->keys_[i]), Proj{}(key))) {
            ++i;
          }
        }
        x = x->children_[i].get();
      }
    }
  }

  template <typename T>
  pair<iterator_type, bool>
  insert_lb(T &&key) requires(!AllowDup && is_same_v<remove_cvref_t<T>, V>) {
    auto x = root_.get();
    while (true) {
      auto i =
          distance(x->keys_.begin(),
                   ranges::lower_bound(x->keys_, Proj{}(key), Comp{}, Proj{}));
      if (i < ssize(x->keys_) && Proj{}(key) == Proj{}(x->keys_[i])) {
        return {iterator_type(x, i), false};
      } else if (x->is_leaf()) {
        return {insert_leaf(x, i, forward<T>(key)), true};
      } else {
        if (x->children_[i]->is_full()) {
          split_child(x, i);
          if (Comp{}(Proj{}(x->keys_[i]), Proj{}(key))) {
            ++i;
          }
        }
        x = x->children_[i].get();
      }
    }
  }

  iterator_type erase_leaf(Node *node, index_t i) {
    assert(node && i >= 0 && i < ssize(node->keys_) && node->is_leaf() &&
           !node->has_minimal_keys());
    bool update_begin = (begin_ == iterator_type(node, i));
    shift_left(node->keys_.begin() + i, node->keys_.end(), 1);
    node->keys_.pop_back();
    iterator_type iter(node, i);
    iter.climb();
    if (update_begin) {
      begin_ = iter;
    }
    assert(size_ > 0);
    size_--;
    assert(verify());
    return iter;
  }

  size_t erase_lb(Node *x, const K &key) requires(!AllowDup) {
    while (true) {
      auto i = distance(x->keys_.begin(),
                        ranges::lower_bound(x->keys_, key, Comp{}, Proj{}));
      if (i < ssize(x->keys_) && key == Proj{}(x->keys_[i])) {
        // key found
        assert(x->is_leaf() || i + 1 < ssize(x->children_));
        if (x->is_leaf()) {
          erase_leaf(x, i);
          return 1;
        } else if (x->children_[i]->can_take_key()) {
          // swap key with pred
          nonconst_iterator_type iter(x, i);
          auto pred = prev(iter);
          assert(pred.node_ == rightmost_leaf(x->children_[i].get()));
          iter_swap(pred, iter);
          // search pred
          x = x->children_[i].get();
        } else if (x->children_[i + 1]->can_take_key()) {
          // swap key with succ
          nonconst_iterator_type iter(x, i);
          auto succ = next(iter);
          assert(succ.node_ == leftmost_leaf(x->children_[i + 1].get()));
          iter_swap(succ, iter);
          // search succ
          x = x->children_[i + 1].get();
        } else {
          merge_child(x, i);
          auto next = x->children_[i].get();
          if (x == root_.get() && x->keys_.empty()) {
            // root is empty, promote the merged child to new root
            root_ = move(x->children_[i]);
            root_->index_ = 0;
            root_->parent_ = nullptr;
          }
          x = next;
        }
      } else if (x->is_leaf()) {
        // no child, key is not in the tree
        return 0;
      } else {
        auto next = x->children_[i].get();
        if (x->children_[i]->has_minimal_keys()) {
          if (i + 1 < ssize(x->children_) &&
              x->children_[i + 1]->can_take_key()) {
            left_rotate(x->children_[i].get(), x);
          } else if (i - 1 >= 0 && x->children_[i - 1]->can_take_key()) {
            right_rotate(x->children_[i].get(), x);
          } else if (i + 1 < ssize(x->children_)) {
            merge_child(x, i);
            if (x == root_.get() && x->keys_.empty()) {
              // root is empty, promote the merged child to new root
              root_ = move(x->children_[i]);
              root_->index_ = 0;
              root_->parent_ = nullptr;
            }
          } else if (i - 1 >= 0) {
            merge_child(x, i - 1);
            next = x->children_[i - 1].get();
            if (x == root_.get() && x->keys_.empty()) {
              // root is empty, promote the merged child to new root
              root_ = move(x->children_[i - 1]);
              root_->index_ = 0;
              root_->parent_ = nullptr;
            }
          }
        }
        x = next;
      }
    }
  }

  iterator_type erase_hint(const V &value, vector<index_t> &hints) {
    auto x = root_.get();
    while (true) {
      auto i = hints.back();
      hints.pop_back();
      if (hints.empty()) {
        // key found
        assert(i < ssize(x->keys_) && value == x->keys_[i]);
        assert(x->is_leaf() || i + 1 < ssize(x->children_));
        if (x->is_leaf()) {
          return erase_leaf(x, i);
        } else if (x->children_[i]->can_take_key()) {
          // swap key with pred
          nonconst_iterator_type iter(x, i);
          auto pred = prev(iter);
          assert(pred.node_ == rightmost_leaf(x->children_[i].get()));
          iter_swap(pred, iter);
          // search pred
          x = x->children_[i].get();
          auto curr = x;
          assert(curr->index_ == i);
          while (!curr->is_leaf()) {
            hints.push_back(ssize(curr->children_) - 1);
            curr = curr->children_.back().get();
          }
          hints.push_back(ssize(curr->keys_) - 1);
          ranges::reverse(hints);
        } else if (x->children_[i + 1]->can_take_key()) {
          // swap key with succ
          nonconst_iterator_type iter(x, i);
          auto succ = next(iter);
          assert(succ.node_ == leftmost_leaf(x->children_[i + 1].get()));
          iter_swap(succ, iter);
          // search succ
          x = x->children_[i + 1].get();
          auto curr = x;
          assert(curr->index_ == i + 1);
          while (!curr->is_leaf()) {
            hints.push_back(0);
            curr = curr->children_.front().get();
          }
          hints.push_back(0);
        } else {
          merge_child(x, i);
          auto next = x->children_[i].get();
          if (x == root_.get() && x->keys_.empty()) {
            // root is empty, promote the merged child to new root
            root_ = move(x->children_[i]);
            root_->index_ = 0;
            root_->parent_ = nullptr;
          }
          x = next;
          // i'th key of x is now t - 1'th key of x->children_[i]
          hints.push_back(x->fanout() - 1);
        }
      } else {
        assert(!hints.empty());
        auto next = x->children_[i].get();
        if (x->children_[i]->has_minimal_keys()) {
          if (i + 1 < ssize(x->children_) &&
              x->children_[i + 1]->can_take_key()) {
            left_rotate(x->children_[i].get(), x);
          } else if (i - 1 >= 0 && x->children_[i - 1]->can_take_key()) {
            right_rotate(x->children_[i].get(), x);
            // x->children_[i] stuffs are shifted right by 1
            hints.back() += 1;
          } else if (i + 1 < ssize(x->children_)) {
            merge_child(x, i);
            if (x == root_.get() && x->keys_.empty()) {
              // root is empty, promote the merged child to new root
              root_ = move(x->children_[i]);
              root_->index_ = 0;
              root_->parent_ = nullptr;
            }
          } else if (i - 1 >= 0) {
            merge_child(x, i - 1);
            next = x->children_[i - 1].get();
            if (x == root_.get() && x->keys_.empty()) {
              // root is empty, promote the merged child to new root
              root_ = move(x->children_[i - 1]);
              root_->index_ = 0;
              root_->parent_ = nullptr;
            }
            // x->children_[i] stuffs are shifted right by t
            hints.back() += next->fanout();
          }
        }
        x = next;
      }
    }
  }

  size_t erase_range(iterator_type first, iterator_type last) {
    if (first == begin_ && last == end()) {
      auto cnt = size_;
      clear();
      return cnt;
    }
    auto cnt = distance(first, last);
    for (auto i = cnt; i >= 0; --i) {
      first = erase(first);
    }
    return cnt;
  }

public:
  iterator_type find(const K &key) {
    return iterator_type(search(key));
  }

  const_iterator_type find(const K &key) const {
    return search(key);
  }

  bool contains(const K &key) const {
    return search(key) != cend();
  }

  iterator_type lower_bound(const K &key) {
    return iterator_type(find_lower_bound(key));
  }

  const_iterator_type lower_bound(const K &key) const {
    return const_iterator_type(find_lower_bound(key));
  }

  iterator_type upper_bound(const K &key) {
    return iterator_type(find_upper_bound(key));
  }

  const_iterator_type upper_bound(const K &key) const {
    return const_iterator_type(find_upper_bound(key));
  }

  ranges::subrange<iterator_type> equal_range(const K &key) {
    return {iterator_type(find_lower_bound(key)),
            iterator_type(find_upper_bound(key))};
  }

  ranges::subrange<const_iterator_type> equal_range(const K &key) const {
    return {const_iterator_type(find_lower_bound(key)),
            const_iterator_type(find_upper_bound(key))};
  }

protected:
  template <typename T>
  conditional_t<AllowDup, iterator_type, pair<iterator_type, bool>>
  insert_value(T &&key) requires(is_same_v<remove_cvref_t<T>, V>) {
    if (root_->is_full()) {
      // if root is full then make it as a child of the new root
      auto new_root = make_unique<Node>(alloc_, false);
      root_->parent_ = new_root.get();
      new_root->children_.push_back(move(root_));
      root_ = move(new_root);
      // and split
      split_child(root_.get(), 0);
    }
    if constexpr (AllowDup) {
      return insert_ub(forward<T>(key));
    } else {
      return insert_lb(forward<T>(key));
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
    if (root_->is_full()) {
      // if root is full then make it as a child of the new root
      auto new_root = make_unique<Node>(alloc_, false);
      root_->parent_ = new_root.get();
      new_root->children_.push_back(move(root_));
      root_ = move(new_root);
      // and split
      split_child(root_.get(), 0);
    }

    K key{forward<T>(raw_key)};
    auto x = root_.get();
    while (true) {
      auto i = distance(x->keys_.begin(),
                        ranges::lower_bound(x->keys_, key, Comp{}, Proj{}));
      if (i < ssize(x->keys_) && key == Proj{}(x->keys_[i])) {
        return iterator_type(x, i)->second;
      } else if (x->is_leaf()) {
        V val{move(key), {}};
        return insert_leaf(x, i, forward<V>(val))->second;
      } else {
        if (x->children_[i]->is_full()) {
          split_child(x, i);
          if (Comp{}(Proj{}(x->keys_[i]), key)) {
            ++i;
          }
        }
        x = x->children_[i].get();
      }
    }
  }

public:
  iterator_type erase(iterator_type iter) {
    if (iter == end()) {
      throw invalid_argument("attempt to erase end()");
    }
    auto node = iter.node_;
    vector<index_t> hints;
    hints.push_back(iter.index_);
    while (node && node->parent_) {
      hints.push_back(node->index_);
      node = node->parent_;
    }
    V value(move(*iter));
    return erase_hint(value, hints);
  }

  size_t erase(const K &key) {
    if constexpr (AllowDup) {
      return erase_range(iterator_type(find_lower_bound(key)),
                         iterator_type(find_upper_bound(key)));
    } else {
      return erase_lb(root_.get(), key);
    }
  }
};

} // namespace detail

template <Containable K, index_t t = 2, index_t t_leaf = 2,
          typename Comp = ranges::less, typename Alloc = allocator<K>>
using BTreeSet = detail::BTreeBase<K, K, t, t_leaf, Comp, false, Alloc>;

template <Containable K, index_t t = 2, index_t t_leaf = 2,
          typename Comp = ranges::less, typename Alloc = allocator<K>>
using BTreeMultiSet = detail::BTreeBase<K, K, t, t_leaf, Comp, true, Alloc>;

template <Containable K, Containable V, index_t t = 2, index_t t_leaf = 2,
          typename Comp = ranges::less, typename Alloc = allocator<pair<K, V>>>
using BTreeMap =
    detail::BTreeBase<K, pair<K, V>, t, t_leaf, Comp, false, Alloc>;

template <Containable K, Containable V, index_t t = 2, index_t t_leaf = 2,
          typename Comp = ranges::less, typename Alloc = allocator<pair<K, V>>>
using BTreeMultiMap =
    detail::BTreeBase<K, pair<K, V>, t, t_leaf, Comp, true, Alloc>;

} // namespace frozenca

#endif //__CLRS4_BTREE_H__
