#include <05_probabilistic_analysis_and_randomized_algorithms/inc/easy.h>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <numbers>
#include <random>
#include <unordered_map>
#include <utility>
#include <vector>

using namespace std;

int main() {
  namespace fc = frozenca;
  mt19937 gen(random_device{}());
  constexpr int N = 94;
  constexpr int trials = 1000;
  uniform_int_distribution<> toss(0, N - 1);
  int empty_bin_count = 0;
  int one_bin_count = 0;
  for (int t = 0; t < trials; ++t) {
    vector<int> bin(N);
    for (int i = 0; i < N; ++i) {
      ++bin[toss(gen)];
    }
    for (auto balls : bin) {
      if (balls == 0) {
        ++empty_bin_count;
      } else if (balls == 1) {
        ++one_bin_count;
      }
    }
  }

  cout << "Expected number of empty bins : "
       << empty_bin_count / static_cast<float>(trials) << '\n';
  cout << "n ((n-1)/n)^n : " << (N * pow(static_cast<float>(N - 1) / N, N))
       << '\n';
  cout << "Expected number of bins with exactly one ball : "
       << one_bin_count / static_cast<float>(trials) << '\n';
  cout << "n ((n-1)/n)^(n-1) : "
       << (N * pow(static_cast<float>(N - 1) / N, N - 1)) << '\n';
}