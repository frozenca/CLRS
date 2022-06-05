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
  BinaryTreeNode(T key) : key_{move(key)} {}

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

  BinaryTreeNode(const BinaryTreeNode &node) = delete;
  BinaryTreeNode &operator=(const BinaryTreeNode &node) = delete;
  BinaryTreeNode(BinaryTreeNode &&node) = delete;
  BinaryTreeNode &operator=(BinaryTreeNode &&node) = delete;

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

} // namespace frozenca

#endif //__CLRS4_BINARY_TREE_H__
