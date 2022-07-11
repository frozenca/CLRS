#include <17_augmenting_data_structures/inc/easy.h>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <random>
#include <ranges>
#include <vector>

namespace fc = frozenca;
using namespace std;

void order_rank_verify(const fc::RankTree<int> &h) {
  auto n = h.size();
  vector<fc::index_t> v(n);
  iota(v.begin(), v.end(), 0);
  vector<fc::index_t> indices;
  for (auto num : h) {
    indices.push_back(h.index(num));
  }
  assert(v == indices);
}

int main() {
  constexpr int max_n = 200;
  mt19937 gen(random_device{}());
  vector<int> v(max_n);
  iota(v.begin(), v.end(), 0);

  fc::RankTree<int> h;

  ranges::shuffle(v, gen);
  for (auto num : v) {
    h.insert(num);
    order_rank_verify(h);
  }

  ranges::shuffle(v, gen);
  for (auto num : v) {
    h.erase(num);
    order_rank_verify(h);
  }
}
