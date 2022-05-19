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

  friend istream& operator>>(istream& is, unique_ptr<BinaryTreeNode>& node) {
    string val;
    is >> val;
    if (val == "#") {
      return is;
    }
    istringstream istr {val};
    T v;
    istr >> v;
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

  friend ostream& operator<<(ostream& os, const BinaryTreeNode* node) {
    if (node) {
      os << node->key_ << ' ';
      os << node->left_.get();
      os << node->right_.get();
    } else {
      os << "# ";
    }
    return os;
  }
};

} // namespace frozenca

#endif //__CLRS4_BINARY_TREE_H__
