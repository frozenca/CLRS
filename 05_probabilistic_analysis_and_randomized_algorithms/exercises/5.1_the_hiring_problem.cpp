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
  vector<int> candidates(20);
  iota(candidates.begin(), candidates.end(), 1);
  ranges::shuffle(candidates, gen);
  fc::print(candidates);

  auto hired = fc::hire_assistant(candidates);
  fc::print(hired);
}