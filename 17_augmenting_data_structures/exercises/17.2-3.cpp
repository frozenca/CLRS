#include <17_augmenting_data_structures/inc/easy.h>
#include <iostream>

namespace fc = frozenca;
using namespace std;

int main() {
  fc::AttrStatisticTree<int> tree;
  for (int i = 0; i < 10; ++i) {
    tree.insert(i);
  }
  cout << tree.get_attr() << '\n';
  for (int i = 0; i < 5; ++i) {
    tree.erase(i);
  }
  cout << tree.get_attr() << '\n';
  for (int i = 5; i < 10; ++i) {
    tree.erase(i);
  }
  cout << tree.get_attr() << '\n';

  fc::StringConcatTree strtree;
  strtree.insert("a");
  strtree.insert("e");
  strtree.insert("d");
  strtree.insert("b");
  strtree.insert("c");
  cout << strtree.get_attr() << '\n';
  strtree.erase("c");
  strtree.erase("b");
  cout << strtree.get_attr() << '\n';
}
