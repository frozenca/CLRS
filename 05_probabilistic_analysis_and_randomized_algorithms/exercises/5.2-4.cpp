#include <cmath>
#include <iostream>
#include <random>

int main() {
  using namespace std;

  mt19937 gen(random_device{}());
  constexpr int trials = 1'000;
  uniform_int_distribution<> dice(1, 6);
  {
    int sum = 0;

    for (int i = 0; i < trials; i++) {
      sum += dice(gen);
      sum += dice(gen);
    }

    cout << "Two 6-sided dices, rolled independently "
         << static_cast<float>(sum) / trials << '\n';
  }
  {
    int sum = 0;

    for (int i = 0; i < trials; i++) {
      auto a = dice(gen);
      sum += a;
      sum += a;
    }

    cout << "Two 6-sided dices, the second die is set equal to the first die "
         << static_cast<float>(sum) / trials << '\n';
  }
    {
    int sum = 0;

    for (int i = 0; i < trials; i++) {
      auto a = dice(gen);
      sum += a;
      sum += (7 - a);
    }

    cout << "Two 6-sided dices, the second die is set equal to 7 minus the value of the first die "
         << static_cast<float>(sum) / trials << '\n';
  }
}