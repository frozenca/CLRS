#ifndef __CLRS4_BINARY_TREE_INORDER_H__
#define __CLRS4_BINARY_TREE_INORDER_H__

#include <binarysearchtree.h>
#include <common.h>
#include <iostream>

namespace frozenca {

using namespace std;

template <typename K> void inorder(const BinaryTreeNode<K> *node) {
  if (node) {
    inorder(node->left_.get());
    cout << node->key_ << ' ';
    inorder(node->right_.get());
  }
}

template <typename K> void inorder_iterative(const BinaryTreeNode<K> *node) {
  bool left_done = false;
  while (node) {
    if (!left_done) {
      while (node->left_) {
        node = node->left_.get();
      }
    }
    left_done = true;
    cout << node->key_ << ' ';
    if (node->right_) {
      left_done = false;
      node = node->right_.get();
    } else if (node->parent_) {
      while (node->parent_ && node == node->parent_->right_.get()) {
        node = node->parent_;
      }
      if (!node->parent_) {
        break;
      }
      node = node->parent_;
    } else {
      break;
    }
  }
}

template <typename K> void preorder(const BinaryTreeNode<K> *node) {
  if (node) {
    cout << node->key_ << ' ';
    preorder(node->left_.get());
    preorder(node->right_.get());
  }
}

template <typename K> void postorder(const BinaryTreeNode<K> *node) {
  if (node) {
    postorder(node->left_.get());
    postorder(node->right_.get());
    cout << node->key_ << ' ';
  }
}

} // namespace frozenca

#endif //__CLRS4_BINARY_TREE__INORDER_H__
