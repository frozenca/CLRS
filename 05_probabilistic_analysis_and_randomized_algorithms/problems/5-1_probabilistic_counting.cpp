#include <05_probabilistic_analysis_and_randomized_algorithms/inc/easy.h>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

int main() {
  namespace fc = frozenca;
  mt19937 gen(random_device{}());
  constexpr int N = 94;
  constexpr int trials = 1000;
  uniform_int_distribution<> incr(1, N);

  int total_num = 0;
  for (int t = 0; t < trials; t++) {
    vector<int> seq(N);
    int cur_num = 0;
    for (int i = 0; i < N; i++) {
      cur_num += incr(gen);
      seq[i] = cur_num;
    }
    cur_num = 0;
    int cur_index = 0;
    for (int i = 0; i < N; i++) {
      bernoulli_distribution cur_incr(
          1.0 / static_cast<float>(seq[cur_index + 1] - seq[cur_index]));
      if (cur_incr(gen)) {
        cur_num += seq[cur_index + 1] - seq[cur_index];
        cur_index++;
      }
    }
    total_num += cur_num;
  }
  cout << "Expected value of counter after N increment operations : "
       << total_num / static_cast<float>(trials);
}