#include <17_augmenting_data_structures/inc/easy.h>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <random>
#include <ranges>
#include <vector>

namespace fc = frozenca;
using namespace std;

int main() {
  constexpr int max_n = 20;
  mt19937 gen(random_device{}());
  vector<int> v(max_n);
  iota(v.begin(), v.end(), 0);

  fc::BHTree<int> h;

  ranges::shuffle(v, gen);
  for (auto num : v) {
    h.insert(num);
    h.verify();
  }

  ranges::shuffle(v, gen);
  for (auto num : v) {
    h.erase(num);
    h.verify();
  }
}
