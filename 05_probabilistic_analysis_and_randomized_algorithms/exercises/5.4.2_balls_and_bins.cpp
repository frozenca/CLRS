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
  constexpr fc::index_t N = 100;
  constexpr int trials = 10'000;
  uniform_int_distribution<> toss(0, N - 1);
  int toss_count = 0;
  for (int t = 0; t < trials; t++) {
    vector<int> bin(N);
    while (true) {
      bin[toss(gen)] = 1;
      ++toss_count;
      if (ranges::all_of(bin, [](int i) { return i; })) {
        break;
      }
    }
  }

  cout << "Expected number of tosses : "
       << toss_count / static_cast<float>(trials) << '\n';
  cout << "N (ln N + egamma + (1/(2*N))) : "
       << N * (log(N) + numbers::egamma_v<float> +
               1 / (2 * static_cast<float>(N)));
}