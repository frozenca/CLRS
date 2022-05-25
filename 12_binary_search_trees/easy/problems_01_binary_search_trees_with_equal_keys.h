#ifndef __CLRS4_BST_EQUAL_KEYS_H__
#define __CLRS4_BST_EQUAL_KEYS_H__

#include <binarytree.h>
#include <cassert>
#include <common.h>
#include <iostream>
#include <stdexcept>
#include <vector>


namespace frozenca {

using namespace std;

template <typename T> struct BSTNodeFlag {
  T key_ = T{};
  BSTNodeFlag *parent_ = nullptr;
  unique_ptr<BSTNodeFlag> left_;
  unique_ptr<BSTNodeFlag> right_;
  bool b_ = false;

  BSTNodeFlag() = default;
  BSTNodeFlag(const T &key) : key_{key} {}

  void clone(const BSTNodeFlag &other) {
    key_ = other.key_;
    if (other.left_) {
      left_ = make_unique<BSTNodeFlag>();
      left_->clone(*other.left_);
      left_->parent_ = this;
    }
    if (other.right_) {
      right_ = make_unique<BSTNodeFlag>();
      right_->clone(*other.right_);
      right_->parent_ = this;
    }
  }

  void steal(BSTNodeFlag &&other) {
    key_ = move(other.key_);
    if (other.left_) {
      left_ = move(other.left_);
      left_->parent_ = this;
    }
    if (other.right_) {
      right_ = move(other.right_);
      right_->parent_ = this;
    }
  }

  BSTNodeFlag(const BSTNodeFlag &node) { this->clone(node); }
  BSTNodeFlag &operator=(const BSTNodeFlag &node) {
    this->clone(node);
    return *this;
  }
  BSTNodeFlag(BSTNodeFlag &&node) noexcept { this->steal(move(node)); }
  BSTNodeFlag &operator=(BSTNodeFlag &&node) noexcept {
    this->steal(move(node));
    return *this;
  }

  friend void inorder_print(ostream &os, const BSTNodeFlag *node) {
    if (node) {
      inorder_print(os, node->left_.get());
      os << node->key_ << ' ';
      inorder_print(os, node->right_.get());
    }
  }
};

template <typename T, typename Comp = less<T>> class BSTEqualAlternate {
  using Node = BSTNodeFlag<T>;

  unique_ptr<Node> root_;

public:
  BSTEqualAlternate() = default;
  BSTEqualAlternate(const BSTEqualAlternate &other) {
    if (other->root_) {
      root_ = make_unique<Node>();
      root_->clone(*(other->root_));
    }
  }
  BSTEqualAlternate &operator=(const BSTEqualAlternate &other) {
    if (other->root_) {
      root_->clone(*(other->root_));
    }
    return *this;
  }
  BSTEqualAlternate(BSTEqualAlternate &&other) noexcept = default;
  BSTEqualAlternate &operator=(BSTEqualAlternate &&other) noexcept = default;

  void tree_insert(const T &key) {
    auto z = make_unique<Node>(key);
    auto x = root_.get(); // node being compared with z
    Node *y = nullptr;    // y will be parent of z
    while (x) {           // descend until reaching a leaf
      y = x;
      if (Comp{}(z->key_, x->key_)) {
        x = x->left_.get();
      } else if (z->key_ == x->key_) {
        break;
      } else {
        x = x->right_.get();
      }
    }
    z->parent_ = y;
    if (!y) {
      root_ = move(z); // tree was empty.
    } else if (Comp{}(key, y->key_)) {
      y->left_ = move(z);
    } else if (key == y->key_) {
      unique_ptr<Node> y_holder;
      Node *parent = y->parent_;
      if (!parent) {
        y_holder = move(root_);
        y_holder->b_ = !(y_holder->b_);
        y_holder->parent_ = z.get();
        if (y_holder->b_) {
          z->left_ = move(y_holder);
        } else {
          z->right_ = move(y_holder);
        }
        z->parent_ = nullptr;
        root_ = move(z);
      } else if (y == parent->left_.get()) {
        y_holder = move(parent->left_);
        y_holder->b_ = !(y_holder->b_);
        y_holder->parent_ = z.get();
        if (y_holder->b_) {
          z->left_ = move(y_holder);
        } else {
          z->right_ = move(y_holder);
        }
        z->parent_ = parent;
        parent->left_ = move(z);
      } else {
        y_holder = move(parent->right_);
        y_holder->b_ = !(y_holder->b_);
        y_holder->parent_ = z.get();
        if (y_holder->b_) {
          z->left_ = move(y_holder);
        } else {
          z->right_ = move(y_holder);
        }
        z->parent_ = parent;
        parent->right_ = move(z);
      }
    } else {
      y->right_ = move(z);
    }
  }

public:
  friend ostream &operator<<(ostream &os, const BSTEqualAlternate<T> &tree) {
    inorder_print(os, tree.root_.get());
    return os;
  }
};

template <typename T> struct BSTNodeList {
  T key_ = T{};
  BSTNodeList *parent_ = nullptr;
  unique_ptr<BSTNodeList> left_;
  unique_ptr<BSTNodeList> right_;
  vector<unique_ptr<BSTNodeList>> equal_nodes_;

  BSTNodeList() = default;
  BSTNodeList(const T &key) : key_{key} {}

  BSTNodeList(const BSTNodeList &node) = delete;
  BSTNodeList &operator=(const BSTNodeList &node) = delete;
  BSTNodeList(BSTNodeList &&node) = delete;
  BSTNodeList &operator=(BSTNodeList &&node) = delete;

  friend void inorder_print(ostream &os, const BSTNodeList *node) {
    if (node) {
      inorder_print(os, node->left_.get());
      os << node->key_ << ' ';
      for (auto &&equal_node : node->equal_nodes_) {
        os << equal_node->key_ << ' ';
      }
      inorder_print(os, node->right_.get());
    }
  }
};

template <typename T, typename Comp = less<T>> class BSTEqualList {
  using Node = BSTNodeList<T>;

  unique_ptr<Node> root_;

public:
  BSTEqualList() = default;
  BSTEqualList(const BSTEqualList &other) = delete;
  BSTEqualList &operator=(const BSTEqualList &other) = delete;
  BSTEqualList(BSTEqualList &&other) = delete;
  BSTEqualList &operator=(BSTEqualList &&other) = delete;

  void tree_insert(const T &key) {
    auto z = make_unique<Node>(key);
    auto x = root_.get(); // node being compared with z
    Node *y = nullptr;    // y will be parent of z
    while (x) {           // descend until reaching a leaf
      y = x;
      if (Comp{}(z->key_, x->key_)) {
        x = x->left_.get();
      } else if (z->key_ == x->key_) {
        break;
      } else {
        x = x->right_.get();
      }
    }
    z->parent_ = y; // found the location - insert z with parent y
    if (!y) {
      root_ = move(z); // tree was empty.
    } else if (Comp{}(key, y->key_)) {
      y->left_ = move(z);
    } else if (key == y->key_) {
      y->equal_nodes_.push_back(move(z));
    } else {
      y->right_ = move(z);
    }
  }

public:
  friend ostream &operator<<(ostream &os, const BSTEqualList<T> &tree) {
    inorder_print(os, tree.root_.get());
    return os;
  }
};

template <typename T, typename Comp = less<T>> class BSTEqualRandom {
  using Node = BinaryTreeNode<T>;

  unique_ptr<Node> root_;

public:
  BSTEqualRandom() = default;
  BSTEqualRandom(const BSTEqualRandom &other) {
    if (other->root_) {
      root_ = make_unique<Node>();
      root_->clone(*(other->root_));
    }
  }
  BSTEqualRandom &operator=(const BSTEqualRandom &other) {
    if (other->root_) {
      root_->clone(*(other->root_));
    }
    return *this;
  }
  BSTEqualRandom(BSTEqualRandom &&other) noexcept = default;
  BSTEqualRandom &operator=(BSTEqualRandom &&other) noexcept = default;

  void tree_insert(const T &key) {
    auto z = make_unique<Node>(key);
    auto x = root_.get(); // node being compared with z
    Node *y = nullptr;    // y will be parent of z
    mt19937 gen(random_device{}());
    bernoulli_distribution dist(0.5);
    bool set_left = dist(gen);
    while (x) { // descend until reaching a leaf
      y = x;
      if (Comp{}(z->key_, x->key_)) {
        x = x->left_.get();
      } else if (z->key_ == x->key_) {
        break;
      } else {
        x = x->right_.get();
      }
    }
    z->parent_ = y; // found the location - insert z with parent y
    if (!y) {
      root_ = move(z); // tree was empty.
    } else if (Comp{}(key, y->key_)) {
      y->left_ = move(z);
    } else if (key == y->key_) {
      unique_ptr<Node> y_holder;
      Node *parent = y->parent_;
      if (!parent) {
        y_holder = move(root_);
        y_holder->parent_ = z.get();
        if (set_left) {
          z->left_ = move(y_holder);
        } else {
          z->right_ = move(y_holder);
        }
        z->parent_ = nullptr;
        root_ = move(z);
      } else if (y == parent->left_.get()) {
        y_holder = move(parent->left_);
        y_holder->parent_ = z.get();
        if (set_left) {
          z->left_ = move(y_holder);
        } else {
          z->right_ = move(y_holder);
        }
        z->parent_ = parent;
        parent->left_ = move(z);
      } else {
        y_holder = move(parent->right_);
        y_holder->parent_ = z.get();
        if (set_left) {
          z->left_ = move(y_holder);
        } else {
          z->right_ = move(y_holder);
        }
        z->parent_ = parent;
        parent->right_ = move(z);
      }
    } else {
      y->right_ = move(z);
    }
  }

public:
  friend ostream &operator<<(ostream &os, const BSTEqualRandom<T> &tree) {
    inorder_print(os, tree.root_.get());
    return os;
  }
};

} // namespace frozenca

#endif //__CLRS4_BST_EQUAL_KEYS_H__
