#include <09_medians_and_order_statistics/inc/easy.h>
#include <algorithm>
#include <cassert>
#include <common.h>
#include <iostream>
#include <iterator>
#include <numeric>
#include <random>
#include <ranges>
#include <test.h>
#include <util.h>
#include <utility>
#include <vector>

namespace fc = frozenca;
using namespace std;

template <typename T>
void apply_permutation(vector<T> &arr, fc::index_t p, fc::index_t r,
                       vector<fc::index_t> &perm) {
  const auto n = r - p;
  assert(ssize(perm) == n);
  for (fc::index_t i = 0; i + 1 < n; ++i) {
    auto ind = perm[i];
    while (ind < i) {
      ind = perm[ind];
    }
    swap(arr[p + i], arr[p + ind]);
  }
}

int select_small_order(vector<int> &A, fc::index_t p, fc::index_t r,
                       fc::index_t i, vector<fc::index_t> &perm) {
  const auto n = r - p;
  const auto m = n / 2;
  assert(ssize(perm) == n);
  if (i >= m) {
    // directly select i-th.
    return fc::select_with_perm(A, p, r, i, perm, p);
  } else {
    // divide into two parts. [p : p + m), [p + m : p + 2m)
    // compare A[p + j] and A[p + j + m] for j = [0, m),
    // put the smaller of the two elements into the right,
    // the larger into the left.
    for (fc::index_t j = 0; j < m; ++j) {
      if (A[p + j] < A[p + j + m]) {
        swap(A[p + j], A[p + j + m]);
        swap(perm[j], perm[j + m]); // we should remember "all" swaps.
      }
    }

    // recursively find the ith smallest element in A[p + m : p + 2m)
    vector<fc::index_t> half_perm(m);
    iota(half_perm.begin(), half_perm.end(), 0);
    select_small_order(A, p + m, p + 2 * m, i, half_perm);

    // apply local permutation from the right side recursive call
    apply_permutation(A, p, p + m, half_perm);
    apply_permutation(perm, 0, m, half_perm);
    apply_permutation(perm, m, 2 * m, half_perm);

    // gather A[p : p + i], A[p + m : p + m + i], A[p + n] (if there is)
    // to A[p : p + 2i + 1] (or A[p : p + 2i + 2])
    for (fc::index_t j = 0; j < m - i - 1; ++j) {
      swap(A[p + i + 1 + j], A[p + m + i - j]);
      swap(perm[i + 1 + j], perm[m + i - j]);
    }
    if (2 * m < n) {
      swap(A[p + 2 * m], A[p + 2 * i + 2]);
      swap(perm[2 * m], perm[2 * i + 2]);
      // directly select i-th.
      // i < m, therefore 2 * i + 3 <= n
      return fc::select_with_perm(A, p, p + 2 * i + 3, i, perm, p);
    } else {
      return fc::select_with_perm(A, p, p + 2 * i + 2, i, perm, p);
    }
  }
}

int select_small_order(vector<int> &A, fc::index_t i) {
  const auto n = ssize(A);
  vector<fc::index_t> perm(n);
  iota(perm.begin(), perm.end(), 0);
  return select_small_order(A, 0, ssize(A), i, perm);
}

int main() {
  mt19937 gen(random_device{}());

  constexpr int num_trials = 10'000;
  constexpr int max_n = 10'000;
  uniform_int_distribution<> dist(1, max_n);

  int fail_count = 0;
  for (int i = 0; i < num_trials; ++i) {
    int n = dist(gen);
    vector<int> v(n);
    iota(v.begin(), v.end(), 0);
    ranges::shuffle(v, gen);

    uniform_int_distribution<> k_dist(0, n - 1);
    int k = k_dist(gen);
    auto kth = select_small_order(v, k);
    if (kth != k) {
      cout << "Select algorithm failed! " << kth << " " << k << '\n';
      fail_count++;
    }
  }
  if (fail_count == 0) {
    cout << "Select algorithm succeeded\n";
  }
}