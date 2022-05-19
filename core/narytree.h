#ifndef __CLRS4_NARY_TREE_H__
#define __CLRS4_NARY_TREE_H__

#include <common.h>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

namespace frozenca {

using namespace std;

template <typename T> struct NaryTreeNode {
  T key_ = T{};
  NaryTreeNode *parent_ = nullptr;
  unique_ptr<NaryTreeNode> left_child_;
  unique_ptr<NaryTreeNode> right_sibling_;
  NaryTreeNode() = default;
  NaryTreeNode(const T &key) : key_{key} {}

  friend istream &operator>>(istream &is, unique_ptr<NaryTreeNode> &node) {
    string val;
    is >> val;
    if (val == "#" || val == "$") {
      return is;
    }
    istringstream istr{val};
    T v;
    istr >> v;
    node = make_unique<NaryTreeNode>(v);
    is >> node->left_child_;
    auto curr = node->left_child_.get();
    if (curr) {
      curr->parent_ = node.get();
    }
    while (curr) {
      is >> curr->right_sibling_;
      curr = curr->right_sibling_.get();
      if (curr) {
        curr->parent_ = node.get();
      }
    }
    return is;
  }

  friend ostream &operator<<(ostream &os, const NaryTreeNode *node) {
    if (node) {
      os << node->key_ << ' ';
      auto curr = node->left_child_.get();
      while (curr) {
        os << curr;
        curr = curr->right_sibling_.get();
      }
      os << "$ ";
    } else {
      os << "# ";
    }
    return os;
  }
};

} // namespace frozenca

#endif //__CLRS4_NARY_TREE_H__
