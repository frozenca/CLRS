#ifndef __CLRS4_RED_BLACK_TREE_H__
#define __CLRS4_RED_BLACK_TREE_H__

#include <cassert>
#include <common.h>
#include <iostream>
#include <random>
#include <stdexcept>

namespace frozenca::hard {

using namespace std;

namespace detail {

template <Containable T, typename Derived> struct RBTreeNodeBase {
  T key_ = T{};
  Derived *parent_ = nullptr;
  bool black_ = false; // the new node starts out red
  unique_ptr<Derived> left_;
  unique_ptr<Derived> right_;
  RBTreeNodeBase() = default;
  RBTreeNodeBase(T key) : key_{move(key)} {}

  void clone(const RBTreeNodeBase &other) {
    key_ = other.key_;
    black_ = other.black_;
    if (other.left_) {
      left_ = make_unique<RBTreeNodeBase>();
      left_->clone(*other.left_);
      left_->parent_ = this;
    }
    if (other.right_) {
      right_ = make_unique<RBTreeNodeBase>();
      right_->clone(*other.right_);
      right_->parent_ = this;
    }
  }

  RBTreeNodeBase(const RBTreeNodeBase &node) = delete;
  RBTreeNodeBase &operator=(const RBTreeNodeBase &node) = delete;
  RBTreeNodeBase(RBTreeNodeBase &&node) = delete;
  RBTreeNodeBase &operator=(RBTreeNodeBase &&node) = delete;

  Derived &derived() { return static_cast<Derived &>(*this); }

  const Derived &derived() const { return static_cast<const Derived &>(*this); }

  friend istream &operator>>(istream &is, unique_ptr<RBTreeNodeBase> &node) {
    string val;
    is >> val;
    if (val == "#") {
      return is;
    }
    istringstream istr{val};
    T v;
    if (!(istr >> v)) {
      return is;
    }
    node = make_unique<RBTreeNodeBase>(v);
    is >> node->left_;
    if (node->left_) {
      node->left_->parent_ = node.get();
    }
    is >> node->right_;
    if (node->right_) {
      node->right_->parent_ = node.get();
    }
    return is;
  }

  friend ostream &operator<<(ostream &os, const RBTreeNodeBase *node) {
    if (node) {
      os << node->key_ << ' ';
      os << node->left_.get();
      os << node->right_.get();
    } else {
      os << "# ";
    }
    return os;
  }

  friend void inorder_print(ostream &os, const RBTreeNodeBase *node) {
    if (node) {
      inorder_print(os, node->left_.get());
      os << node->key_ << ' ';
      inorder_print(os, node->right_.get());
    }
  }
};

template <Containable T>
struct RBTreeNode : public RBTreeNodeBase<T, RBTreeNode<T>> {};

template <Containable T, bool Const, typename Node> class BSTIterator {
public:
  using difference_type = ptrdiff_t;
  using value_type = T;
  using pointer = conditional_t<Const, const T *, T *>;
  using reference = conditional_t<Const, const T &, T &>;
  using iterator_category = bidirectional_iterator_tag;
  using iterator_concept = iterator_category;

public:
  Node *node_ = nullptr;

  BSTIterator() = default;
  BSTIterator(Node *node) : node_{node} {}

  BSTIterator(const BSTIterator<T, !Const, Node> &other) : node_{other.node_} {}

  BSTIterator(const BSTIterator &other) = default;
  BSTIterator &operator=(const BSTIterator &other) = default;
  BSTIterator(BSTIterator &&other) noexcept = default;
  BSTIterator &operator=(BSTIterator &&other) noexcept = default;

  reference operator*() const noexcept { return node_->key_; }

  pointer operator->() const noexcept { return &(node_->key_); }

  void increment() {
    if (node_ && node_->right_) {
      auto curr = node_->right_.get();
      while (curr->left_) {
        curr = curr->left_.get();
      }
      node_ = curr;
    } else {
      auto parent = node_->parent_;
      while (parent && node_ == parent->right_.get()) {
        node_ = parent;
        parent = parent->parent_;
      }
      node_ = parent;
    }
  }

  void decrement() {
    if (node_ && node_->left_) {
      auto curr = node_->left_.get();
      while (curr->right_) {
        curr = curr->right_.get();
      }
      node_ = curr;
    } else {
      auto parent = node_->parent_;
      while (parent && node_ == parent->left_.get()) {
        node_ = parent;
        parent = parent->parent_;
      }
    }
  }

  BSTIterator &operator++() {
    increment();
    return *this;
  }

  BSTIterator operator++(int) {
    BSTIterator temp = *this;
    increment();
    return temp;
  }

  BSTIterator &operator--() {
    decrement();
    return *this;
  }

  BSTIterator operator--(int) {
    BSTIterator temp = *this;
    decrement();
    return temp;
  }

  [[nodiscard]] friend bool operator==(const BSTIterator &lhs,
                                       const BSTIterator &rhs) {
    return lhs.node_ == rhs.node_;
  }
};

enum class BSTChild {
  Unused,
  Left,
  Right,
};

template <typename Node> struct BSTSearchResult {
  Node *bound_ = nullptr;
  Node *parent_ = nullptr;
  BSTChild where_ = BSTChild::Unused;
};

template <Containable K, typename V, typename Comp, bool AllowDup,
          typename Node, typename Derived>
requires invocable<Comp, K, K>
class RedBlackTreeBase;

template <Containable K, typename V, typename Comp, bool AllowDup,
          typename Node, typename Derived, typename T>
RedBlackTreeBase<K, V, Comp, AllowDup, Node, Derived>
join(RedBlackTreeBase<K, V, Comp, AllowDup, Node, Derived> &&tree1,
     T &&raw_value,
     RedBlackTreeBase<K, V, Comp, AllowDup, Node, Derived> &&tree2) requires
    is_constructible_v<V, remove_cvref_t<T>>;

template <Containable K, typename V, typename Comp, bool AllowDup,
          typename Node, typename Derived>
requires invocable<Comp, K, K>
class RedBlackTreeBase {
  // invariant: V is either K or pair<const K, Value> for some Value type.
  static constexpr bool is_set_ = is_same_v<K, V>;

public:
  using SearchResult = BSTSearchResult<Node>;

private:
  unique_ptr<Node> root_;

  static int verify(const Node *node) {
    if (!node) {
      return 0;
    }

    auto left_black_height = verify(node->left_.get());
    auto right_black_height = verify(node->right_.get());

    if (node->left_ && node->left_->black_) {
      left_black_height++;
    }
    if (node->right_ && node->right_->black_) {
      right_black_height++;
    }

    // binary search tree properties
    assert(!node->left_ ||
           (node->left_->parent_ == node &&
            !Comp{}(proj(node->key_), proj(node->left_->key_))));
    assert(!node->right_ ||
           (node->right_->parent_ == node &&
            !Comp{}(proj(node->right_->key_), proj(node->key_))));

    // if a node is red, then both its children are black.
    assert(node->black_ || ((!node->left_ || node->left_->black_) &&
                            (!node->right_ || node->right_->black_)));

    // for each node, all simple paths from the node to
    // descendant leaves contain the same number of black nodes.
    assert(left_black_height == right_black_height);

    // to mute unused variable right_black_height compiler warning for release
    // builds..
    return (left_black_height + right_black_height) / 2;
  }

  void verify() const {
    // the root is black.
    assert(!root_ || root_->black_);
    assert(bh_ == verify(root_.get()));
  }

  static const K &proj(const V &value) noexcept {
    if constexpr (is_set_) {
      return value;
    } else {
      return value.first;
    }
  }

public:
  using key_type = K;
  using value_type = V;
  using reference_type = V &;
  using const_reference_type = const V &;
  using node_type = Node;
  // invariant: K cannot be mutated
  // so if V is K, uses const iterator.
  // if V is pair<const K, value>, uses non-const iterator (but only value can
  // be mutated)
  using iterator_type = BSTIterator<V, is_set_, Node>;
  using const_iterator_type = BSTIterator<V, true, Node>;
  using reverse_iterator_type = reverse_iterator<iterator_type>;
  using const_reverse_iterator_type = reverse_iterator<const_iterator_type>;

  iterator_type begin_;
  ptrdiff_t size_ = 0;
  ptrdiff_t bh_ = 0;
  ptrdiff_t potential_ = 0;

  RedBlackTreeBase() = default;
  RedBlackTreeBase(const RedBlackTreeBase &other) {
    if (other.root_) {
      root_ = make_unique<Node>();
      root_->clone(*(other.root_));
    }
    begin_ = iterator_type(minimum());
    size_ = other.size_;
    bh_ = other.bh_;
  }
  RedBlackTreeBase &operator=(const RedBlackTreeBase &other) {
    if (other.root_) {
      root_->clone(*(other.root_));
    }
    begin_ = iterator_type(minimum());
    size_ = other.size_;
    bh_ = other.bh_;
    return *this;
  }
  RedBlackTreeBase(RedBlackTreeBase &&other) noexcept = default;
  RedBlackTreeBase &operator=(RedBlackTreeBase &&other) noexcept = default;

  [[nodiscard]] iterator_type begin() noexcept { return begin_; }

  [[nodiscard]] const_iterator_type begin() const noexcept {
    return const_iterator_type(begin_);
  }

  [[nodiscard]] const_iterator_type cbegin() const noexcept {
    return const_iterator_type(begin_);
  }

  [[nodiscard]] iterator_type end() noexcept { return {}; }

  [[nodiscard]] const_iterator_type end() const noexcept { return {}; }

  [[nodiscard]] const_iterator_type cend() const noexcept { return {}; }

  [[nodiscard]] bool empty() const noexcept { return size_ == 0; }

  [[nodiscard]] ptrdiff_t size() const noexcept { return size_; }

  void clear() noexcept {
    root_ = nullptr;
    size_ = 0;
    bh_ = 0;
  }

  Node *get_root() noexcept { return root_.get(); }

  const Node *get_root() const noexcept { return root_.get(); }

  Derived &derived() { return static_cast<Derived &>(*this); }

  const Derived &derived() const { return static_cast<const Derived &>(*this); }

protected:
  Node *minimum() const noexcept { return minimum(root_.get()); }

  Node *maximum() const noexcept { return maximum(root_.get()); }

  void set_begin(Node *ptr) noexcept { begin_ = iterator_type(ptr); }

  void set_root(unique_ptr<Node> z) noexcept { root_ = move(z); }

  unique_ptr<Node> extract_root() noexcept { return move(root_); }

  void size_incr() noexcept { size_++; }

  void size_decr() noexcept { size_--; }

  int height() const noexcept { return height(root_.get()); }

  static int height(Node *root) {
    if (!root) {
      return 0;
    }
    auto l_height = height(root->left_.get());
    auto r_height = height(root->right_.get());
    return max(l_height, r_height) + 1;
  }

  void left_rotate(Node *x) {
    auto y = move(x->right_);
    x->right_ = move(y->left_); // turn y's left subtree into x's right subtree
    if (x->right_) {            // if y's left subtree is not empty...
      x->right_->parent_ =
          x; // ...then x becomes the parent of the subtree's root
    }
    y->parent_ = x->parent_; // x's parent becomes y's parent
    unique_ptr<Node> x_holder;
    Node *y_ptr = nullptr;
    if (!x->parent_) { // if x was the root...
      x_holder = move(root_);
      root_ = move(y); // ...then y becomes the root
      y_ptr = root_.get();
    } else if (x ==
               x->parent_->left_.get()) { // otherwise, if x was a left child...
      x_holder = move(x->parent_->left_);
      x->parent_->left_ = move(y); // ...then y becomes a left child
      y_ptr = x->parent_->left_.get();
    } else { // otherwise, x was a right child,
      x_holder = move(x->parent_->right_);
      x->parent_->right_ = move(y); // and now y is a right child.
      y_ptr = x->parent_->right_.get();
    }
    y_ptr->left_ = move(x_holder); // make x become y's left child.
    y_ptr->left_->parent_ = y_ptr;
    derived().left_rotate_post(y_ptr, y_ptr->left_.get());
  }

  void right_rotate(Node *x) {
    auto y = move(x->left_);
    x->left_ = move(y->right_); // turn y's right subtree into x's left subtree
    if (x->left_) {             // if y's right subtree is not empty...
      x->left_->parent_ =
          x; // ...then x becomes the parent of the subtree's root
    }
    y->parent_ = x->parent_; // x's parent becomes y's parent
    unique_ptr<Node> x_holder;
    Node *y_ptr = nullptr;
    if (!x->parent_) { // if x was the root...
      x_holder = move(root_);
      root_ = move(y); // ...then y becomes the root
      y_ptr = root_.get();
    } else if (x ==
               x->parent_->left_.get()) { // otherwise, if x was a left child...
      x_holder = move(x->parent_->left_);
      x->parent_->left_ = move(y); // ...then y becomes a left child
      y_ptr = x->parent_->left_.get();
    } else { // otherwise, x was a right child,
      x_holder = move(x->parent_->right_);
      x->parent_->right_ = move(y); // and now y is a right child.
      y_ptr = x->parent_->right_.get();
    }
    y_ptr->right_ = move(x_holder); // make x become y's right child.
    y_ptr->right_->parent_ = y_ptr;
    derived().right_rotate_post(y_ptr, y_ptr->right_.get());
  }

protected:
  void insert_fixup(Node *z) {
    while (z && z->parent_ && !z->parent_->black_) {
      if (z->parent_ ==
          z->parent_->parent_->left_.get()) { // is z's parent a left child?
        auto y = z->parent_->parent_->right_.get(); // y is z's uncle
        if (y && !y->black_) { // are z's parent and uncle both red?
          // case 1
          z->parent_->black_ = true;
          y->black_ = true;
          z->parent_->parent_->black_ = false;
          z = z->parent_->parent_;
          derived().adjust_bh(z, 1);
          potential_++;
        } else {
          // case 2
          if (z == z->parent_->right_.get()) {
            z = z->parent_;
            left_rotate(z);
          }
          // case 3
          z->parent_->black_ = true;
          z->parent_->parent_->black_ = false;
          right_rotate(z->parent_->parent_);
        }
      } else { // same as the above, but with "right" and "left" exchanged
        auto y = z->parent_->parent_->left_.get(); // y is z's uncle
        if (y && !y->black_) { // are z's parent and uncle both red?
          // case 1
          z->parent_->black_ = true;
          y->black_ = true;
          z->parent_->parent_->black_ = false;
          z = z->parent_->parent_;
          derived().adjust_bh(z, 1);
          potential_++;
        } else {
          // case 2
          if (z == z->parent_->left_.get()) {
            z = z->parent_;
            right_rotate(z);
          }
          // case 3
          z->parent_->black_ = true;
          z->parent_->parent_->black_ = false;
          left_rotate(z->parent_->parent_);
        }
      }
    }
    if (!root_->black_) {
      root_->black_ = true;
    }
  }

  void transplant(Node *u, unique_ptr<Node> v) {
    assert(u);
    auto up = u->parent_;
    if (!up) {
      root_ = move(v);
      if (root_) {
        root_->parent_ = nullptr;
      }
    } else if (u == up->left_.get()) {
      up->left_ = move(v);
      if (up->left_) {
        up->left_->parent_ = up;
      }
    } else {
      up->right_ = move(v);
      if (up->right_) {
        up->right_->parent_ = up;
      }
    }
  }

  iterator_type erase(Node *z) {
    if (!z) {
      return {};
    }
    iterator_type it(z);
    ++it;
    if (begin_.node_ == z) {
      begin_ = it;
    }
    BSTChild child = BSTChild::Unused;
    derived().erase_fixup_z(z, child);
    bool orig_black = z->black_;
    Node *x = nullptr;
    Node *xp = nullptr;
    if (!z->left_) {
      x = z->right_.get();
      xp = z->parent_;
      transplant(z, move(z->right_)); // replace z by its right child
    } else if (!z->right_) {
      x = z->left_.get();
      xp = z->parent_;
      transplant(z, move(z->left_)); // replace z by its left child
    } else {
      auto y = minimum(z->right_.get()); // y is z's successor
      orig_black = y->black_;
      x = y->right_.get();
      unique_ptr<Node> y_holder;
      if (y != z->right_.get()) { // is y farther down the tree?
        auto yp = y->parent_;
        assert(yp && y == yp->left_.get());
        y_holder = move(yp->left_);
        // replace y by its right child.
        yp->left_ = move(y->right_);
        if (yp->left_) {
          yp->left_->parent_ = yp;
        }
        y = y_holder.get();
        // z's right child becomes y's right child
        y->right_ = move(z->right_);
        xp = yp;
        child = BSTChild::Left;
      } else {
        xp = y;
        child = BSTChild::Right;
      }
      auto zl = move(z->left_); // extract left of z
      assert(zl);

      auto yp = y->parent_; // prepare to extract y
      assert(yp);
      if (!y_holder) {
        assert(yp == z);
        y_holder = move(yp->right_);
      }

      // replace z by its successor y
      // and give z's left child to y, which had no left child
      derived().erase_fixup_zy(z, y_holder.get());
      bool z_black = z->black_;
      auto zp = z->parent_;
      if (!zp) {
        root_ = move(y_holder);
        root_->parent_ = nullptr;
        y = root_.get();
      } else if (z == zp->left_.get()) {
        zp->left_ = move(y_holder);
        zp->left_->parent_ = zp;
        y = zp->left_.get();
      } else {
        zp->right_ = move(y_holder);
        zp->right_->parent_ = zp;
        y = zp->right_.get();
      }
      y->left_ = move(zl);
      y->left_->parent_ = y;
      if (y->right_) {
        y->right_->parent_ = y;
      }

      y->black_ = z_black;
    }
    derived().erase_fixup_x(x, xp, child);
    // if any red-black violations occurred, correct them
    if (orig_black) {
      erase_fixup(x, xp);
    }
    size_--;
    // add verify(); here to verify.
    verify();

    return it;
  }

  void erase_fixup(Node *x, Node *xp) {
    while (x != root_.get() && (!x || x->black_)) {
      assert(xp);
      if (x == xp->left_.get()) {  // is x a left child?
        auto w = xp->right_.get(); // w is x's sibling
        assert(w);
        if (!w->black_) {
          // case 1
          w->black_ = true;
          xp->black_ = false;
          left_rotate(xp);
          w = xp->right_.get();
        }
        if ((!w->left_ || w->left_->black_) &&
            (!w->right_ || w->right_->black_)) {
          // case 2
          derived().adjust_bh(xp, x, 0);
          if (xp == root_.get()) {
            bh_--;
          }
          w->black_ = false;
          x = xp;
          xp = xp->parent_;
          potential_++;
        } else {
          if (!w->right_ || w->right_->black_) {
            // case 3
            assert(w->left_);
            w->left_->black_ = true;
            w->black_ = false;
            right_rotate(w);
            w = xp->right_.get();
          }
          // case 4
          assert(w->right_);
          w->black_ = xp->black_;
          xp->black_ = true;
          w->right_->black_ = true;
          derived().adjust_bh(xp, x, 1);
          derived().adjust_bh(xp->parent_, xp, 1);
          left_rotate(xp);
          x = root_.get();
          potential_++;
        }
      } else { // same as the above, but with "right" and "left" exchanged
        auto w = xp->left_.get(); // w is x's sibling
        assert(w);
        if (!w->black_) {
          // case 1
          w->black_ = true;
          xp->black_ = false;
          right_rotate(xp);
          w = xp->left_.get();
        }
        if ((!w->left_ || w->left_->black_) &&
            (!w->right_ || w->right_->black_)) {
          // case 2
          derived().adjust_bh(xp, x, 0);
          if (xp == root_.get()) {
            bh_--;
          }
          w->black_ = false;
          x = xp;
          xp = xp->parent_;
          potential_++;
        } else {
          if (!w->left_ || w->left_->black_) {
            // case 3
            assert(w->right_);
            w->right_->black_ = true;
            w->black_ = false;
            left_rotate(w);
            w = xp->left_.get();
          }
          // case 4
          assert(w->left_);
          w->black_ = xp->black_;
          xp->black_ = true;
          w->left_->black_ = true;
          derived().adjust_bh(xp, x, 1);
          derived().adjust_bh(xp->parent_, xp, 1);
          right_rotate(xp);
          x = root_.get();
          potential_++;
        }
      }
    }
    if (x) {
      x->black_ = true;
    }
  }

  static Node *minimum(Node *node) {
    while (node->left_) {
      node = node->left_.get();
    }
    return node;
  }

  static Node *maximum(Node *node) {
    while (node->right_) {
      node = node->right_.get();
    }
    return node;
  }

  Node *find_node(const K &key) const {
    auto res = find_lower_bound(key);
    if (res.bound_ && proj(res.bound_->key_) == key) {
      return res.bound_;
    } else {
      return nullptr;
    }
  }

  SearchResult find_lower_bound(const K &key) const {
    auto x = root_.get();
    SearchResult res;
    while (x) {
      res.parent_ = x;
      if (Comp{}(proj(x->key_), key)) {
        res.where_ = BSTChild::Right;
        x = x->right_.get();
      } else {
        res.where_ = BSTChild::Left;
        res.bound_ = x;
        x = x->left_.get();
      }
    }
    return res;
  }

  SearchResult find_upper_bound(const K &key) const {
    auto x = root_.get();
    SearchResult res;
    while (x) {
      res.parent_ = x;
      if (Comp{}(key, proj(x->key_))) {
        res.where_ = BSTChild::Left;
        res.bound_ = x;
        x = x->left_.get();
      } else {
        res.where_ = BSTChild::Right;
        x = x->right_.get();
      }
    }
    return res;
  }

  pair<Node *, Node *> find_equal_range(const K &key) const {
    Node *lo = nullptr;
    Node *hi = nullptr;

    auto x = root_.get();
    while (x) {
      if (Comp{}(proj(x->key_), key)) {
        x = x->right_.get();
      } else {
        if (!hi && Comp{}(key, proj(x->key_))) {
          hi = x;
        }
        lo = x;
        x = x->left_.get();
      }
    }
    x = hi ? hi->left_.get() : root_.get();
    while (x) {
      if (Comp{}(key, proj(x->key_))) {
        hi = x;
        x = x->left_.get();
      } else {
        x = x->right_.get();
      }
    }
    return {lo, hi};
  }

  Node *insert_node(unique_ptr<Node> z, const SearchResult &res) {
    z->parent_ = res.parent_; // found the location - insert z with parent y
    Node *z_ptr = nullptr;
    if (!z->parent_) {
      root_ = move(z); // tree was empty.
      z_ptr = root_.get();
    } else if (res.where_ == BSTChild::Left) {
      res.parent_->left_ = move(z);
      z_ptr = res.parent_->left_.get();
    } else {
      res.parent_->right_ = move(z);
      z_ptr = res.parent_->right_.get();
    }
    assert(z_ptr);
    derived().insert_fixup_pre(z_ptr, z_ptr->parent_);
    insert_fixup(z_ptr);
    size_++;
    if (!begin_.node_ || Comp{}(proj(z_ptr->key_), proj(*begin_))) {
      begin_ = iterator_type(z_ptr);
    }
    // add verify(); here to verify.
    verify();
    return z_ptr;
  }

  size_t erase_range(iterator_type first, iterator_type last) {
    if (first == begin_ && !last.node_) {
      auto cnt = size_;
      clear();
      return cnt;
    }
    auto cnt = 0;
    while (first != last) {
      first = erase(first.node_);
      cnt++;
    }
    return cnt;
  }

public:
  iterator_type find(const K &key) { return iterator_type(find_node(key)); }

  const_iterator_type find(const K &key) const {
    return const_iterator_type(find_node(key));
  }

  bool contains(const K &key) const { return find_node(key) != nullptr; }

  iterator_type lower_bound(const K &key) {
    return iterator_type(find_lower_bound(key).bound_);
  }

  const_iterator_type lower_bound(const K &key) const {
    return const_iterator_type(find_lower_bound(key).bound_);
  }

  iterator_type upper_bound(const K &key) {
    return iterator_type(find_upper_bound(key).bound_);
  }

  const_iterator_type upper_bound(const K &key) const {
    return const_iterator_type(find_upper_bound(key).bound_);
  }

  pair<iterator_type, iterator_type> equal_range(const K &key) {
    auto res = find_equal_range(key);
    return {iterator_type(res.first), iterator_type(res.second)};
  }

  pair<const_iterator_type, const_iterator_type>
  equal_range(const K &key) const {
    auto res = find_equal_range(key);
    return {const_iterator_type(res.first), const_iterator_type(res.second)};
  }

  pair<const_iterator_type, const_iterator_type> enumerate(const K &a,
                                                           const K &b) const {
    if (Comp{}(b, a)) {
      throw invalid_argument("b < a in enumerate()");
    }
    return {lower_bound(a), upper_bound(b)};
  }

private:
  template <typename T>
  conditional_t<AllowDup, iterator_type, pair<iterator_type, bool>>
  insert_value(T &&key) requires is_same_v<remove_cvref_t<T>, V> {
    SearchResult res;
    if constexpr (AllowDup) {
      res = find_upper_bound(proj(key));
    } else {
      res = find_lower_bound(proj(key));
      if (res.bound_ && proj(res.bound_->key_) == proj(key)) {
        return {iterator_type(res.bound_), false};
      }
    }

    auto z = make_unique<Node>(forward<T>(key));
    auto z_ptr = insert_node(move(z), res);
    if constexpr (AllowDup) {
      return iterator_type(z_ptr);
    } else {
      return {iterator_type(z_ptr), true};
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
    SearchResult res;
    res = find_lower_bound(key);
    if (res.bound_ && proj(res.bound_->key_) == key) {
      return res.bound_->key_.second;
    }

    V val{move(key), {}};
    auto z = make_unique<Node>(move(val));
    auto z_ptr = insert_node(move(z), res);
    return z_ptr->key_.second;
  }

  iterator_type erase(iterator_type iter) {
    if (iter == end()) {
      throw invalid_argument("attempt to erase end()");
    }
    return erase(iter.node_);
  }

  size_t erase(const K &key) {
    if constexpr (AllowDup) {
      auto eqr = find_equal_range(key);
      return erase_range(iterator_type(eqr.first), iterator_type(eqr.second));
    } else {
      auto res = find_lower_bound(proj(key));
      if (res.bound_ && proj(res.bound_->key_) == key) {
        erase(res.bound_);
        return 1;
      } else {
        return 0;
      }
    }
  }

  friend ostream &operator<<(ostream &os, const RedBlackTreeBase &tree) {
    inorder_print(os, tree.root_.get());
    return os;
  }

private:
  void preorder_print(const Node *node) const {
    if (node) {
      cout << node->key_ << ' ';
      cout << ((node->black_) ? "B " : "R ");
      preorder_print(node->left_.get());
      preorder_print(node->right_.get());
    } else {
      cout << "#B ";
    }
  }

protected:
  // customizable protected functions for derived classes
  void adjust_bh(Node* node, ptrdiff_t i) noexcept {
    if (node == root_.get()) {
      bh_ += i;
    }
  }

  void adjust_bh(const Node*, const Node*, ptrdiff_t) const noexcept {
    // do nothing
  }

  void insert_fixup_pre(const Node *, const Node *) const noexcept {
    // do nothing
  }

  void erase_fixup_x(const Node *, const Node *, const BSTChild&) const noexcept {
    // do nothing
  }

  void erase_fixup_z(const Node *, const BSTChild&) const noexcept {
    // do nothing
  }

  void erase_fixup_zy(const Node *, const Node *) const noexcept {
    // do nothing
  }

  void left_rotate_post(const Node *, const Node *) const noexcept {
    // do nothing
  }

  void right_rotate_post(const Node *, const Node *) const noexcept {
    // do nothing
  }

public:
  void preorder_print() const { preorder_print(root_.get()); }

  template <Containable K_, typename V_, typename Comp_, bool AllowDup_,
            typename Node_, typename Derived_, typename T>
  friend RedBlackTreeBase<K_, V_, Comp_, AllowDup_, Node_, Derived_>
  join(RedBlackTreeBase<K_, V_, Comp_, AllowDup_, Node_, Derived_> &&tree1,
       T &&raw_value,
       RedBlackTreeBase<K_, V_, Comp_, AllowDup_, Node_, Derived_>
           &&tree2) requires is_constructible_v<V_, remove_cvref_t<T>>;

  [[nodiscard]] ptrdiff_t get_potential() const noexcept { return potential_; }
};

template <Containable K, typename V, typename Comp, bool AllowDup,
          typename Node, typename Derived, typename T>
RedBlackTreeBase<K, V, Comp, AllowDup, Node, Derived>
join(RedBlackTreeBase<K, V, Comp, AllowDup, Node, Derived> &&tree1,
     T &&raw_value,
     RedBlackTreeBase<K, V, Comp, AllowDup, Node, Derived> &&tree2) requires
    is_constructible_v<V, remove_cvref_t<T>> {
  using Tree = RedBlackTreeBase<K, V, Comp, AllowDup, Node, Derived>;
  V mid_value{forward<T>(raw_value)};
  assert(tree1.empty() || !Comp{}(proj(mid_value), proj(*tree1.rbegin())));
  assert(tree2.empty() || !Comp{}(proj(*tree2.begin()), proj(mid_value)));

  if (tree1.bh_ >= tree2.bh_) {
    Tree new_tree = move(tree1);
    ptrdiff_t curr_bh = tree1.bh_;
    Node *curr_node = new_tree.root_.get();
    Node *curr_parent = nullptr;
    BSTChild child = BSTChild::Unused;
    while (curr_node && curr_bh > tree2.bh_) {
      if (curr_node->black_) {
        --curr_bh;
      }
      curr_parent = curr_node;
      if (curr_node->right_) {
        curr_node = curr_node->right_.get();
        child = BSTChild::Right;
      } else {
        curr_node = curr_node->left_.get();
        child = BSTChild::Left;
      }
    }
    unique_ptr<Node> same_bh_subtree = make_unique<Node>(move(mid_value));
    if (child == BSTChild::Right) {
      same_bh_subtree->left_ = move(curr_parent->right_);
      same_bh_subtree->right_ = move(tree2.root_);
      curr_parent->right_ = move(same_bh_subtree);
    } else if (child == BSTChild::Left) {
      same_bh_subtree->left_ = move(curr_parent->left_);
      same_bh_subtree->right_ = move(tree2.root_);
      curr_parent->left_ = move(same_bh_subtree);
    } else if (child == BSTChild::Unused) {
      // tree1 was empty or black height of two trees were the same
      same_bh_subtree->left_ = move(new_tree.root_);
      same_bh_subtree->right_ = move(tree2.root_);
      new_tree.root_ = move(same_bh_subtree);
    } else {
      throw runtime_error("BSTChild class error");
    }
    return new_tree;
  } else {
    Tree new_tree = move(tree2);
    ptrdiff_t curr_bh = tree2.bh_;
    Node *curr_node = new_tree.root_.get();
    Node *curr_parent = nullptr;
    BSTChild child = BSTChild::Unused;
    while (curr_node && curr_bh > tree1.bh_) {
      if (curr_node->black_) {
        --curr_bh;
      }
      curr_parent = curr_node;
      if (curr_node->left_) {
        curr_node = curr_node->left_.get();
        child = BSTChild::Left;
      } else {
        curr_node = curr_node->right_.get();
        child = BSTChild::Right;
      }
    }
    unique_ptr<Node> same_bh_subtree = make_unique<Node>(move(mid_value));
    if (child == BSTChild::Right) {
      same_bh_subtree->left_ = move(tree1.root_);
      same_bh_subtree->right_ = move(curr_parent->right_);
      curr_parent->right_ = move(same_bh_subtree);
    } else if (child == BSTChild::Left) {
      same_bh_subtree->left_ = move(tree1.root_);
      same_bh_subtree->right_ = move(curr_parent->left_);
      curr_parent->left_ = move(same_bh_subtree);
    } else if (child == BSTChild::Unused) {
      // tree1 was empty or black height of two trees were the same
      same_bh_subtree->left_ = move(tree1.root_);
      same_bh_subtree->right_ = move(new_tree.root_);
      new_tree.root_ = move(same_bh_subtree);
    } else {
      throw runtime_error("BSTChild class error");
    }
    return new_tree;
  }
}

template <Containable K, typename V, typename Comp, bool AllowDup,
          typename Node>
class RedBlackTree
    : public RedBlackTreeBase<K, V, Comp, AllowDup, Node,
                              RedBlackTree<K, V, Comp, AllowDup, Node>> {};

} // namespace detail

template <Containable K, typename Comp = less<K>>
using TreeSet = detail::RedBlackTree<K, K, Comp, false, detail::RBTreeNode<K>>;

template <Containable K, typename Comp = less<K>>
using TreeMultiSet =
    detail::RedBlackTree<K, K, Comp, true, detail::RBTreeNode<K>>;

template <Containable K, Containable V, typename Comp = less<K>>
using TreeMap = detail::RedBlackTree<K, pair<const K, V>, Comp, false,
                                     detail::RBTreeNode<pair<const K, V>>>;

template <Containable K, Containable V, typename Comp = less<K>>
using TreeMultiMap = detail::RedBlackTree<K, pair<const K, V>, Comp, true,
                                          detail::RBTreeNode<pair<const K, V>>>;

} // namespace frozenca::hard

#endif //__CLRS4_RED_BLACK_TREE_H__
