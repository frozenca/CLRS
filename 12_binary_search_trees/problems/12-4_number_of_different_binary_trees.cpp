#include <cassert>
#include <cmath>
#include <common.h>
#include <iostream>
#include <numbers>
#include <vector>

namespace fc = frozenca;
using namespace std;

int64_t number_distinct_bst(int n, vector<int64_t> &b) {
  auto &ans = b[n];
  if (ans != -1) {
    return ans;
  }
  int64_t res = 0;
  for (int i = 0; i < n; ++i) {
    res += number_distinct_bst(i, b) * number_distinct_bst(n - 1 - i, b);
  }
  ans = res;
  return ans;
}

int64_t number_distinct_bst(int n) {
  assert(n >= 0);
  vector<int64_t> b(n + 1, -1);
  b[0] = 1;
  return number_distinct_bst(n, b);
}

int main() {
  constexpr int n = 20;
  cout << "n : " << n << '\n';
  cout << number_distinct_bst(n) << '\n';
  cout << pow(4, n) / (sqrt(numbers::pi_v<float>) * pow(n, 1.5f)) << '\n';
}