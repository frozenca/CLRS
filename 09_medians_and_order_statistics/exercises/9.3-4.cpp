#include <algorithm>
#include <cassert>
#include <common.h>
#include <iostream>
#include <ranges>
#include <utility>
#include <vector>


int main() {

  namespace fc = frozenca;
  using namespace std;
  vector<int> v{4, 2, 1, 5, 3, 6};
  ranges::nth_element(v, v.begin() + v.size() / 2);
  cout << "Smaller elements: ";
  for (fc::index_t i = 0; i < ssize(v) / 2; ++i) {
    cout << v[i] << ' ';
  }
  cout << "\nLarger elements: ";
  for (fc::index_t i = ssize(v) / 2 + 1; i < ssize(v); ++i) {
    cout << v[i] << ' ';
  }
}