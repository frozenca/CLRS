#include <algorithm>
#include <cassert>
#include <common.h>
#include <iostream>
#include <iterator>
#include <queue>
#include <ranges>
#include <vector>


namespace fc = frozenca;
using namespace std;

vector<int> largest_ith_sorted1(const vector<int> &A, fc::index_t i) {
  auto B = A;
  ranges::sort(B);
  vector<int> C(make_move_iterator(B.begin() + B.size() - i),
                make_move_iterator(B.end()));
  return C;
}

vector<int> largest_ith_sorted2(const vector<int> &A, fc::index_t i) {
  priority_queue<int> q(A.begin(), A.end());
  vector<int> C;
  for (fc::index_t j = 0; j < i; j++) {
    C.push_back(q.top());
    q.pop();
  }
  ranges::reverse(C);
  return C;
}

vector<int> largest_ith_sorted3(const vector<int> &A, fc::index_t i) {
  auto B = A;
  ranges::nth_element(B, B.begin() + B.size() - i);
  ranges::sort(B.begin() + (B.size() - i), B.end());
  vector<int> C(make_move_iterator(B.begin() + B.size() - i),
                make_move_iterator(B.end()));
  return C;
}

int main() {
  vector<int> v{3, 1, 6, 2, 5, 4, 7, 9, 8};
  fc::index_t i = 4;
  auto a1 = largest_ith_sorted1(v, i);
  for (auto n : a1) {
    cout << n << ' ';
  }
  cout << '\n';
  auto a2 = largest_ith_sorted2(v, i);
  for (auto n : a2) {
    cout << n << ' ';
  }
  cout << '\n';
  auto a3 = largest_ith_sorted3(v, i);
  for (auto n : a3) {
    cout << n << ' ';
  }
  cout << '\n';
}