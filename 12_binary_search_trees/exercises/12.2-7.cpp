#include <12_binary_search_trees/inc/easy.h>
#include <iostream>
#include <sstream>
#include <memory>

namespace fc = frozenca;
using namespace std;

using Node = fc::BinaryTreeNode<int>;
using BST = fc::BinarySearchTree<int>;

template <typename K>
void inorder_BST(fc::BinaryTreeNode<K>* root) {
  auto node = fc::BinarySearchTree<K>::tree_minimum(root);
  while (node) {
    cout << node->key_ << ' ';
    node = fc::BinarySearchTree<K>::tree_successor(node);
  }
}

int main() {

  unique_ptr<Node> root;
  istringstream istr{
      "4 2 1 # # 3 # # 5 6 # # 7 # # "};
  istr >> root;

  inorder_BST(root.get());

}