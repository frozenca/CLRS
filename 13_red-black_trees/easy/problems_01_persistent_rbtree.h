#ifndef __CLRS4_PERSISTENT_RB_TREE_H__
#define __CLRS4_PERSISTENT_RB_TREE_H__

#include <algorithm>
#include <cassert>
#include <common.h>
#include <memory>
#include <string>
#include <vector>

namespace frozenca {

using namespace std;

enum class RBChild {
  Unused,
  Left,
  Right,
};

template <Containable T> struct RBTreePersistentNode {
  T key_ = T{};
  bool black_ = false;
  shared_ptr<RBTreePersistentNode> left_;
  shared_ptr<RBTreePersistentNode> right_;
  RBTreePersistentNode() = default;
  RBTreePersistentNode(T key) : key_{move(key)} {}
  ~RBTreePersistentNode() = default;

  RBTreePersistentNode(const RBTreePersistentNode &node) = default;
  RBTreePersistentNode &operator=(const RBTreePersistentNode &node) = default;
  RBTreePersistentNode(RBTreePersistentNode &&node) = default;
  RBTreePersistentNode &operator=(RBTreePersistentNode &&node) = default;

  friend void inorder_print(ostream &os, const RBTreePersistentNode *node) {
    if (node) {
      inorder_print(os, node->left_.get());
      os << node->key_ << ' ';
      inorder_print(os, node->right_.get());
    }
  }
};

template <Containable K> class RBTreePersistent;

template <Containable T> class RBTreePersistentIterator {
public:
  using difference_type = ptrdiff_t;
  using value_type = T;
  using pointer = const T *;
  using reference = const T &;
  using iterator_category = bidirectional_iterator_tag;
  using iterator_concept = iterator_category;
  using Node = RBTreePersistentNode<T>;
  using Tree = RBTreePersistent<T>;

public:
  Node *node_ = nullptr;

private:
  Tree *tree_ = nullptr;

public:
  RBTreePersistentIterator() = default;
  ~RBTreePersistentIterator() = default;
  RBTreePersistentIterator(Node *node, Tree *tree) : node_{node}, tree_{tree} {}

  RBTreePersistentIterator(const RBTreePersistentIterator &other) = default;
  RBTreePersistentIterator &
  operator=(const RBTreePersistentIterator &other) = default;
  RBTreePersistentIterator(RBTreePersistentIterator &&other) noexcept = default;
  RBTreePersistentIterator &
  operator=(RBTreePersistentIterator &&other) noexcept = default;

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
      Node *candidate = nullptr;
      Node *probe = tree_->root_.get();
      while (probe != node_) {
        if (node_->key_ < probe->key_) {
          candidate = probe;
          probe = probe->left_.get();
        } else {
          probe = probe->right_.get();
        }
      }
      node_ = candidate;
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
      Node *candidate = nullptr;
      Node *probe = tree_->root_.get();
      while (probe != node_) {
        if (node_->key_ > probe->key_) {
          probe = probe->left_.get();
        } else {
          candidate = probe;
          probe = probe->right_.get();
        }
      }
      if (candidate) {
        node_ = candidate;
      }
    }
  }

  RBTreePersistentIterator &operator++() {
    increment();
    return *this;
  }

  RBTreePersistentIterator operator++(int) {
    RBTreePersistentIterator temp = *this;
    increment();
    return temp;
  }

  RBTreePersistentIterator &operator--() {
    decrement();
    return *this;
  }

  RBTreePersistentIterator operator--(int) {
    RBTreePersistentIterator temp = *this;
    decrement();
    return temp;
  }

  [[nodiscard]] friend bool operator==(const RBTreePersistentIterator &lhs,
                                       const RBTreePersistentIterator &rhs) {
    return lhs.node_ == rhs.node_;
  }
};

template <Containable K> class RBTreePersistent {
  using Node = RBTreePersistentNode<K>;

  struct SearchResult {
    Node *bound_ = nullptr;
    RBChild where_ = RBChild::Unused;
  };

public:
  shared_ptr<Node> root_;

private:
  shared_ptr<Node> clone(shared_ptr<Node> original) {
    if (!original) {
      return nullptr;
    }
    shared_ptr<Node> new_node = make_shared<Node>(original->key_);
    new_node->black_ = original->black_;
    new_node->left_ = clone(original->left_);
    new_node->right_ = clone(original->right_);
    return new_node;
  }

  static int verify(const Node *node) {
    if (!node) {
      return 0;
    }

    auto left_black_height = verify(node->left_.get());
    auto right_black_height = verify(node->right_.get());

    // binary search tree properties
    assert(!node->left_ || (node->key_ >= node->left_->key_));
    assert(!node->right_ || (node->right_->key_ >= node->key_));

    // if a node is red, then both its children are black.
    assert(node->black_ || ((!node->left_ || node->left_->black_) &&
                            (!node->right_ || node->right_->black_)));

    // for each node, all simple paths from the node to
    // descendant leaves contain the same number of black nodes.
    assert(left_black_height == right_black_height);

    // to mute unused variable right_black_height compiler warning for release
    // builds..
    return ((left_black_height + right_black_height) / 2) +
           ((node->black_) ? 1 : 0);
  }

  void verify() const {
    // the root is black.
    assert(!root_ || root_->black_);
    verify(root_.get());
  }

public:
  using value_type = K;
  using reference_type = K &;
  using const_reference_type = const K &;
  using iterator_type = RBTreePersistentIterator<K>;
  using const_iterator_type = RBTreePersistentIterator<K>;
  using reverse_iterator_type = reverse_iterator<iterator_type>;
  using const_reverse_iterator_type = reverse_iterator<const_iterator_type>;

  friend class RBTreePersistentIterator<K>;

  iterator_type begin_;
  ptrdiff_t size_ = 0;

  RBTreePersistent() = default;
  RBTreePersistent(const RBTreePersistent &other) {
    this->root_ = clone(other.root_);
    this->size_ = other.size_;
  }
  RBTreePersistent &operator=(const RBTreePersistent &other) = default;
  RBTreePersistent(RBTreePersistent &&other) noexcept = default;
  RBTreePersistent &operator=(RBTreePersistent &&other) noexcept = default;

  [[nodiscard]] iterator_type begin() noexcept { return begin_; }

  [[nodiscard]] const_iterator_type begin() const noexcept {
    return const_iterator_type(begin_, this);
  }

  [[nodiscard]] const_iterator_type cbegin() const noexcept {
    return const_iterator_type(begin_, this);
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
  pair<RBChild, shared_ptr<Node>> get_child_relation(Node *xp, Node *x) {
    if (!xp) {
      return {RBChild::Unused, root_};
    } else if (x == xp->left_.get()) {
      return {RBChild::Left, xp->left_};
    } else if (x == xp->right_.get()) {
      return {RBChild::Right, xp->right_};
    } else {
      assert(false);
      return {RBChild::Unused, nullptr};
    }
  }

  void left_rotate(Node *xp, Node *x_raw) {
    auto [where, x] = get_child_relation(xp, x_raw);
    shared_ptr<Node> y = x->right_;
    x->right_ = y->left_;
    if (where == RBChild::Unused) {
      root_ = y;
    } else if (where == RBChild::Left) {
      xp->left_ = y;
    } else {
      xp->right_ = y;
    }
    y->left_ = x;
  }

  void right_rotate(Node *xp, Node *x_raw) {
    auto [where, x] = get_child_relation(xp, x_raw);
    shared_ptr<Node> y = x->left_;
    x->left_ = y->right_;
    if (where == RBChild::Unused) {
      root_ = y;
    } else if (where == RBChild::Left) {
      xp->left_ = y;
    } else {
      xp->right_ = y;
    }
    y->right_ = x;
  }

private:
  void insert_fixup(Node *z, vector<Node *> &nodes) {
    Node *zp = nodes.empty() ? nullptr : nodes.back();
    while (zp && !zp->black_) {
      nodes.pop_back();
      assert(!nodes.empty());
      auto zpp = nodes.back();
      nodes.pop_back();
      if (zp == zpp->left_.get()) { // is z's parent a left child?
        auto y = zpp->right_.get(); // y is z's uncle
        if (y && !y->black_) {      // are z's parent and uncle both red?
          // case 1
          zp->black_ = true;
          y->black_ = true;
          zpp->black_ = false;
          z = zpp;
          zp = nodes.empty() ? nullptr : nodes.back();
        } else {
          // case 2
          if (z == zp->right_.get()) {
            left_rotate(zpp, zp);
            swap(z, zp);
          }
          // case 3
          zp->black_ = true;
          zpp->black_ = false;
          auto zppp = nodes.empty() ? nullptr : nodes.back();
          nodes.clear();
          right_rotate(zppp, zpp);
        }
      } else if (zp == zpp->right_.get()) { // same as the above, but with
                                            // "right" and "left" exchanged
        auto y = zpp->left_.get();          // y is z's uncle
        if (y && !y->black_) { // are z's parent and uncle both red?
          // case 1
          zp->black_ = true;
          y->black_ = true;
          zpp->black_ = false;
          z = zpp;
          zp = nodes.empty() ? nullptr : nodes.back();
        } else {
          // case 2
          if (z == zp->left_.get()) {
            right_rotate(zpp, zp);
            swap(z, zp);
          }
          // case 3
          zp->black_ = true;
          zpp->black_ = false;
          auto zppp = nodes.empty() ? nullptr : nodes.back();
          nodes.clear();
          left_rotate(zppp, zpp);
        }
      } else {
        assert(false);
      }
    }
    root_->black_ = true;
  }

  void transplant(Node *u, Node *up, shared_ptr<Node> v) {
    assert(u);
    if (!up) {
      root_ = v;
    } else if (u == up->left_.get()) {
      up->left_ = v;
    } else {
      up->right_ = v;
    }
  }

  iterator_type erase(Node *z, vector<Node *> &nodes) {
    if (!z) {
      return {};
    }
    iterator_type it(z, this);
    ++it;
    if (begin_.node_ == z) {
      begin_ = it;
    }
    bool orig_black = z->black_;
    Node *x = nullptr;
    Node *zp = nodes.empty() ? nullptr : nodes.back();
    if (!z->left_) {
      x = z->right_.get();
      transplant(z, zp, z->right_); // replace z by its right child
    } else if (!z->right_) {
      x = z->left_.get();
      transplant(z, zp, z->left_); // replace z by its right child
    } else {
      auto [where_z, z_sh] = get_child_relation(zp, z);
      auto y = z->right_.get();
      Node *yp = z;
      index_t length = 0;
      while (y->left_) {
        length++;
        nodes.push_back(y);
        yp = y;
        y = y->left_.get();
      }
      orig_black = y->black_;
      x = y->right_.get();
      auto [where_y, y_sh] = get_child_relation(yp, y);
      nodes.push_back(y);
      if (y != z->right_.get()) { // is y farther down the tree?
        assert(!nodes.empty());
        transplant(y, yp, y->right_);
        y->right_ = z->right_;
        // reorder parent chain...
        rotate(nodes.rbegin(), nodes.rbegin() + 1, nodes.rbegin() + length + 1);
      }
      transplant(z, zp, y_sh);
      y->left_ = z->left_;
      y->black_ = z->black_;
    }
    // if any red-black violations occurred, correct them
    if (orig_black) {
      erase_fixup(x, nodes);
    }
    size_--;

    return it;
  }

  void erase_fixup(Node *x, vector<Node *> &nodes) {
    while (x != root_.get() && (!x || x->black_)) {
      assert(!nodes.empty());
      auto xp = nodes.back();
      nodes.pop_back();
      assert(xp);
      if (x == xp->left_.get()) {  // is x a left child?
        auto w = xp->right_.get(); // w is x's sibling
        assert(w);
        Node *xpp = nodes.empty() ? nullptr : nodes.back();
        if (!w->black_) {
          // case 1
          w->black_ = true;
          xp->black_ = false;
          Node *next_xpp = xp->right_.get();
          left_rotate(xpp, xp);
          w = xp->right_.get();
          if (next_xpp) {
            xpp = next_xpp;
            nodes.push_back(xpp);
          }
        }
        if ((!w->left_ || w->left_->black_) &&
            (!w->right_ || w->right_->black_)) {
          // case 2
          w->black_ = false;
          x = xp;
        } else {
          if (!w->right_ || w->right_->black_) {
            // case 3
            assert(w->left_);
            w->left_->black_ = true;
            w->black_ = false;
            right_rotate(xp, w);
            w = xp->right_.get();
          }
          // case 4
          assert(w->right_);
          w->black_ = xp->black_;
          xp->black_ = true;
          w->right_->black_ = true;
          left_rotate(xpp, xp);
          x = root_.get();
          nodes.clear();
        }
      } else if (x == xp->right_.get()) { // same as the above, but with "right"
                                          // and "left" exchanged
        auto w = xp->left_.get();         // w is x's sibling
        assert(w);
        Node *xpp = nodes.empty() ? nullptr : nodes.back();
        if (!w->black_) {
          // case 1
          w->black_ = true;
          xp->black_ = false;
          Node *next_xpp = xp->left_.get();
          right_rotate(xpp, xp);
          w = xp->left_.get();
          if (next_xpp) {
            xpp = next_xpp;
            nodes.push_back(xpp);
          }
        }
        if ((!w->left_ || w->left_->black_) &&
            (!w->right_ || w->right_->black_)) {
          // case 2
          w->black_ = false;
          x = xp;
        } else {
          if (!w->left_ || w->left_->black_) {
            // case 3
            assert(w->right_);
            w->right_->black_ = true;
            w->black_ = false;
            left_rotate(xp, w);
            w = xp->left_.get();
          }
          // case 4
          assert(w->left_);
          w->black_ = xp->black_;
          xp->black_ = true;
          w->left_->black_ = true;
          right_rotate(xpp, xp);
          x = root_.get();
          nodes.clear();
        }
      } else {
        assert(false);
      }
    }
    if (x) {
      x->black_ = true;
    }
  }

  Node *insert_node(shared_ptr<Node> z, const SearchResult &res,
                    vector<Node *> &nodes) {
    auto zp = nodes.empty() ? nullptr : nodes.back();
    if (!zp) {
      root_ = z;
    } else if (res.where_ == RBChild::Left) {
      zp->left_ = z;
    } else {
      zp->right_ = z;
    }
    insert_fixup(z.get(), nodes);
    size_++;
    if (!begin_.node_ || z->key_ < *begin_) {
      begin_ = iterator_type(z.get(), this);
    }
    return z.get();
  }

  Node *find_node(const K &key) const {
    auto res = find_lower_bound(key);
    if (res.bound_ && res.bound_->key_ == key) {
      return res.bound_;
    } else {
      return nullptr;
    }
  }

public:
  iterator_type find(const K &key) {
    return iterator_type(find_node(key), this);
  }

  const_iterator_type find(const K &key) const {
    return const_iterator_type(find_node(key), this);
  }

  bool contains(const K &key) const { return find_node(key) != nullptr; }

  SearchResult find_lower_bound(const K &key) const {
    auto x = root_.get();
    SearchResult res;
    while (x) {
      if (x->key_ < key) {
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

  SearchResult find_lower_bound(const K &key, vector<Node *> &nodes) const {
    auto x = root_.get();
    SearchResult res;
    while (x) {
      nodes.push_back(x);
      if (x->key_ < key) {
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

private:
  template <typename T>
  pair<iterator_type, bool>
  insert_value(T &&key) requires is_same_v<remove_cvref_t<T>, K> {
    vector<Node *> nodes;
    SearchResult res = find_lower_bound(key, nodes);
    if (res.bound_ && res.bound_->key_ == key) {
      return {iterator_type(res.bound_, this), false};
    }
    auto z = make_shared<Node>(forward<T>(key));
    auto z_ptr = insert_node(z, res, nodes);
    return {iterator_type(z_ptr, this), true};
  }

public:
  pair<iterator_type, bool> insert(const K &key) { return insert_value(key); }

  pair<iterator_type, bool> insert(K &&key) { return insert_value(move(key)); }

  size_t erase(const K &key) {
    vector<Node *> nodes;
    auto res = find_lower_bound(key, nodes);
    if (res.bound_ && res.bound_->key_ == key) {
      while (!nodes.empty() && nodes.back() != res.bound_) {
        nodes.pop_back();
      }
      if (!nodes.empty()) {
        nodes.pop_back();
      }
      erase(res.bound_, nodes);
      return 1;
    } else {
      return 0;
    }
  }

  friend ostream &operator<<(ostream &os, const RBTreePersistent &tree) {
    inorder_print(os, tree.root_.get());
    return os;
  }
};

} // namespace frozenca

#endif //__CLRS4_PERSISTENT_RB_TREE_H__
