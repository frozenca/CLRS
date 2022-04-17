#include <algorithm>
#include <cmath>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>


int main() {
  using namespace std;

  mt19937 gen(random_device{}());
  constexpr int N = 1'000;
  constexpr int trials = 1'000;

  vector<int> hats(N);
  iota(hats.begin(), hats.end(), 1);
  int count = 0;
  for (int i = 0; i < trials; i++) {
    auto shuffled_hats = hats;
    ranges::shuffle(shuffled_hats, gen);
    for (int j = 0; j < N; j++) {
      if (shuffled_hats[j] == hats[j]) {
        count++;
      }
    }
  }
  cout << "Average matched count : " << static_cast<float>(count) / trials
       << '\n';
}