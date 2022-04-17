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

  int one_count = 0;
  int n_count = 0;
  for (int i = 0; i < num_trials; ++i) {
    vector<int> candidates(num_candidates);
    iota(candidates.begin(), candidates.end(), 1);
    ranges::shuffle(candidates, gen);
    auto hired = fc::hire_assistant(candidates);
    if (hired.size() == 1) {
      ++one_count;
    } else if (hired.size() == num_candidates) {
      ++n_count;
    }
  }
  cout << "The probability that you hire exactly one time " << one_count / static_cast<float>(num_trials) << '\n';
  cout << "The probability that you hire exactly n times " << n_count / static_cast<float>(num_trials) << '\n';
}