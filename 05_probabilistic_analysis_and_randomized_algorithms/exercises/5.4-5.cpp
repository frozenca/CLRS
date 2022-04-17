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
  uniform_int_distribution<> birthday(1, 365);
  constexpr int N = 94;
  constexpr int trials = 1000;
  int same_birthday_count = 0;
  for (int t = 0; t < trials; t++) {
    vector<int> people(N);
    unordered_map<int, int> freq;
    for (int i = 0; i < N; i++) {
      people[i] = birthday(gen);
      ++freq[people[i]];
    }
    int max_freq = 0;
    for (const auto &[day, ppl] : freq) {
      max_freq = max(max_freq, ppl);
    }
    if (max_freq >= 3) {
      ++same_birthday_count;
    }
  }

  cout << "Probability that at least 3 people have the same birthday with " << N
       << " peoples : " << same_birthday_count / static_cast<float>(trials)
       << '\n';
}