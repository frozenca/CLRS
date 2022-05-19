#include <binarytree.h>
#include <iostream>
#include <memory>
#include <utility>

namespace fc = frozenca;
using namespace std;

template <typename T> void preorder(const fc::BinaryTreeNode<T> *node) {
  if (!node) {
    return;
  }
  enum class From { Parent, LeftChild, RightChild };

  From pred = From::Parent;
  auto curr = node;
  while (curr) {
    if (pred == From::Parent) {
      cout << curr->key_ << ' ';
      if (curr->left_) {
        curr = curr->left_.get();
        pred = From::Parent;
      } else {
        pred = From::LeftChild;
      }
    } else if (pred == From::LeftChild) {
      if (curr->right_) {
        curr = curr->right_.get();
        pred = From::Parent;
      } else {
        pred = From::RightChild;
      }
    } else {
      if (!curr->parent_) {
        curr = nullptr;
        break;
      }
      if (curr == curr->parent_->left_.get()) {
        pred = From::LeftChild;
      } else {
        pred = From::RightChild;
      }
      curr = curr->parent_;
    }
  }
}

int main() {

  using Node = fc::BinaryTreeNode<int>;

  unique_ptr<Node> root;
  istringstream istr{"15 17 22 # # 13 12 # # 28 # # 20 25 # 33 14 # # # #"};
  istr >> root;

  preorder(root.get());
}