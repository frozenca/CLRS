#include <17_augmenting_data_structures/inc/easy.h>
#include <iomanip>
#include <iostream>

int main() {
  namespace fc = frozenca;
  using namespace std;

  fc::IntervalTree tree;

  tree.insert({16, 21});
  tree.insert({8, 9});
  tree.insert({5, 8});
  tree.insert({0, 3});
  tree.insert({6, 10});
  tree.insert({15, 23});
  tree.insert({25, 30});
  tree.insert({17, 19});
  tree.insert({26, 26});
  tree.insert({19, 20});

  cout << tree.get_attr() << '\n';
  cout << tree.size() << '\n';

  tree.erase({25, 30});

  cout << tree.get_attr() << '\n';
  cout << tree.size() << '\n';
}
