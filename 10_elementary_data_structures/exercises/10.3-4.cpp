#include <iostream>
#include <memory>
#include <narytree.h>
#include <sstream>
#include <utility>

namespace fc = frozenca;
using namespace std;

template <typename T> void preorder_recursive(const fc::NaryTreeNode<T> *node) {
  if (node) {
    cout << node->key_ << ' ';
    auto curr = node->left_child_.get();
    while (curr) {
      preorder_recursive(curr);
      curr = curr->right_sibling_.get();
    }
  }
}

int main() {

  using Node = fc::NaryTreeNode<int>;

  unique_ptr<Node> root;
  istringstream istr{
      "1 2 5 $ 6 12 $ $ $ 3 7 $ 8 $ 9 13 $ 14 $ $ 10 $ $ 4 11 $ $ $"};
  istr >> root;

  preorder_recursive(root.get());
}