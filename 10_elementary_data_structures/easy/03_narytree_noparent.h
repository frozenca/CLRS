#ifndef __CLRS4_NARY_TREE_NOPARENT_H__
#define __CLRS4_NARY_TREE_NOPARENT_H__

#include <common.h>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

namespace frozenca {

using namespace std;

template <typename T> struct NaryTreeNoParentNode {
  T key_ = T{};
  NaryTreeNoParentNode* left_child_ = nullptr;
  NaryTreeNoParentNode* right_sibling_ = nullptr;
  bool is_rightmost_ = false;
  NaryTreeNoParentNode() = default;
  NaryTreeNoParentNode(const T &key) : key_{key} {}

  ~NaryTreeNoParentNode() {
    auto child = left_child_;
    while (child) {
      auto next = child->right_sibling_;
      auto is_rightmost = child->is_rightmost_;
      delete child;
      if (is_rightmost) {
        break;
      } else {
        child = next;
      }
    }
  }

  friend istream &operator>>(istream &is, NaryTreeNoParentNode*&node) {
    string val;
    is >> val;
    if (val == "#" || val == "$") {
      return is;
    }
    istringstream istr{val};
    T v;
    istr >> v;
    node = new NaryTreeNoParentNode(v);
    is >> node->left_child_;
    auto curr = node->left_child_;
    while (curr) {
      is >> curr->right_sibling_;
      if (!curr->right_sibling_) {
        curr->is_rightmost_ = true;  
        curr->right_sibling_ = node;
        break;
      }
      curr = curr->right_sibling_;
    }
    return is;
  }

  friend ostream &operator<<(ostream &os, const NaryTreeNoParentNode *node) {
    if (node) {
      os << node->key_ << ' ';
      auto curr = node->left_child_;
      while (curr) {
        os << curr;
        curr = curr->is_rightmost_ ? nullptr : curr->right_sibling_;
      }
      os << "$ ";
    } else {
      os << "# ";
    }
    return os;
  }
};

} // namespace frozenca

#endif //__CLRS4_NARY_TREE_NOPARENT_H__
