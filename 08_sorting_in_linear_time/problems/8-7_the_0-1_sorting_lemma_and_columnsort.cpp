#include <algorithm>
#include <cassert>
#include <iostream>
#include <numeric>
#include <random>
#include <ranges>
#include <vector>

using namespace std;

int main() {

  mt19937 gen(random_device{}());
  constexpr int R = 14;
  constexpr int S = 3;
  constexpr int N = R * S;
  static_assert(R % 2 == 0);
  vector<vector<int>> A(S, vector<int>(R));
  vector<int> B(N);
  iota(B.begin(), B.end(), 1);
  ranges::shuffle(B, gen);
  for (int s = 0; s < S; s++) {
    for (int r = 0; r < R; r++) {
      A[s][r] = B[s * R + r];
    }
  }
  for (const auto &row : A) {
    for (auto n : row) {
      cout << n << ' ';
    }
    cout << '\n';
  }
  cout << '\n';
  // step 1
  for (auto &row : A) {
    ranges::sort(row);
    for (auto n : row) {
      cout << n << ' ';
    }
    cout << '\n';
  }
  cout << '\n';

  // step 2
  vector<vector<int>> C(S, vector<int>(R));
  for (int i = 0; i < N; i++) {
    C[i % S][i / S] = A[i / R][i % R];
  }
  A = C;
  for (const auto &row : A) {
    for (auto n : row) {
      cout << n << ' ';
    }
    cout << '\n';
  }
  cout << '\n';

  // step 3
  for (auto &row : A) {
    ranges::sort(row);
    for (auto n : row) {
      cout << n << ' ';
    }
    cout << '\n';
  }
  cout << '\n';

  // step 4
  for (int i = 0; i < N; i++) {
    C[i / R][i % R] = A[i % S][i / S];
  }
  A = C;
  for (const auto &row : A) {
    for (auto n : row) {
      cout << n << ' ';
    }
    cout << '\n';
  }
  cout << '\n';

  // step 5
  for (auto &row : A) {
    ranges::sort(row);
    for (auto n : row) {
      cout << n << ' ';
    }
    cout << '\n';
  }
  cout << '\n';

  // step 6
  C.clear();
  C.resize(S + 1, vector<int>(R));
  for (int s = 0; s < S; s++) {
    for (int r = 0; r < R / 2; r++) {
      C[s][r + R / 2] = A[s][r];
    }
    for (int r = R / 2; r < R; r++) {
      C[s + 1][r - R / 2] = A[s][r];
    }
  }
  C.back().resize(R / 2);
  A = C;
  for (const auto &row : A) {
    for (auto n : row) {
      cout << n << ' ';
    }
    cout << '\n';
  }
  cout << '\n';

  // step 7
  for (auto &row : A) {
    ranges::sort(row);
    for (auto n : row) {
      cout << n << ' ';
    }
    cout << '\n';
  }
  cout << '\n';

  // step 8
  C.clear();
  C.resize(S, vector<int>(R));
  for (int s = 0; s < S; s++) {
    for (int r = R / 2; r < R; r++) {
      C[s][r - R / 2] = A[s][r];
    }
    for (int r = 0; r < R / 2; r++) {
      C[s][r + R / 2] = A[s + 1][r];
    }
  }
  A = C;
  for (const auto &row : A) {
    for (auto n : row) {
      cout << n << ' ';
    }
    cout << '\n';
  }
  cout << '\n';
}