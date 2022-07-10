#ifndef __CLRS4_BINARY_SEARCH_TREE_H__
#define __CLRS4_BINARY_SEARCH_TREE_H__

#include <binarytree.h>
#include <cassert>
#include <common.h>
#include <iostream>
#include <random>
#include <stdexcept>

namespace frozenca {

using namespace std;

template <typename T, typename Comp = less<T>> class BinarySearchTree {
  using Node = BinaryTreeNode<T>;

  unique_ptr<Node> root_;

  static void verify(const Node *node) {
    if (!node) {
      return;
    }
    assert(!node->left_ || (node->left_->parent_ == node &&
                            Comp{}(node->left_->key_, node->key_)));
    assert(!node->right_ || (node->right_->parent_ == node &&
                             Comp{}(node->key_, node->right_->key_)));
    verify(node->left_.get());
    verify(node->right_.get());
  }

  void verify() const { verify(root_.get()); }

public:
  BinarySearchTree() = default;
  BinarySearchTree(const BinarySearchTree &other) {
    if (other->root_) {
      root_ = make_unique<Node>();
      root_->clone(*(other->root_));
    }
  }
  BinarySearchTree &operator=(const BinarySearchTree &other) {
    if (other->root_) {
      root_->clone(*(other->root_));
    }
    return *this;
  }
  BinarySearchTree(BinarySearchTree &&other) noexcept = default;
  BinarySearchTree &operator=(BinarySearchTree &&other) noexcept = default;

  Node *tree_search(const T &key) const {
    return tree_search(root_.get(), key);
  }

  Node *tree_search_iterative(const T &key) const {
    return tree_search_iterative(root_.get(), key);
  }

  [[nodiscard]] bool contains(const T &key) const {
    return (tree_search(key) != nullptr);
  }

  Node *tree_minimum() const { return tree_minimum(root_.get()); }

  Node *tree_maximum() const { return tree_maximum(root_.get()); }

  Node *tree_minimum_recursive() const {
    return tree_minimum_recursive(root_.get());
  }

  Node *tree_maximum_recursive() const {
    return tree_maximum_recursive(root_.get());
  }

  int tree_height() const { return tree_height(root_.get()); }

  void tree_insert(const T &key) {
    auto z = make_unique<Node>(key);
    auto x = root_.get(); // node being compared with z
    Node *y = nullptr;    // y will be parent of z
    while (x) {           // descend until reaching a leaf
      y = x;
      if (Comp{}(z->key_, x->key_)) {
        x = x->left_.get();
      } else {
        x = x->right_.get();
      }
    }
    z->parent_ = y; // found the location - insert z with parent y
    if (!y) {
      root_ = move(z); // tree was empty.
    } else if (Comp{}(key, y->key_)) {
      y->left_ = move(z);
    } else {
      y->right_ = move(z);
    }
    verify();
  }

  void insert(const T &key) { tree_insert(key); }

  void tree_insert_recursive(const T &key) {
    auto z = make_unique<Node>(key);
    tree_insert_recursive(root_.get(), nullptr, move(z));
    verify();
  }

private:
  static int tree_height(Node *root) {
    if (!root) {
      return 0;
    }
    auto l_height = tree_height(root->left_.get());
    auto r_height = tree_height(root->right_.get());
    return max(l_height, r_height) + 1;
  }

  void tree_insert_recursive(Node *curr, Node *parent, unique_ptr<Node> z) {
    assert(z);
    if (!curr) {
      z->parent_ = parent;
      if (!parent) {
        root_ = move(z);
      } else if (Comp{}(z->key_, parent->key_)) {
        parent->left_ = move(z);
      } else {
        parent->right_ = move(z);
      }
    } else if (Comp{}(z->key_, curr->key_)) {
      tree_insert_recursive(curr->left_.get(), curr, move(z));
    } else {
      tree_insert_recursive(curr->right_.get(), curr, move(z));
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

  bool tree_delete(Node *z) {
    if (!z) {
      return false;
    }
    if (!z->left_) {
      transplant(z, move(z->right_)); // replace z by its right child
    } else if (!z->right_) {
      transplant(z, move(z->left_)); // replace z by its left child
    } else {
      auto y = tree_minimum(z->right_.get()); // y is z's successor
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
    }
    return true;
  }

  void tree_delete_pred(Node *z) {
    if (!z) {
      return;
    }
    if (!z->left_) {
      transplant(z, move(z->right_)); // replace z by its right child
    } else if (!z->right_) {
      transplant(z, move(z->left_)); // replace z by its left child
    } else {
      auto y = tree_maximum(z->left_.get()); // y is z's predecessor
      unique_ptr<Node> y_holder;
      if (y != z->left_.get()) { // is y farther down the tree?
        auto yp = y->parent_;
        assert(yp && y == yp->right_.get());
        y_holder = move(yp->right_);
        // replace y by its left child.
        yp->right_ = move(y->left_);
        if (yp->right_) {
          yp->right_->parent_ = yp;
        }
        y = y_holder.get();

        // z's left child becomes y's left child
        y->left_ = move(z->left_);
      }
      auto zr = move(z->right_); // extract right of z
      assert(zr);

      auto yp = y->parent_; // prepare to extract y
      assert(yp);
      if (!y_holder) {
        assert(yp == z);
        y_holder = move(yp->left_);
      }

      // replace z by its successor y
      // and give z's right child to y, which had no right child
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
      y->right_ = move(zr);
      y->right_->parent_ = y;
      if (y->left_) {
        y->left_->parent_ = y;
      }
    }
  }

  void tree_delete_fair(Node *z) {
    if (!z) {
      return;
    }
    if (!z->left_) {
      transplant(z, move(z->right_)); // replace z by its right child
    } else if (!z->right_) {
      transplant(z, move(z->left_)); // replace z by its left child
    } else {
      mt19937 gen(random_device{}());
      bernoulli_distribution dist(0.5);
      if (dist(gen)) {
        auto y = tree_minimum(z->right_.get()); // y is z's successor
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
      } else {
        auto y = tree_maximum(z->left_.get()); // y is z's predecessor
        unique_ptr<Node> y_holder;
        if (y != z->left_.get()) { // is y farther down the tree?
          auto yp = y->parent_;
          assert(yp && y == yp->right_.get());
          y_holder = move(yp->right_);
          // replace y by its left child.
          yp->right_ = move(y->left_);
          if (yp->right_) {
            yp->right_->parent_ = yp;
          }
          y = y_holder.get();

          // z's left child becomes y's left child
          y->left_ = move(z->left_);
        }
        auto zr = move(z->right_); // extract right of z
        assert(zr);

        auto yp = y->parent_; // prepare to extract y
        assert(yp);
        if (!y_holder) {
          assert(yp == z);
          y_holder = move(yp->left_);
        }

        // replace z by its successor y
        // and give z's right child to y, which had no right child
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
        y->right_ = move(zr);
        y->right_->parent_ = y;
        if (y->left_) {
          y->left_->parent_ = y;
        }
      }
    }
  }

public:
  void tree_delete(const T &key) {
    auto z = tree_search(key);
    tree_delete(z);
    verify();
  }

  void tree_delete_pred(const T &key) {
    auto z = tree_search(key);
    tree_delete_pred(z);
    verify();
  }

  void tree_delete_fair(const T &key) {
    auto z = tree_search(key);
    tree_delete_fair(z);
    verify();
  }

  bool erase(const T &key) { return tree_delete(tree_search(key)); }

  static Node *tree_search(Node *node, const T &key) {
    if (!node || key == node->key_) {
      return node;
    }
    if (Comp{}(key, node->key_)) {
      return tree_search(node->left_.get(), key);
    } else {
      return tree_search(node->right_.get(), key);
    }
  }

  static Node *tree_search_iterative(Node *node, const T &key) {
    while (node && key != node->key_) {
      if (Comp{}(key, node->key_)) {
        node = node->left_.get();
      } else {
        node = node->right_.get();
      }
    }
    return node;
  }

  static Node *tree_minimum(Node *node) {
    while (node->left_) {
      node = node->left_.get();
    }
    return node;
  }

  static Node *tree_maximum(Node *node) {
    while (node->right_) {
      node = node->right_.get();
    }
    return node;
  }

  static Node *tree_minimum_recursive(Node *node) {
    if (node && node->left_) {
      return tree_minimum_recursive(node->left_.get());
    } else {
      return node;
    }
  }

  static Node *tree_maximum_recursive(Node *node) {
    if (node && node->right_) {
      return tree_maximum_recursive(node->right_.get());
    } else {
      return node;
    }
  }

  static Node *tree_successor(Node *node) {
    if (node && node->right_) {
      return tree_minimum(
          node->right_.get()); // leftmost node in the right subtree
    } else { // find the lowest ancestor of x whose left child is an ancestor of
             // x
      auto parent = node->parent_;
      while (parent && node == parent->right_.get()) {
        node = parent;
        parent = parent->parent_;
      }
      return parent;
    }
  }

  static Node *tree_predecessor(Node *node) {
    if (node && node->left_) {
      return tree_maximum(
          node->left_.get()); // rightmost node in the left subtree
    } else { // find the lowest ancestor of x whose right child is an ancestor
             // of x
      auto parent = node->parent_;
      while (parent && node == parent->left_.get()) {
        node = parent;
        parent = parent->parent_;
      }
      return parent;
    }
  }

private:
  vector<T> inorder_walk() const {
    vector<T> walk;
    auto curr = tree_minimum();
    while (curr) {
      walk.push_back(curr->key_);
      curr = tree_successor(curr);
    }
    return walk;
  }

  unique_ptr<Node> construct_balanced_tree(const vector<T> &walk, index_t first,
                                           index_t last, float alpha) {
    if (first >= last || last > ssize(walk)) {
      return nullptr;
    }
    auto mid = first + static_cast<index_t>((last - first) * alpha);
    auto root = make_unique<Node>(walk[mid]);
    root->left_ = construct_balanced_tree(walk, first, mid, alpha);
    if (root->left_) {
      root->left_->parent_ = root.get();
    }
    root->right_ = construct_balanced_tree(walk, mid + 1, last, alpha);
    if (root->right_) {
      root->right_->parent_ = root.get();
    }
    return root;
  }

public:
  void tree_balance(float alpha = 0.5) {
    if (!root_) {
      return;
    }
    auto walk = inorder_walk();
    auto new_root = construct_balanced_tree(walk, 0, ssize(walk), alpha);
    swap(root_, new_root);
  }

  friend ostream &operator<<(ostream &os, const BinarySearchTree<T> &tree) {
    inorder_print(os, tree.root_.get());
    return os;
  }
};

} // namespace frozenca

#endif //__CLRS4_BINARY_SEARCH_TREE_H__
