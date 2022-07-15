#include <17_augmenting_data_structures/inc/easy.h>
#include <iostream>

int main() {
  namespace fc = frozenca;
  using namespace std;

  fc::EndpointTree tree;
  tree.insert_interval({0.0f, 10.0f});
  tree.insert_interval({1.0f, 9.0f});
  tree.insert_interval({2.0f, 8.0f});
  tree.insert_interval({3.0f, 7.0f});
  tree.insert_interval({4.0f, 6.0f});
  tree.insert_interval({5.0f, 5.0f});
  cout << tree.get_attr().where_ << '\n';
  tree.erase_interval({5.0f, 5.0f});
  cout << tree.get_attr().where_ << '\n';
}
