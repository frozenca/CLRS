#include <binarytree.h>
#include <iostream>
#include <memory>
#include <sstream>
#include <stack>
#include <utility>


namespace fc = frozenca;
using namespace std;

template <typename T>
void inorder_nonrecursive(const fc::BinaryTreeNode<T> *node) {
  if (!node) {
    return;
  }
  stack<const fc::BinaryTreeNode<T> *> s;
  s.push(node);
  while (!s.empty()) {
    auto x = s.top();
    while (x) {
      s.push(x->left_.get());
      x = s.top();
    }
    s.pop();
    if (!s.empty()) {
      x = s.top();
      s.pop();
      cout << x->key_ << ' ';
      s.push(x->right_.get());
    }
  }
}

int main() {

  using Node = fc::BinaryTreeNode<int>;

  unique_ptr<Node> root;
  istringstream istr{"15 17 22 # # 13 12 # # 28 # # 20 25 # 33 14 # # # #"};
  istr >> root;

  inorder_nonrecursive(root.get());
}