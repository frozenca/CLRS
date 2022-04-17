#include <iostream>
#include <random>

using namespace std;

bool unbiased_random() {
  mt19937 gen(random_device{}());
  bernoulli_distribution biased_random(0.8);
  while (true) {
    bool a = biased_random(gen);
    bool b = biased_random(gen);
    if (a != b) {
      return a;
    }
  }
}

int main() {

  int count = 0;
  int trials = 100'000;
  for (int i = 0; i < trials; i++) {
    if (unbiased_random()) {
      count++;
    }
  }
  cout << static_cast<float>(count) / trials << '\n';
}