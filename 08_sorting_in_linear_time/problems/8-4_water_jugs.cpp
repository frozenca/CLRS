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

void match_jugs(vector<int> &R, vector<int> &B, vector<pair<int, int>> &jugs) {
  mt19937 gen(random_device{}());
  assert(ssize(R) == ssize(B));
  if (R.empty()) {
    return;
  } else if (ssize(R) == 1) {
    jugs.emplace_back(R[0], B[0]);
  } else {
    uniform_int_distribution<fc::index_t> dist(0, ssize(R) - 1);
    int r = R[dist(gen)];
    vector<int> B_s, B_l, R_s, R_l;
    for (auto &b : B) {
      if (b == r) {
        jugs.emplace_back(r, b);
      } else if (b > r) {
        B_l.push_back(b);
      } else {
        B_s.push_back(b);
      }
    }
    auto b = jugs.back().second;
    for (auto t : R) {
      if (t == b) {
        continue;
      } else if (t > b) {
        R_l.push_back(t);
      } else {
        R_s.push_back(t);
      }
    }
    R.clear();
    B.clear();
    match_jugs(R_l, B_l, jugs);
    match_jugs(R_s, B_s, jugs);
  }
}

int main() {
  constexpr int N = 1'000;

  mt19937 gen(random_device{}());

  vector<int> R(N), B(N);
  iota(R.begin(), R.end(), 0);
  iota(B.begin(), B.end(), 0);
  ranges::shuffle(R, gen);
  ranges::shuffle(B, gen);
  vector<pair<int, int>> jugs;
  match_jugs(R, B, jugs);
  if (ranges::all_of(jugs, [](const auto &p) { return p.first == p.second; })) {
    cout << "Jugs matched\n";
  } else {
    cout << "Jugs matching failed\n";
  }
}