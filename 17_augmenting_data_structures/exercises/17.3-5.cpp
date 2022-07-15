#include <17_augmenting_data_structures/inc/easy.h>
#include <iomanip>
#include <iostream>

int main() {
  namespace fc = frozenca;
  using namespace std;

  fc::GapTree<float> tree;

  tree.insert(1);
  tree.insert(5);
  tree.insert(9);
  tree.insert(15);
  tree.insert(18);
  tree.insert(22);
  cout << tree.get_attr().min_gap_ << '\n';
}
