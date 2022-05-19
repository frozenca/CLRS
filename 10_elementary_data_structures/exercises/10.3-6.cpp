#include <10_elementary_data_structures/inc/easy.h>
#include <iostream>
#include <memory>
#include <sstream>
#include <utility>


namespace fc = frozenca;
using namespace std;

template <typename T>
fc::NaryTreeNoParentNode<T> *
get_parent(const fc::NaryTreeNoParentNode<T> *node) {
  if (!node) {
    return nullptr;
  }
  auto curr = node;
  while (!curr->is_rightmost_) {
    curr = curr->right_sibling_;
  }
  return curr->right_sibling_;
}

int main() {

  using Node = fc::NaryTreeNoParentNode<int>;

  Node *root = nullptr;
  istringstream istr{
      "1 2 5 $ 6 12 $ $ $ 3 7 $ 8 $ 9 13 $ 14 $ $ 10 $ $ 4 11 $ $ $"};
  istr >> root;

  cout << get_parent(root->left_child_);

  delete root;
}