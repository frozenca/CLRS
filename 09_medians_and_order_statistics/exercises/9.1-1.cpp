#include <algorithm>
#include <cassert>
#include <common.h>
#include <iostream>
#include <numeric>
#include <random>
#include <ranges>
#include <vector>

namespace fc = frozenca;
using namespace std;

int second_smallest(const vector<int> &A) {
  assert(ssize(A) >= 2);
  vector<int> B = A;
  vector<pair<int, int>> total_results;
  vector<int> C;
  while (ssize(B) > 1) {
    C.clear();
    for (fc::index_t i = 0; i < ssize(B); i += 2) {
      if (i == ssize(B) - 1) {
        C.push_back(B[i]);
        total_results.emplace_back(B[i], B[i]);
      } else if (B[i] < B[i + 1]) {
        C.push_back(B[i]);
        total_results.emplace_back(B[i], B[i + 1]);
      } else { // B[i] >= B[i + 1]
        C.push_back(B[i + 1]);
        total_results.emplace_back(B[i + 1], B[i]);
      }
    }
    B = C;
  }
  int min_element = C[0];
  C.clear();
  for (const auto &[small, big] : total_results) {
    if (small == min_element) {
      C.push_back(big);
    }
  }
  return *ranges::min_element(C);
}

int main() {
  vector<int> v{1, 5, 3, 2, 6};
  cout << second_smallest(v);
}