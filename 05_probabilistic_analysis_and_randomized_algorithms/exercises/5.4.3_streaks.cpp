#include <05_probabilistic_analysis_and_randomized_algorithms/inc/easy.h>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <numbers>
#include <random>
#include <utility>
#include <vector>


int main() {
  namespace fc = frozenca;
  using namespace std;
  mt19937 gen(random_device{}());
  constexpr int N = 100;
  constexpr int trials = 1'000;
  bernoulli_distribution coin(0.5);
  int longest_streak_count = 0;
  for (int t = 0; t < trials; t++) {
    int longest_streak = 0;
    int current_streak = 0;
    for (int i = 0; i < N; i++) {
      if (coin(gen)) {
        current_streak++;
      } else {
        longest_streak = max(longest_streak, current_streak);
        current_streak = 0;
      }
    }
    longest_streak = max(longest_streak, current_streak);
    longest_streak_count += longest_streak;
  }

  cout << "Expected number of longest streak : "
       << longest_streak_count / static_cast<float>(trials) << '\n';
  cout << "ln N : " << log(N);
}
