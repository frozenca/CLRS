#include <17_augmenting_data_structures/inc/easy.h>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <random>
#include <ranges>
#include <vector>

namespace fc = frozenca;
using namespace std;

void track_verify(const fc::TrackTree<int> &h) {
  vector<int> v;
  for (auto num : h) {
    v.push_back(num);
  }

  auto seq1 = h.track_sequence_fwd();
  auto seq2 = h.track_sequence_bwd();
  assert(v == seq1);
  assert(v == seq2);
}

int main() {
  constexpr int max_n = 200;
  mt19937 gen(random_device{}());
  vector<int> v(max_n);
  iota(v.begin(), v.end(), 0);

  fc::TrackTree<int> h;

  ranges::shuffle(v, gen);
  for (auto num : v) {
    h.insert(num);
    track_verify(h);
  }

  ranges::shuffle(v, gen);
  for (auto num : v) {
    h.erase(num);
    track_verify(h);
  }
}
