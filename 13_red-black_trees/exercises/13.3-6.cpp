#include <13_red-black_trees/inc/easy.h>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

int main() {
  namespace fc = frozenca;
  using namespace std;

  fc::RBTreeNoParent<int> tree1;
  tree1.insert_noparent(1);
  tree1.insert_noparent(2);
  tree1.insert_noparent(3);
  tree1.insert_noparent(4);
  tree1.insert_noparent(5);

  tree1.print();
}
