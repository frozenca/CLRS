#include <algorithm>
#include <cassert>
#include <common.h>
#include <iostream>
#include <vector>


namespace fc = frozenca;
using namespace std;

int select_two_sorted_array(const vector<int> &A, fc::index_t pa,
                            fc::index_t ra, const vector<int> &B,
                            fc::index_t pb, fc::index_t rb, fc::index_t k) {
  assert(pa >= 0 && ra <= ssize(A) && pb >= 0 && rb <= ssize(B));
  if (k == 0) {
    return min(A[pa], B[pb]);
  }
  if (ra == pa) {
    return B[pb + k];
  }
  if (ra - pa > rb - pb) { // ensure len(A) <= len(B)
    return select_two_sorted_array(B, pb, rb, A, pa, ra, k);
  }
  auto i = min(ra - pa, (k + 1) / 2);
  auto j = min(rb - pb, (k + 1) / 2);
  if (A[pa + i - 1] > B[pb + j - 1]) {
    // skip first j elements of B and search k - j th element
    return select_two_sorted_array(A, pa, ra, B, pb + j, rb, k - j);
  } else {
    // skip first i elements of A and search k - i th element
    return select_two_sorted_array(A, pa + i, ra, B, pb, rb, k - i);
  }
}

int select_two_sorted_array(const vector<int> &A, const vector<int> &B,
                            fc::index_t k) {
  assert(ranges::is_sorted(A));
  assert(ranges::is_sorted(B));
  const auto m = ssize(A);
  const auto n = ssize(B);
  assert(k >= 0 && k < m + n);
  return select_two_sorted_array(A, 0, m, B, 0, n, k);
}

int main() {
  vector<int> A{2, 3, 6, 7, 9};
  vector<int> B{1, 4, 5, 8, 10, 11};
  for (fc::index_t i = 0; i < ssize(A) + ssize(B); ++i) {
    cout << select_two_sorted_array(A, B, i) << '\n';
  }
}