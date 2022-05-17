#include <algorithm>
#include <cassert>
#include <cmath>
#include <common.h>
#include <iostream>
#include <ranges>
#include <utility>
#include <vector>

namespace fc = frozenca;
using namespace std;

vector<int> k_closest_to_median(vector<int> &A, fc::index_t k) {
  const auto n = ssize(A);
  assert(k <= n);
  ranges::nth_element(A, A.begin() + n / 2);
  auto median = A[n / 2];
  ranges::nth_element(A, A.begin() + k, [median](auto a, auto b) {
    return abs(a - median) < abs(b - median);
  });
  return vector<int>(A.begin(), A.begin() + k);
}

int main() {
  vector<int> v{3, 2, 6, 1, 5, 4, 8, 7};
  auto w = k_closest_to_median(v, 3);
  for (auto n : w) {
    cout << n << ' ';
  }
}