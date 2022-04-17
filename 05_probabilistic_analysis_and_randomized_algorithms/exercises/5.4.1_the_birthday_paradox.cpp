#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <random>
#include <utility>
#include <vector>
#include <05_probabilistic_analysis_and_randomized_algorithms/inc/easy.h>

using namespace std;

int main() {
  namespace fc = frozenca;
  mt19937 gen(random_device{}());
  uniform_int_distribution<> birthday(1, 365);
  constexpr fc::index_t N = 23;
  constexpr int trials = 10'000;
  int same_birthday_count = 0;
  for (int t = 0; t < trials; t++) {
    vector<int> people(N);
    for (fc::index_t i = 0; i < N; i++) {
      people[i] = birthday(gen);
    }
    ranges::sort(people);
    auto ret = ranges::unique(people);
    people.erase(ret.begin(), ret.end());
    if (ssize(people) < N) {
      same_birthday_count++;
    }
  }

  cout << "Probability that at least 2 people have the same birthday : "
       << same_birthday_count / static_cast<float>(trials) << '\n';
  cout << "1 - exp(-k(k-1)/2n) : "
       << 1.0f - exp(-static_cast<float>(N) * (N - 1) / (2 * 365));
}