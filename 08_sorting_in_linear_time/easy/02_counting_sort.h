#ifndef __CLRS4_COUNTING_SORT_H__
#define __CLRS4_COUNTING_SORT_H__

#include <algorithm>
#include <cassert>
#include <cmath>
#include <common.h>
#include <vector>

namespace frozenca {

using namespace std;

void counting_sort(vector<int> &A, int k) {
  assert(*ranges::min_element(A) >= 0 && *ranges::max_element(A) <= k);
  const auto n = ssize(A);
  vector<int> B(n);
  vector<int> C(k + 1);
  for (auto num : A) {
    C[num]++;
  }
  // C[i] now contains the number of elements equal to i.
  for (int j = 1; j <= k; ++j) {
    C[j] += C[j - 1];
  }
  // C[i] now contains the number of elements less than or equal to i.
  // Copy A to B, starting from the end of A.
  for (index_t j = n - 1; j >= 0; --j) {
    B[C[A[j]] - 1] = A[j];
    C[A[j]]--; // to handle duplicate values
  }
  swap(A, B);
}

void counting_sort2(vector<int> &A, int k) {
  assert(*ranges::min_element(A) >= 0 && *ranges::max_element(A) <= k);
  const auto n = ssize(A);
  vector<int> B(n);
  vector<int> C(k + 2);
  for (auto num : A) {
    C[num + 1]++;
  }
  // C[i + 1] now contains the number of elements equal to i.
  for (int j = 2; j <= k + 1; ++j) {
    C[j] += C[j - 1];
  }
  // C[i + 1] now contains the number of elements less than or equal to i.
  // Copy A to B, starting from the beginning of A.
  for (int i = 0; i < n; ++i) {
    auto num = A[i];
    B[C[num]] = num;
    C[num]++;
  }
  swap(A, B);
}

void counting_sort_inplace(vector<int> &A, int k) {
  assert(*ranges::min_element(A) >= 0 && *ranges::max_element(A) <= k);
  vector<int> C(k + 1);
  for (auto num : A) {
    C[num]++;
  }
  // C[i + 1] now contains the number of elements equal to i.
  for (int j = 1; j <= k; ++j) {
    C[j] += C[j - 1];
  }
  // C[i + 1] now contains the number of elements less than or equal to i.
  int prev_count = 0;
  for (int i = 0; i < k;) {
    auto count = C[i];
    for (int j = prev_count; j <= count; ++j) {
      A[j] = i;
    }
    while (C[i] == count) {
      i++;
    }
    prev_count = count;
  }
}

vector<int> counting_range_query(const vector<int> &A, int k,
                                 const vector<pair<int, int>> &queries) {
  assert(*ranges::min_element(A) >= 0 && *ranges::max_element(A) <= k);
  vector<int> C(k + 2);
  for (auto num : A) {
    C[num + 1]++;
  }
  // C[i] now contains the number of elements equal to i.
  for (int j = 2; j <= k + 1; ++j) {
    C[j] += C[j - 1];
  }
  // C[i] now contains the number of elements less than or equal to i.
  vector<int> query_answers;
  for (const auto &[a, b] : queries) {
    query_answers.push_back(C[b + 1] - C[a]);
  }
  return query_answers;
}

void counting_sort_fraction(vector<float> &A, int k, int d) {
  assert(d >= 0 && *ranges::min_element(A) >= 0 && *ranges::max_element(A) <= k);
  const auto n = ssize(A);
  vector<float> B(n);
  const int pow_ten = static_cast<int>(pow(10, d));
  vector<int> C(k * pow_ten + 1);
  for (auto num : A) {
    C[static_cast<int>(num * pow_ten)]++;
  }
  // C[i] now contains the number of elements equal to i.
  for (int j = 1; j <= k * pow_ten; ++j) {
    C[j] += C[j - 1];
  }
  // C[i] now contains the number of elements less than or equal to i.
  // Copy A to B, starting from the end of A.
  for (index_t j = n - 1; j >= 0; --j) {
    B[C[static_cast<int>(A[j] * pow_ten)] - 1] = A[j];
    C[static_cast<int>(A[j] * pow_ten)]--; // to handle duplicate values
  }
  swap(A, B);
}

} // namespace frozenca

#endif //__CLRS4_COUNTING_SORT_H__
