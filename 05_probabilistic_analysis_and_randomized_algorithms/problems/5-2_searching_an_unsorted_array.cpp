#include <05_probabilistic_analysis_and_randomized_algorithms/inc/easy.h>
#include <iostream>
#include <random>
#include <numeric>
#include <unordered_set>
#include <vector>

using namespace std;

int main() {
  namespace fc = frozenca;
  mt19937 gen(random_device{}());
  constexpr int N = 100;
  constexpr int trials = 1000;
  uniform_int_distribution<> ind(0, N - 1);

  int total_count = 0;
  vector<int> v(N);
  iota(v.begin(), v.end(), 1);
  for (int t = 0; t < trials; t++) {
    unordered_set<int> s;
    while (s.size() < N) {
      total_count++;
      int new_index = ind(gen);
      if (v[new_index] == 1) {
        break;
      }
      s.insert(new_index);
    }
  }
  cout << "Expected value of indices of RANDOM-SEARCH : "
       << total_count / static_cast<float>(trials) << '\n';

  constexpr int K = 3;
  for (int k = 0; k < K; k++) {
    v[k] = 1;
  }
  total_count = 0;
  for (int t = 0; t < trials; t++) {
    unordered_set<int> s;
    while (s.size() < N) {
      total_count++;
      int new_index = ind(gen);
      if (v[new_index] == 1) {
        break;
      }
      s.insert(new_index);
    }
  }
  cout << "Expected value of indices of RANDOM-SEARCH where " << K
       << " duplicates : " << total_count / static_cast<float>(trials) << '\n';

  total_count = 0;
  for (int t = 0; t < trials; t++) {
    unordered_set<int> s;
    while (s.size() < N) {
      total_count++;
      int new_index = ind(gen);
      if (v[new_index] == N + 1) {
        break;
      }
      s.insert(new_index);
    }
  }
  cout << "Expected value of indices of RANDOM-SEARCH where zero desired "
          "elements : "
       << total_count / static_cast<float>(trials) << '\n';

  total_count = 0;
  iota(v.begin(), v.end(), 1);
  for (int t = 0; t < trials; t++) {
    ranges::shuffle(v, gen);
    for (int i = 0; i < N; i++) {
      if (v[i] == 1) {
        total_count += (i + 1);
        break;
      }
    }
  }
  cout << "Expected value of indices of DETERMINISTIC-SEARCH : "
       << total_count / static_cast<float>(trials) << '\n';

  iota(v.begin(), v.end(), 1);
  for (int k = 0; k < K; k++) {
    v[k] = 1;
  }
  total_count = 0;
  for (int t = 0; t < trials; t++) {
    ranges::shuffle(v, gen);
    for (int i = 0; i < N; i++) {
      if (v[i] == 1) {
        total_count += (i + 1);
        break;
      }
    }
  }
  cout << "Expected value of indices of DETERMINISTIC-SEARCH where " << K
       << " duplicates : " << total_count / static_cast<float>(trials) << '\n';

  total_count = 0;
  for (int t = 0; t < trials; t++) {
    total_count += N;
  }
  cout << "Expected value of indices of DETERMINISTIC-SEARCH where zero "
          "desired elements : "
       << total_count / static_cast<float>(trials) << '\n';

  iota(v.begin(), v.end(), 1);
  for (int k = 0; k < K; k++) {
    v[k] = 1;
  }
  total_count = 0;
  for (int t = 0; t < trials; t++) {
    ranges::shuffle(v, gen);
    for (int i = 0; i < N; i++) {
      if (v[i] == 1) {
        total_count += (i + 1);
        break;
      }
    }
  }
  cout << "Expected value of indices of SCRAMBLED-SEARCH where " << K
       << " duplicates : " << total_count / static_cast<float>(trials) << '\n';
}