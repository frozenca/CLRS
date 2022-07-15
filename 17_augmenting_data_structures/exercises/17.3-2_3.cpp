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

  cout << *tree.overlapping_lower_bound({10, 20}) << '\n';
  cout << *tree.overlapping_upper_bound({10, 20}) << '\n';

  auto [it1, it2] = tree.overlapping_equal_range({10, 20});

  for (auto it = it1; it != it2; ++it) {
    cout << *it << ' ';
  }
  cout << '\n';
}
