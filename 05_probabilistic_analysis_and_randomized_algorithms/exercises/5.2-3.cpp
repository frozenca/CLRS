#include <cmath>
#include <iostream>
#include <random>

int main() {
  using namespace std;

  mt19937 gen(random_device{}());
  constexpr int trials = 1'000;
  int sum = 0;
  uniform_int_distribution<> dice(1, 6);

  for (int i = 0; i < trials; i++) {
    sum += dice(gen);
  }

  cout << "sum / trials of a dice : " << static_cast<float>(sum) / trials
       << '\n';
}