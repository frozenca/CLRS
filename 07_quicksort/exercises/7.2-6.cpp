#include <07_quicksort/inc/easy.h>
#include <algorithm>
#include <chrono>
#include <iostream>
#include <numeric>
#include <random>
#include <util.h>

int main() {
  namespace fc = frozenca;
  using namespace std;

  constexpr float alpha = 0.3f;
  constexpr int trials = 1'000;
  constexpr int N = 1'000;

  vector<int> v(N);
  iota(v.begin(), v.end(), 0);
  mt19937 gen(random_device{}());

  int balanced_count = 0;
  for (int i = 0; i < trials; i++) {
    auto u = v;
    ranges::shuffle(u, gen);
    auto q = fc::partition_impl(u, 0, ssize(u));

    if (q < (1.0f - alpha) * N && q > (alpha)*N) {
      balanced_count++;
    }
  }

  cout << "Probability that the partition is balanced than (1-a)/a : "
       << balanced_count / static_cast<float>(trials) << '\n';
  cout << "1 - 2a : " << 1.0f - 2 * alpha << '\n';
}