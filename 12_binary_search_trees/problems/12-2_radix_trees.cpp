#include <12_binary_search_trees/inc/easy.h>
#include <iostream>

int main() {
  namespace fc = frozenca;
  using namespace std;

  fc::RadixTree tree;
  tree.tree_insert("10");
  tree.tree_insert("011");
  tree.tree_insert("0");
  tree.tree_insert("1011");
  tree.tree_insert("100");
  cout << tree << '\n';
}