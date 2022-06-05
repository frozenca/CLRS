#ifndef __CLRS4_RED_BLACK_TREE_H__
#define __CLRS4_RED_BLACK_TREE_H__

#include <binarytree.h>
#include <cassert>
#include <common.h>
#include <iostream>
#include <random>
#include <stdexcept>

namespace frozenca::hard {

using namespace std;

namespace detail {

template <Containable T> struct RBTreeNode {
  T key_ = T{};
  RBTreeNode *parent_ = nullptr;
  bool black_ = false;                        // the new node starts out red
  unique_ptr<RBTreeNode> left_;
  unique_ptr<RBTreeNode> right_;
  RBTreeNode() = default;
  RBTreeNode(T key) : key_{move(key)} {}

  void clone(const RBTreeNode &other) {
    key_ = other.key_;
    black_ = other.black_;
    if (other.left_) {
      left_ = make_unique<RBTreeNode>();
      left_->clone(*other.left_);
      left_->parent_ = this;
    }
    if (other.right_) {
      right_ = make_unique<RBTreeNode>();
      right_->clone(*other.right_);
      right_->parent_ = this;
    }
  }

  RBTreeNode(const RBTreeNode &node) = delete;
  RBTreeNode &operator=(const RBTreeNode &node) = delete;
  RBTreeNode(RBTreeNode &&node) = delete;
  RBTreeNode &operator=(RBTreeNode &&node) = delete;

  friend istream &operator>>(istream &is, unique_ptr<RBTreeNode> &node) {
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
    node = make_unique<RBTreeNode>(v);
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

  friend ostream &operator<<(ostream &os, const RBTreeNode *node) {
    if (node) {
      os << node->key_ << ' ';
      os << node->left_.get();
      os << node->right_.get();
    } else {
      os << "# ";
    }
    return os;
  }

  friend void inorder_print(ostream& os, const RBTreeNode* node) {
    if (node) {
      inorder_print(os, node->left_.get());
      os << node->key_ << ' ';
      inorder_print(os, node->right_.get());
    }
  }
};

template <Containable T, bool Const> class RBTreeIterator {
public:
  using difference_type = ptrdiff_t;
  using value_type = T;
  using pointer = conditional_t<Const, const T *, T *>;
  using reference = conditional_t<Const, const T &, T &>;
  using iterator_category = bidirectional_iterator_tag;
  using iterator_concept = iterator_category;
  using Node = RBTreeNode<T>;

public:
  Node *node_ = nullptr;

  RBTreeIterator() = default;
  RBTreeIterator(Node *node) : node_{node} {}

  RBTreeIterator(const RBTreeIterator<T, !Const> &other) : node_{other.node_} {}

  RBTreeIterator(const RBTreeIterator &other) = default;
  RBTreeIterator &operator=(const RBTreeIterator &other) = default;
  RBTreeIterator(RBTreeIterator &&other) noexcept = default;
  RBTreeIterator &operator=(RBTreeIterator &&other) noexcept = default;

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

  RBTreeIterator &operator++() {
    increment();
    return *this;
  }

  RBTreeIterator operator++(int) {
    RBTreeIterator temp = *this;
    increment();
    return temp;
  }

  RBTreeIterator &operator--() {
    decrement();
    return *this;
  }

  RBTreeIterator operator--(int) {
    RBTreeIterator temp = *this;
    decrement();
    return temp;
  }

  [[nodiscard]] friend bool operator==(const RBTreeIterator &lhs,
                                       const RBTreeIterator &rhs) {
    return lhs.node_ == rhs.node_;
  }
};

enum class RBChild {
  Unused,
  Left,
  Right,
};

template <typename V>
struct RBSearchResult {
  using Node = RBTreeNode<V>;

  Node* bound_ = nullptr;
  Node* parent_ = nullptr;
  RBChild where_ = RBChild::Unused;
};

template <Containable K, typename V, typename Comp, bool AllowDup>
class RedBlackTree {
  // invariant: V is either K or pair<const K, Value> for some Value type.
  static constexpr bool is_set_ = is_same_v<K, V>;

  using Node = RBTreeNode<V>;
  using SearchResult = RBSearchResult<V>;

  unique_ptr<Node> root_;

  static int verify(const Node *node) {
    if (!node) {
      return 0;
    }

    auto left_black_height = verify(node->left_.get());
    auto right_black_height = verify(node->right_.get());
    
    // binary search tree properties
    assert(!node->left_ || (node->left_->parent_ == node &&
                            Comp{}(node->left_->key_, node->key_)));
    assert(!node->right_ || (node->right_->parent_ == node &&
                             Comp{}(node->key_, node->right_->key_)));
    
    // if a node is red, then both its children are black.
    assert(node->black_ || ((!node->left_ || node->left_->black_) && (!node->right_ || node->right_->black_)));

    // for each node, all simple paths from the node to
    // descendant leaves contain the same number of black nodes.
    assert(left_black_height == right_black_height);

    // to mute unused variable right_black_height compiler warning for release builds..
    return ((left_black_height + right_black_height) / 2) + ((node->black_) ? 1 : 0);
  }

  void verify() const {
    // the root is black.
    assert(!root_ || root_->black_);
    verify(root_.get());
  }

  const K& proj(const V& value) const noexcept {
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
  // if V is pair<const K, value>, uses non-const iterator (but only value can be mutated)
  using iterator_type = RBTreeIterator<V, is_set_>;
  using const_iterator_type = RBTreeIterator<V, true>;
  using reverse_iterator_type = reverse_iterator<iterator_type>;
  using const_reverse_iterator_type = reverse_iterator<const_iterator_type>;

  iterator_type begin_;
  ptrdiff_t size_ = 0;

  RedBlackTree() = default;
  RedBlackTree(const RedBlackTree &other) {
    if (other->root_) {
      root_ = make_unique<Node>();
      root_->clone(*(other->root_));
    }
    begin_ = iterator_type(minimum());
  }
  RedBlackTree &operator=(const RedBlackTree &other) {
    if (other->root_) {
      root_->clone(*(other->root_));
    }
    begin_ = iterator_type(minimum());
    return *this;
  }
  RedBlackTree(RedBlackTree &&other) noexcept = default;
  RedBlackTree &operator=(RedBlackTree &&other) noexcept = default;

  [[nodiscard]] iterator_type begin() noexcept {
    return begin_;
  }

  [[nodiscard]] const_iterator_type begin() const noexcept {
    return const_iterator_type(begin_);
  }

  [[nodiscard]] const_iterator_type cbegin() const noexcept {
    return const_iterator_type(begin_);
  }

  [[nodiscard]] iterator_type end() noexcept {
    return {};
  }

  [[nodiscard]] const_iterator_type end() const noexcept {
    return {};
  }

  [[nodiscard]] const_iterator_type cend() const noexcept {
    return {};
  }

  [[nodiscard]] bool empty() const noexcept { return size_ == 0; }

  [[nodiscard]] ptrdiff_t size() const noexcept { return size_; }

  void clear() noexcept {
    root_ = nullptr;
    size_ = 0;
  }

private:
  Node *minimum() const noexcept { return minimum(root_.get()); }

  Node *maximum() const noexcept { return maximum(root_.get()); }

  int height() const noexcept {
    return height(root_.get());
  }

  static int height(Node* root) {
    if (!root) {
      return 0;
    }
    auto l_height = height(root->left_.get());
    auto r_height = height(root->right_.get());
    return max(l_height, r_height) + 1;
  }

  void left_rotate(Node* x) {
    auto y = move(x->right_);
    x->right_ = move(y->left_);                 // turn y's left subtree into x's right subtree
    if (x->right_) {                            // if y's left subtree is not empty...
      x->right_->parent_ = x;                   // ...then x becomes the parent of the subtree's root
    }
    y->parent_ = x->parent_;                    // x's parent becomes y's parent
    unique_ptr<Node> x_holder;
    Node* y_ptr = nullptr;
    if (!x->parent_) {                          // if x was the root...
      x_holder = move(root_);
      root_ = move(y);                          // ...then y becomes the root
      y_ptr = root_.get();
    } else if (x == x->parent_->left_.get()) {  // otherwise, if x was a left child...
      x_holder = move(x->parent_->left_);
      x->parent_->left_ = move(y);              // ...then y becomes a left child
      y_ptr = x->parent_->left_.get();
    } else {                                    // otherwise, x was a right child,
      x_holder = move(x->parent_->right_);
      x->parent_->right_ = move(y);             // and now y is a right child.
      y_ptr = x->parent_->right_.get();
    }
    y_ptr->left_ = move(x_holder);              // make x become y's left child.
    y_ptr->left_->parent_ = y_ptr;
  }

  void right_rotate(Node* x) {
    auto y = move(x->left_);
    x->left_ = move(y->right_);                 // turn y's right subtree into x's left subtree
    if (x->left_) {                             // if y's right subtree is not empty...
      x->left_->parent_ = x;                    // ...then x becomes the parent of the subtree's root
    }
    y->parent_ = x->parent_;                    // x's parent becomes y's parent
    unique_ptr<Node> x_holder;
    Node* y_ptr = nullptr;
    if (!x->parent_) {                          // if x was the root...
      x_holder = move(root_);
      root_ = move(y);                          // ...then y becomes the root
      y_ptr = root_.get();
    } else if (x == x->parent_->left_.get()) {  // otherwise, if x was a left child...
      x_holder = move(x->parent_->left_);
      x->parent_->left_ = move(y);              // ...then y becomes a left child
      y_ptr = x->parent_->left_.get();
    } else {                                    // otherwise, x was a right child,
      x_holder = move(x->parent_->right_);
      x->parent_->right_ = move(y);             // and now y is a right child.
      y_ptr = x->parent_->right_.get();
    }
    y_ptr->right_ = move(x_holder);             // make x become y's right child.
    y_ptr->right_->parent_ = y_ptr;
  }

  void insert_fixup(Node* z) {
    while (z && z->parent_ && !z->parent_->black_) {
      if (z->parent_ == z->parent_->parent_->left_.get()) {      // is z's parent a left child?
        auto y = z->parent_->parent_->right_.get();              // y is z's uncle
        if (y && !y->black_) {                                   // are z's parent and uncle both red?
          // case 1
          z->parent_->black_ = true;
          y->black_ = true;
          z->parent_->parent_->black_ = false;
          z = z->parent_->parent_;
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
        auto y = z->parent_->parent_->left_.get();               // y is z's uncle
        if (y && !y->black_) {                                   // are z's parent and uncle both red?
          // case 1
          z->parent_->black_ = true;
          y->black_ = true;
          z->parent_->parent_->black_ = false;
          z = z->parent_->parent_;
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
    root_->black_ = true;
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

  void erase(Node *z) {
    if (!z) {
      return;
    }
    bool orig_black = z->black_;
    Node* x = nullptr;
    Node* xp = nullptr;
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
      } else {
        xp = y;
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
    // if any red-black violations occurred, correct them
    if (orig_black) {
      erase_fixup(x, xp);
    }
    size_--;
    verify();
  }

  void erase_fixup(Node* x, Node* xp) {
    while (x != root_.get() && (!x || x->black_)) {
      assert(xp);
      if (x == xp->left_.get()) {     // is x a left child?
        auto w = xp->right_.get();    // w is x's sibling
        assert(w);
        if (!w->black_) {
          // case 1
          w->black_ = true;
          xp->black_ = false;
          left_rotate(xp);
          w = xp->right_.get();
        }
        if ((!w->left_ || w->left_->black_) && (!w->right_ || w->right_->black_)) {
          // case 2
          w->black_ = false;
          x = xp;
          xp = xp->parent_;
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
          left_rotate(xp);
          x = root_.get();
        }
      } else { // same as the above, but with "right" and "left" exchanged
        auto w = xp->left_.get();    // w is x's sibling
        assert(w);
        if (!w->black_) {
          // case 1
          w->black_ = true;
          xp->black_ = false;
          right_rotate(xp);
          w = xp->left_.get();
        }
        if ((!w->left_ || w->left_->black_) && (!w->right_ || w->right_->black_)) {
          // case 2
          w->black_ = false;
          x = xp;
          xp = xp->parent_;
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
          right_rotate(xp);
          x = root_.get();
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

  Node* find_node(const K& key) const {
    auto res = find_lower_bound(key);
    if (res.bound_ && proj(res.bound_->key_) == key) {
      return res.bound_;
    } else {
      return nullptr;
    }
  }

  SearchResult find_lower_bound(const K& key) const {
    auto x = root_.get();
    SearchResult res;
    while (x) {
      res.parent_ = x;
      if (Comp{}(proj(x->key_), key)) {
        res.where_ = RBChild::Right;
        x = x->right_.get();
      } else {
        res.where_ = RBChild::Left;
        res.bound_ = x;
        x = x->left_.get();
      }
    }
    return res;
  }

  SearchResult find_upper_bound(const K& key) const {
    auto x = root_.get();
    SearchResult res;
    while (x) {
      if (Comp{}(key, proj(x->key_))) {
        res.where_ = RBChild::Left;
        res.bound_ = x;
        x = x->left_.get();
      } else {
        res.where_ = RBChild::Right;
        x = x->right_.get();
      }
    }
    return res;
  }

  pair<Node*, Node*> find_equal_range(const K& key) const {
    Node* lo = nullptr;
    Node* hi = nullptr;

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

  Node* insert_node(unique_ptr<Node> z, const SearchResult& res) {
    z->parent_ = res.parent_;              // found the location - insert z with parent y
    Node* z_ptr = nullptr;
    if (!z->parent_) {
      root_ = move(z);                     // tree was empty.
      z_ptr = root_.get();
    } else if (res.where_ == RBChild::Left) {
      res.parent_->left_ = move(z);
      z_ptr = res.parent_->left_.get();
    } else {
      res.parent_->right_ = move(z);
      z_ptr = res.parent_->right_.get();
    }
    assert(z_ptr);
    insert_fixup(z_ptr);
    size_++;
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
      auto node = first.node_;
      ++first;
      erase(node);
      cnt++;
    }
    return cnt;
  }

public:
  iterator_type find(const K& key) {
    return iterator_type(find_node(key));
  }

  const_iterator_type find(const K& key) const {
    return const_iterator_type(find_node(key));
  }

  bool contains(const K& key) const {
    return find_node(key) != nullptr;
  }

  iterator_type lower_bound(const K& key) {
    return iterator_type(find_lower_bound(key).bound_);
  }

  const_iterator_type lower_bound(const K& key) const {
    return const_iterator_type(find_lower_bound(key).bound_);
  }

  iterator_type upper_bound(const K& key) {
    return iterator_type(find_upper_bound(key).bound_);
  }

  const_iterator_type upper_bound(const K& key) const {
    return const_iterator_type(find_upper_bound(key).bound_);
  }

  pair<iterator_type, iterator_type> equal_range(const K& key) {
    auto res = find_equal_range(key);
    return {iterator_type(res.first), iterator_type(res.second)};
  }

  pair<const_iterator_type, const_iterator_type> equal_range(const K& key) const {
    auto res = find_equal_range(key);
    return {const_iterator_type(res.first), const_iterator_type(res.second)};
  }

  template <typename T>
  conditional_t<AllowDup, iterator_type, pair<iterator_type, bool>>
  insert(T&& key) requires is_same_v<remove_cvref_t<T>, V> {
    SearchResult res;
    if constexpr(AllowDup) {
      res = find_upper_bound(proj(key));
    } else {
      res = find_lower_bound(proj(key));
      if (res.bound_ && proj(res.bound_->key_) == proj(key)) {
        return {iterator_type(res.bound_), false};
      }
    }

    auto z = make_unique<Node>(forward<T>(key));
    auto z_ptr = insert_node(move(z), res);
    return {iterator_type(z_ptr), true};
  }

  template <typename T>
  add_lvalue_reference_t<decltype(declval<V>().second)>
  operator[](T&& raw_key) requires (!is_set_ && !AllowDup) {
    K key {forward<T>(raw_key)};
    SearchResult res;
    res = find_lower_bound(key);
    if (res.bound_ && proj(res.bound_->key_) == key) {
      return res.bound_->key_.second;
    }

    auto z = make_unique<Node>({move(key), {}});
    auto z_ptr = insert_node(move(z), res);
    return z_ptr->key_.second;
  }

  iterator_type erase(iterator_type iter) {
    if (iter == end()) {
      throw invalid_argument("attempt to erase end()");
    }
    auto node = iter.node_;
    ++iter;
    erase(node);
    return iter;
  }

  size_t erase(const K& key) {
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

  friend ostream &operator<<(ostream &os, const RedBlackTree &tree) {
    inorder_print(os, tree.root_.get());
    return os;
  }
};

} // namespace detail

template <Containable K, typename Comp = less<K>>
using TreeSet = detail::RedBlackTree<K, K, Comp, false>;

template <Containable K, typename Comp = less<K>>
using TreeMultiSet = detail::RedBlackTree<K, K, Comp, true>;

template <Containable K, Containable V, typename Comp = less<K>>
using TreeMap = detail::RedBlackTree<K, pair<const K, V>, Comp, false>;

template <Containable K, Containable V, typename Comp = less<K>>
using TreeMultiMap = detail::RedBlackTree<K, pair<const K, V>, Comp, true>;

} // namespace frozenca::hard

#endif //__CLRS4_RED_BLACK_TREE_H__
