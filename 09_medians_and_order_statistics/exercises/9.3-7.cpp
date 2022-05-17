#include <algorithm>
#include <cassert>
#include <iostream>
#include <ranges>
#include <utility>
#include <vector>


using namespace std;

float find_optimal_place(vector<pair<float, float>> &A) {
  ranges::nth_element(
      A, A.begin() + A.size() / 2,
      [](const auto &p1, const auto &p2) { return p1.second < p2.second; });
  return A[A.size() / 2].second;
}

int main() {
  vector<pair<float, float>> A{
      {1.f, 3.f}, {2.f, 5.f}, {3.f, 1.f}, {4.f, 2.f}, {5.f, 4.f}};
  cout << find_optimal_place(A) << '\n';
}