#include <02_getting_started/easy/problems_04_inversion.h>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

int main() {
  namespace fc = frozenca;
  using namespace std;

  mt19937 gen(random_device{}());
  constexpr int N = 1'000;
  constexpr int trials = 1'000;

  vector<int> hats(N);
  iota(hats.begin(), hats.end(), 1);
  fc::index_t count = 0;
  for (int i = 0; i < trials; i++) {
    auto shuffled_hats = hats;
    ranges::shuffle(shuffled_hats, gen);
    count += fc::count_inversion(shuffled_hats);
  }
  cout << "Average inversion count : " << static_cast<float>(count) / trials
       << '\n';
  cout << "N * (N - 1) / 4 : " << N * (N - 1) / 4.0f << '\n';
}