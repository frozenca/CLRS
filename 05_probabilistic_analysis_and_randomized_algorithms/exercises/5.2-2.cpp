#include <05_probabilistic_analysis_and_randomized_algorithms/inc/easy.h>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

int main() {
  namespace fc = frozenca;
  using namespace std;

  mt19937 gen(random_device{}());

  constexpr int num_candidates = 10;
  constexpr int num_trials = 10'000'000;

  int two_count = 0;
  for (int i = 0; i < num_trials; ++i) {
    vector<int> candidates(num_candidates);
    iota(candidates.begin(), candidates.end(), 1);
    ranges::shuffle(candidates, gen);
    auto hired = fc::hire_assistant(candidates);
    if (hired.size() == 2) {
      ++two_count;
    }
  }
  cout << "The probability that you hire exactly two times " << two_count / static_cast<float>(num_trials) << '\n';
}