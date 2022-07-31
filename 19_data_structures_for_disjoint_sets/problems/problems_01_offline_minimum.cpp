#include <19_data_structures_for_disjoint_sets/inc/easy.h>
#include <cassert>
#include <iostream>
#include <vector>

int main() {
  namespace fc = frozenca;
  using namespace std;

  vector<int> v{4, 8, -1, 3, -1, 9, 2, 6, -1, -1, -1, 1, 7, -1, 5};
  auto v2 = fc::offline_minimum(v, 6, 9);

  for (auto n : v2) {
    cout << n << ' ';
  }
}
