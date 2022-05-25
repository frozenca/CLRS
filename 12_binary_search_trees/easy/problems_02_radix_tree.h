#ifndef __CLRS4_RADIX_TREE_H__
#define __CLRS4_RADIX_TREE_H__

#include <common.h>
#include <binarytree.h>
#include <memory>
#include <string>

namespace frozenca {

using namespace std;


class RadixTree {
  using Node = BinaryTreeNode<string>;

  unique_ptr<Node> root_;

public:
  RadixTree() = default;
  RadixTree(const RadixTree &other) {
    if (other.root_) {
      root_ = make_unique<Node>();
      root_->clone(*(other.root_));
    }
  }
  RadixTree &operator=(const RadixTree &other) {
    if (other.root_) {
      root_->clone(*(other.root_));
    }
    return *this;
  }
  RadixTree(RadixTree &&other) noexcept = default;
  RadixTree &operator=(RadixTree &&other) noexcept = default;

  void tree_insert(const string& key) {
    if (!root_) {
      root_ = make_unique<Node>();
    }
    auto x = root_.get(); // node being compared with z
    for (char b : key) {
      if (b == '0') {
        if (!x->left_) {
          x->left_ = make_unique<Node>();
          x->left_->parent_ = x;
        }
        x = x->left_.get();
      } else if (b == '1') {
        if (!x->right_) {
          x->right_ = make_unique<Node>();
          x->right_->parent_ = x;
        }
        x = x->right_.get();
      } else {
        throw invalid_argument("invalid bit string\n");
      }
    }
    x->key_ = key;
  }

  friend ostream &operator<<(ostream &os, const RadixTree &tree) {
    inorder_print(os, tree.root_.get());
    return os;
  }
};

} // namespace frozenca

#endif //__CLRS4_RADIX_TREE_H__
