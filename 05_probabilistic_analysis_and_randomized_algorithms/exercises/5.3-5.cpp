#include <cassert>
#include <iostream>
#include <random>
#include <unordered_set>
#include <utility>

using namespace std;

unordered_set<int> random_sample(int m, int n) {
  assert(m <= n);
  if (m == 0) {
    return {};
  }
  mt19937 gen(random_device{}());
  auto res = random_sample(m - 1, n - 1);
  uniform_int_distribution<> dist(1, n);
  auto i = dist(gen);
  if (res.contains(i)) {
    res.insert(n);
  } else {
    res.insert(i);
  }
  return res;
}

int main() {
  constexpr int N = 20;
  auto v = random_sample(8, N);
  for (auto n : v) {
    cout << n << ' ';
  }
}