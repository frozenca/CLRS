#include <05_probabilistic_analysis_and_randomized_algorithms/inc/easy.h>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <numeric>
#include <random>
#include <utility>
#include <vector>

int main() {
  namespace fc = frozenca;
  using namespace std;
  mt19937 gen(random_device{}());
  constexpr int N = 100;
  constexpr int trials = 10'000;
  vector<int> v(N);
  iota(v.begin(), v.end(), 1);

  for (int K = 1; K < N; K++) {
    int total_best_score = 0;
    for (int t = 0; t < trials; t++) {
      auto u = v;
      ranges::shuffle(u, gen);
      int best_score = *ranges::max_element(u | views::take(K));
      for (int i = K; i < N; i++) {
        if (u[i] > best_score) {
          total_best_score += u[i];
          break;
        }
      }
    }
    cout << "Average best score with K " << K << " and N " << N << " : "
         << total_best_score / static_cast<float>(trials) << '\n';
  }
}