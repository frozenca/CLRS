#ifndef __CLRS4_BST_SUCCESSOR_H__
#define __CLRS4_BST_SUCCESSOR_H__

#include <cassert>
#include <common.h>
#include <iostream>
#include <stdexcept>

namespace frozenca {

using namespace std;


template <typename T> struct BSTSuccNode {
  T key_ = T{};
  BSTSuccNode *succ_ = nullptr;
  unique_ptr<BSTSuccNode> left_;
  unique_ptr<BSTSuccNode> right_;
  BSTSuccNode() = default;
  BSTSuccNode(const T &key) : key_{key} {}

  BSTSuccNode(const BSTSuccNode &node) = delete;
  BSTSuccNode &operator=(const BSTSuccNode &node) = delete;
  BSTSuccNode(BSTSuccNode &&node) = delete;
  BSTSuccNode &operator=(BSTSuccNode &&node) = delete;

  friend void succ_print(ostream& os, const BSTSuccNode* node) {
    if (node) {
      os << node->key_ << ' ';
      succ_print(os, node->succ_);
    }
  }
};

template <typename T, typename Comp = less<T>> class BSTSuccessor {
  using Node = BSTSuccNode<T>;

  unique_ptr<Node> root_;

  static void verify(const Node *node) {
    if (!node) {
      return;
    }
    assert(!node->left_ || (Comp{}(node->left_->key_, node->key_)));
    assert(!node->right_ || (Comp{}(node->key_, node->right_->key_)));
    verify(node->left_.get());
    verify(node->right_.get());
  }

  void verify() const { verify(root_.get()); }

public:
  BSTSuccessor() = default;
  BSTSuccessor(const BSTSuccessor &other) = delete;
  BSTSuccessor &operator=(const BSTSuccessor &other) = delete;
  BSTSuccessor(BSTSuccessor &&other) = delete;
  BSTSuccessor &operator=(BSTSuccessor &&other) = delete;

  Node *tree_search(const T &key) const {
    return tree_search(root_.get(), key);
  }

  void tree_insert(const T &key) {
    auto z = make_unique<Node>(key);
    auto x = root_.get();
    Node *y = nullptr;
    Node *pred = nullptr;
    while (x) {
      y = x;
      if (Comp{}(z->key_, x->key_)) {
        x = x->left_.get();
      } else {
        pred = x;
        x = x->right_.get();
      }
    }
    if (!y) {
      root_ = move(z);
    } else if (Comp{}(key, y->key_)) {
      y->left_ = move(z);
      if (pred) {
        pred->succ_ = y->left_.get();
      }
      y->left_->succ_ = y;
    } else {
      y->right_ = move(z);
      y->right_->succ_ = y->succ_;
      y->succ_ = y->right_.get();
    }
    verify();
  }

private:
  static Node *tree_minimum(Node *node) {
    while (node && node->left_) {
      node = node->left_.get();
    }
    return node;
  }

  static Node *tree_maximum(Node *node) {
    while (node && node->right_) {
      node = node->right_.get();
    }
    return node;
  }

public:
  void tree_delete(const T &key) {
    auto x = root_.get();
    Node *parent = nullptr;
    Node *pred = nullptr;
    while (x && key != x->key_) {
      parent = x;
      if (Comp{}(key, x->key_)) {
        x = x->left_.get();
      } else {
        pred = x;
        x = x->right_.get();
      }
    }
    if (!x) {
      return;
    }
    if (!x->left_) {
      // replace x by its right child
      if (!parent) {
        root_ = move(x->right_);
      } else if (x == parent->left_.get()) {
        parent->left_ = move(x->right_);
        if (pred) {
          if (parent->left_) {
            pred->succ_ = tree_minimum(parent->left_.get());
          } else {
            pred->succ_ = parent;
          }
        }
      } else {
        auto succ = x->succ_;
        parent->right_ = move(x->right_);
        parent->succ_ = succ;
      }
    } else if (!x->right_) {
      // replace x by its left child
      if (!parent) {
        root_ = move(x->left_);
        pred = tree_maximum(root_.get());
        if (pred) {
          pred->succ_ = nullptr;
        }
      } else if (x == parent->left_.get()) {
        parent->left_ = move(x->left_);
        pred = tree_maximum(parent->left_.get());
        if (pred) {
          pred->succ_ = parent;
        }
      } else {
        auto succ = x->succ_;
        parent->right_ = move(x->left_);
        parent->succ_ = tree_minimum(parent->right_.get());
        pred = tree_maximum(parent->right_.get());
        pred->succ_ = succ;
      }
    } else {
      auto y = x->right_.get();
      auto yp = x;
      while (y->left_) {
        yp = y;
        y = y->left_.get();
      }
      unique_ptr<Node> y_holder;
      if (y != x->right_.get()) {
        unique_ptr<Node> yr = move(y->right_);
        y_holder = move(yp->left_);
        yp->left_ = move(yr);
        y_holder->right_ = move(x->right_);
        if (yp->left_) {
          y_holder->succ_ = tree_minimum(yp->left_.get());
        } else {
          y_holder->succ_ = yp;
        }
      }
      if (!y_holder) {
        assert(yp == x);
        y_holder = move(x->right_);
      }
      auto xl = move(x->left_);
      pred = tree_maximum(xl.get());
      if (!parent) {
        root_ = move(y_holder);
        y = root_.get();
      } else if (x == parent->left_.get()) {
        parent->left_ = move(y_holder);
        y = parent->left_.get();
      } else {
        parent->right_ = move(y_holder);
        y = parent->right_.get();
      }
      pred->succ_ = y;
      y->left_ = move(xl);
    }
    verify();
  }

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

  friend ostream &operator<<(ostream &os, const BSTSuccessor<T> &tree) {
    auto node = tree.root_.get();
    while (node && node->left_) {
      node = node->left_.get();
    }
    succ_print(os, node);
    return os;
  }
};

} // namespace frozenca

#endif //__CLRS4_BST_SUCCESSOR_H__
