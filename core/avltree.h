#ifndef __CLRS4_AVL_TREE_H__
#define __CLRS4_AVL_TREE_H__

#include <cassert>
#include <common.h>
#include <iostream>
#include <random>
#include <rbtree.h>
#include <stdexcept>

namespace frozenca::hard {

using namespace std;

namespace detail {

template <Containable T> struct AVLTreeNode {
  T key_ = T{};
  AVLTreeNode *parent_ = nullptr;
  // left subtree height - right subtree height
  // invariant: -1 <= balance_ <= +1
  ptrdiff_t balance_ = 0;
  unique_ptr<AVLTreeNode> left_;
  unique_ptr<AVLTreeNode> right_;
  AVLTreeNode() = default;
  AVLTreeNode(T key) : key_{move(key)} {}

  void clone(const AVLTreeNode &other) {
    key_ = other.key_;
    balance_ = other.balance_;
    if (other.left_) {
      left_ = make_unique<AVLTreeNode>();
      left_->clone(*other.left_);
      left_->parent_ = this;
    }
    if (other.right_) {
      right_ = make_unique<AVLTreeNode>();
      right_->clone(*other.right_);
      right_->parent_ = this;
    }
  }

  AVLTreeNode(const AVLTreeNode &node) = delete;
  AVLTreeNode &operator=(const AVLTreeNode &node) = delete;
  AVLTreeNode(AVLTreeNode &&node) = delete;
  AVLTreeNode &operator=(AVLTreeNode &&node) = delete;

  friend istream &operator>>(istream &is, unique_ptr<AVLTreeNode> &node) {
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
    node = make_unique<AVLTreeNode>(v);
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

  friend ostream &operator<<(ostream &os, const AVLTreeNode *node) {
    if (node) {
      os << node->key_ << ' ';
      os << node->left_.get();
      os << node->right_.get();
    } else {
      os << "# ";
    }
    return os;
  }

  friend void inorder_print(ostream &os, const AVLTreeNode *node) {
    if (node) {
      inorder_print(os, node->left_.get());
      os << node->key_ << ' ';
      inorder_print(os, node->right_.get());
    }
  }
};

template <Containable K, typename V, typename Comp, bool AllowDup>
requires invocable<Comp, K, K>
class AVLTree {
  // invariant: V is either K or pair<const K, Value> for some Value type.
  static constexpr bool is_set_ = is_same_v<K, V>;

public:
  using Node = AVLTreeNode<V>;
  using SearchResult = BSTSearchResult<Node>;

private:
  unique_ptr<Node> root_;

  static int verify(const Node *node) {
    if (!node) {
      return 0;
    }

    // binary search tree properties
    assert(!node->left_ ||
           (node->left_->parent_ == node &&
            !Comp{}(proj(node->key_), proj(node->left_->key_))));
    assert(!node->right_ ||
           (node->right_->parent_ == node &&
            !Comp{}(proj(node->right_->key_), proj(node->key_))));

    auto l_height = verify(node->left_.get());
    auto r_height = verify(node->right_.get());

    assert(l_height - r_height == node->balance_);

    assert(node->balance_ >= -1 && node->balance_ <= 1);

    return max(l_height, r_height) + 1;
  }

  void verify() const { verify(root_.get()); }

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

  AVLTree() = default;
  AVLTree(const AVLTree &other) {
    if (other.root_) {
      root_ = make_unique<Node>();
      root_->clone(*(other.root_));
    }
    begin_ = iterator_type(minimum());
    size_ = other.size_;
  }
  AVLTree &operator=(const AVLTree &other) {
    if (other.root_) {
      root_->clone(*(other.root_));
    }
    begin_ = iterator_type(minimum());
    size_ = other.size_;
    return *this;
  }
  AVLTree(AVLTree &&other) noexcept = default;
  AVLTree &operator=(AVLTree &&other) noexcept = default;

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
  }

  Node *get_root() noexcept { return root_.get(); }

  const Node *get_root() const noexcept { return root_.get(); }

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
    Node *x_ptr = y_ptr->left_.get();

    // x balance : a - max(b, c) - 1 => a - b
    // y balance : b - c => max(a, b) - c + 1

    if (y_ptr->balance_ >= 0) {                 // x balance : a - b - 1
      y_ptr->balance_++;                        // b - c => b - c + 1
      if (x_ptr->balance_ >= 0) {               // a > b?
        y_ptr->balance_ += x_ptr->balance_ + 1; // add a - b => a - c + 1
      }
      x_ptr->balance_++; // a - b - 1 => a - b
    } else {             // x balance : a - c - 1
      x_ptr->balance_ +=
          (1 - y_ptr->balance_); // a - c - 1 + (1 - (b - c)) => a - b
      y_ptr->balance_++;         // b - c => b - c + 1
      if (x_ptr->balance_ > 0) { // a > b?
        y_ptr->balance_ += x_ptr->balance_; // add a - b => a - c + 1
      }
    }
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
    Node *x_ptr = y_ptr->right_.get();

    // x balance : max(a, b) - c + 1 => b - c
    // y balance : a - b => a - max(b, c) - 1
    if (y_ptr->balance_ >= 0) { // a >= b => x balance : a - c + 1
      x_ptr->balance_ -=
          (y_ptr->balance_ + 1);            // (a - c + 1) - (a - b + 1) = b - c
      y_ptr->balance_--;                    // a - b => a - b - 1
      if (x_ptr->balance_ < 0) {            // b < c?
        y_ptr->balance_ += x_ptr->balance_; // a - b - 1 + (b - c) = a - c - 1
      }
    } else {                                // a < b => x balance: b - c + 1
      x_ptr->balance_--;                    // b - c + 1 => b - c
      y_ptr->balance_--;                    // a - b => a - b - 1
      if (x_ptr->balance_ < 0) {            // b < c?
        y_ptr->balance_ += x_ptr->balance_; // a - b - 1 + (b - c) = a - c - 1
      }
    }
  }

private:
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

    Node *zp = z->parent_;
    Node *x = nullptr; // fixup starting point
    bool carry = true;
    if (!z->left_) {
      x = zp;
      if (zp) {
        carry = zp->balance_;
        if (z == zp->left_.get()) {
          zp->balance_--;
        } else if (z == zp->right_.get()) {
          zp->balance_++;
        }
      }
      transplant(z, move(z->right_));
    } else if (!z->right_) {
      x = zp;
      if (zp) {
        carry = zp->balance_;
        if (z == zp->left_.get()) {
          zp->balance_--;
        } else if (z == zp->right_.get()) {
          zp->balance_++;
        }
      }
      transplant(z, move(z->left_));
    } else {
      // find z's successor
      auto y = z->right_.get();
      while (y->left_) {
        // if y is farther down the tree,
        // adjust immediate ancestor's balance
        // because the node being deleted is from their left subtree
        x = y;
        y = y->left_.get();
      }
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
      auto z_balance = z->balance_;
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

      y->balance_ = z_balance;
      if (!x) {
        x = y; // fixup start from y only if y is z->right_
        carry = x->balance_;
        x->balance_++;
      } else {
        carry = x->balance_;
        x->balance_--; // probing start point
        y->balance_ = z_balance;
      }
    }
    size_--;
    erase_fixup(x, carry);
    // add verify(); here to verify.
    return it;
  }

  void erase_fixup(Node *x, bool carry) {
    while (x) {
      auto xp = x->parent_;
      if (x->balance_ < -1) {
        auto y = x->right_.get();
        assert(y);
        if (y->balance_ >= 1) {
          right_rotate(y);
          y = x->right_.get();
        }
        if (xp) {
          carry = xp->balance_ && y->balance_ <= -1;
          if (y->balance_ <= -1) {
            if (x == xp->left_.get()) {
              xp->balance_--;
            } else if (x == xp->right_.get()) {
              xp->balance_++;
            }
          }
        }
        left_rotate(x);
      } else if (x->balance_ > 1) {
        auto y = x->left_.get();
        assert(y);
        if (y->balance_ <= -1) {
          left_rotate(y);
          y = x->left_.get();
        }
        if (xp) {
          carry = xp->balance_ && y->balance_ >= 1;
          if (y->balance_ >= 1) {
            if (x == xp->left_.get()) {
              xp->balance_--;
            } else if (x == xp->right_.get()) {
              xp->balance_++;
            }
          }
        }
        right_rotate(x);
      } else if (carry && xp) {
        carry = xp->balance_;
        if (x == xp->left_.get()) {
          xp->balance_--;
        } else {
          xp->balance_++;
        }
      }
      x = xp;
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
    auto y = res.parent_;
    Node *z_ptr = nullptr;
    bool carry = false;
    if (!z->parent_) {
      root_ = move(z); // tree was empty.
      z_ptr = root_.get();
    } else if (res.where_ == BSTChild::Left) {
      res.parent_->left_ = move(z);
      z_ptr = res.parent_->left_.get();
      y->balance_++;
      if (y->balance_ != 0) {
        carry = true;
      }
    } else {
      res.parent_->right_ = move(z);
      z_ptr = res.parent_->right_.get();
      y->balance_--;
      if (y->balance_ != 0) {
        carry = true;
      }
    }
    assert(z_ptr);
    size_++;
    if (!begin_.node_ || Comp{}(proj(z_ptr->key_), proj(*begin_))) {
      begin_ = iterator_type(z_ptr);
    }
    insert_fixup(y, carry);
    // add verify(); here to verify.
    return z_ptr;
  }

  void insert_fixup(Node *x, bool carry) {
    while (x) {
      auto xp = x->parent_;
      if (x->balance_ < -1) {
        auto xr = x->right_.get();
        assert(xr);
        if (xr->balance_ >= 1) {
          right_rotate(xr);
        }
        left_rotate(x);
        carry = false;
      } else if (x->balance_ > 1) {
        auto xl = x->left_.get();
        assert(xl);
        if (xl->balance_ <= -1) {
          left_rotate(xl);
        }
        right_rotate(x);
        carry = false;
      } else if (carry && xp) {
        if (x == xp->left_.get()) {
          xp->balance_++;
        } else {
          xp->balance_--;
        }
        if (xp->balance_ == 0) {
          carry = false;
        }
      }
      x = xp;
    }
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

  friend ostream &operator<<(ostream &os, const AVLTree &tree) {
    inorder_print(os, tree.root_.get());
    return os;
  }

private:
  void preorder_print(const Node *node) const {
    if (node) {
      cout << node->key_ << ' ';
      preorder_print(node->left_.get());
      preorder_print(node->right_.get());
    } else {
      cout << "# ";
    }
  }

public:
  void preorder_print() const { preorder_print(root_.get()); }
};

} // namespace detail

template <Containable K, typename Comp = less<K>>
using AVLTreeSet = detail::AVLTree<K, K, Comp, false>;

template <Containable K, typename Comp = less<K>>
using AVLTreeMultiSet = detail::AVLTree<K, K, Comp, true>;

template <Containable K, Containable V, typename Comp = less<K>>
using AVLTreeMap = detail::AVLTree<K, pair<const K, V>, Comp, false>;

template <Containable K, Containable V, typename Comp = less<K>>
using AVLTreeMultiMap = detail::AVLTree<K, pair<const K, V>, Comp, true>;

} // namespace frozenca::hard

#endif //__CLRS4_AVL_TREE_H__
