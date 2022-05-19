#include <binarytree.h>
#include <iostream>
#include <memory>
#include <sstream>
#include <utility>

namespace fc = frozenca;
using namespace std;

template <typename T>
void inorder_recursive(const fc::BinaryTreeNode<T> *node) {
  if (node) {
    inorder_recursive(node->left_.get());
    cout << node->key_ << ' ';
    inorder_recursive(node->right_.get());
  }
}

int main() {

  using Node = fc::BinaryTreeNode<int>;

  unique_ptr<Node> root;
  istringstream istr{"15 17 22 # # 13 12 # # 28 # # 20 25 # 33 14 # # # #"};
  istr >> root;

  inorder_recursive(root.get());
}