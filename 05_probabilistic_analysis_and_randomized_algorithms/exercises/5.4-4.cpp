#include <05_probabilistic_analysis_and_randomized_algorithms/inc/easy.h>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <numbers>
#include <random>
#include <utility>
#include <vector>


using namespace std;

int main() {
  namespace fc = frozenca;
  mt19937 gen(random_device{}());
  constexpr fc::index_t bins = 100;
  constexpr int trials = 10'000;
  uniform_int_distribution<> toss(0, bins - 1);
  int toss_count = 0;
  for (int t = 0; t < trials; t++) {
    vector<int> bin(bins);
    while (true) {
      ++bin[toss(gen)];
      ++toss_count;
      if (ranges::any_of(bin, [](int i) { return i >= 2; })) {
        break;
      }
    }
  }

  cout << "Expected number of tosses : "
       << toss_count / static_cast<float>(trials) << '\n';
}