#ifndef __CLRS4_BINARY_TREE_H__
#define __CLRS4_BINARY_TREE_H__

#include <common.h>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

namespace frozenca {

using namespace std;

template <typename T> struct BinaryTreeNode {
  T key_ = T{};
  BinaryTreeNode *parent_ = nullptr;
  unique_ptr<BinaryTreeNode> left_;
  unique_ptr<BinaryTreeNode> right_;
  BinaryTreeNode() = default;
  BinaryTreeNode(const T &key) : key_{key} {}

  void clone(const BinaryTreeNode &other) {
    key_ = other.key_;
    if (other.left_) {
      left_ = make_unique<BinaryTreeNode>();
      left_->clone(*other.left_);
      left_->parent_ = this;
    }
    if (other.right_) {
      right_ = make_unique<BinaryTreeNode>();
      right_->clone(*other.right_);
      right_->parent_ = this;
    }
  }

  void steal(BinaryTreeNode &&other) {
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

  BinaryTreeNode(const BinaryTreeNode &node) { this->clone(node); }
  BinaryTreeNode &operator=(const BinaryTreeNode &node) {
    this->clone(node);
    return *this;
  }
  BinaryTreeNode(BinaryTreeNode &&node) noexcept { this->steal(move(node)); }
  BinaryTreeNode &operator=(BinaryTreeNode &&node) noexcept {
    this->steal(move(node));
    return *this;
  }

  friend istream &operator>>(istream &is, unique_ptr<BinaryTreeNode> &node) {
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
    node = make_unique<BinaryTreeNode>(v);
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

  friend ostream &operator<<(ostream &os, const BinaryTreeNode *node) {
    if (node) {
      os << node->key_ << ' ';
      os << node->left_.get();
      os << node->right_.get();
    } else {
      os << "# ";
    }
    return os;
  }

  friend void inorder_print(ostream& os, const BinaryTreeNode* node) {
    if (node) {
      inorder_print(os, node->left_.get());
      os << node->key_ << ' ';
      inorder_print(os, node->right_.get());
    }
  }
};

template <typename K, typename V> struct BinaryTreeMapNode {
  K key_ = K{};
  V value_ = V{};
  BinaryTreeMapNode *parent_ = nullptr;
  unique_ptr<BinaryTreeMapNode> left_;
  unique_ptr<BinaryTreeMapNode> right_;
  BinaryTreeMapNode() = default;
  BinaryTreeMapNode(const K &key, const V &value) : key_{key}, value_{value} {}
  void clone(const BinaryTreeMapNode &other) {
    key_ = other.key_;
    value_ = other.value_;
    if (other.left_) {
      left_ = make_unique<BinaryTreeMapNode>();
      left_->clone(*other.left_);
      left_->parent_ = this;
    }
    if (other.right_) {
      right_ = make_unique<BinaryTreeMapNode>();
      right_->clone(*other.right_);
      right_->parent_ = this;
    }
  }

  void steal(BinaryTreeMapNode &&other) {
    key_ = move(other.key_);
    key_ = move(other.value_);
    if (other.left_) {
      left_ = move(other.left_);
      left_->parent_ = this;
    }
    if (other.right_) {
      right_ = move(other.right_);
      right_->parent_ = this;
    }
  }

  BinaryTreeMapNode(const BinaryTreeMapNode &node) { this->clone(node); }
  BinaryTreeMapNode &operator=(const BinaryTreeMapNode &node) {
    this->clone(node);
    return *this;
  }
  BinaryTreeMapNode(BinaryTreeMapNode &&node) noexcept {
    this->steal(move(node));
  }
  BinaryTreeMapNode &operator=(BinaryTreeMapNode &&node) noexcept {
    this->steal(move(node));
    return *this;
  }

  friend istream &operator>>(istream &is, unique_ptr<BinaryTreeMapNode> &node) {
    string val;
    is >> val;
    if (val == "#") {
      return is;
    }
    istringstream istr{val};
    char ch;
    istr >> ch;
    if (ch != '[') {
      return is;
    }
    K k;
    if (!(istr >> k)) {
      return is;
    }
    istr >> ch;
    if (ch != ':') {
      return is;
    }
    V v;
    if (!(istr >> v)) {
      return is;
    }
    istr >> ch;
    if (ch != ']') {
      return is;
    }
    node = make_unique<BinaryTreeMapNode>(k, v);
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

  friend ostream &operator<<(ostream &os, const BinaryTreeMapNode *node) {
    if (node) {
      os << "[ " << node->key_ << " : " << node->value_ << " ] ";
      os << node->left_.get();
      os << node->right_.get();
    } else {
      os << "# ";
    }
    return os;
  }

  friend void inorder_print(ostream& os, const BinaryTreeMapNode* node) {
    if (node) {
      inorder_print(os, node->left_.get());
      os << "[ " << node->key_ << " : " << node->value_ << " ] ";
      inorder_print(os, node->right_.get());
    }
  }
};

} // namespace frozenca

#endif //__CLRS4_BINARY_TREE_H__
