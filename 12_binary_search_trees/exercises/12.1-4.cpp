#include <12_binary_search_trees/inc/easy.h>
#include <iostream>
#include <sstream>
#include <memory>

int main() {

  namespace fc = frozenca;
  using namespace std;
  
  using Node = fc::BinaryTreeNode<int>;
  unique_ptr<Node> root;
  istringstream istr{
      "1 2 4 # # 5 # # 3 6 # # 7 # # "};
  istr >> root;

  fc::preorder(root.get());
  fc::postorder(root.get());

}