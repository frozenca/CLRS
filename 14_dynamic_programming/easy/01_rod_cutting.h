#ifndef __CLRS4_ROD_CUTTING_H__
#define __CLRS4_ROD_CUTTING_H__

#include <algorithm>
#include <cassert>
#include <common.h>
#include <iostream>
#include <limits>
#include <ranges>
#include <vector>


namespace frozenca {

using namespace std;

index_t cut_rod_naive_impl(const vector<index_t> &p, index_t n) {
  if (n == 0) {
    return 0;
  }
  assert(n <= ssize(p));
  auto q = numeric_limits<index_t>::lowest();
  for (index_t i = 1; i <= n; ++i) {
    q = max(q, p[i - 1] + cut_rod_naive_impl(p, n - i));
  }
  return q;
}

index_t cut_rod_naive(const vector<index_t> &p) {
  auto n = ssize(p);
  return cut_rod_naive_impl(p, n);
}

index_t cut_rod_memoized_aux(const vector<index_t> &p, index_t n,
                             vector<index_t> &r) {
  assert(n < ssize(r));
  if (r[n] >= 0) { // already have a solution for length n?
    return r[n];
  }
  // remember the solution value for length n
  index_t &q = r[n];
  if (n == 0) {
    q = 0;
  } else {
    q = numeric_limits<index_t>::lowest();
    for (index_t i = 1; i <= n; ++i) { // i is the position of the first cut
      q = max(q, p[i - 1] + cut_rod_memoized_aux(p, n - i, r));
    }
  }
  return q;
}

index_t cut_rod_memoized(const vector<index_t> &p) {
  auto n = ssize(p);
  // will remember solution values in r
  vector<index_t> r(n + 1, numeric_limits<index_t>::lowest());
  return cut_rod_memoized_aux(p, n, r);
}

index_t cut_rod_bottom_up(const vector<index_t> &p) {
  auto n = ssize(p);
  // will remember solution values in r
  vector<index_t> r(n + 1);
  for (index_t j = 1; j <= n; ++j) { // for increasing rod length j
    index_t &q = r[j];               // remember the solution value for length j
    q = numeric_limits<index_t>::lowest();
    for (index_t i = 1; i <= j; ++i) { // i is the position of the first cut
      q = max(q, p[i - 1] + r[j - i]);
    }
  }
  return r[n];
}

pair<vector<index_t>, vector<index_t>>
cut_rod_bottom_up_extended(const vector<index_t> &p) {
  auto n = ssize(p);
  vector<index_t> r(n + 1);
  vector<index_t> s(n);
  for (index_t j = 1; j <= n; ++j) { // for increasing rod length j
    index_t &q = r[j];               // remember the solution value for length j
    q = numeric_limits<index_t>::lowest();
    for (index_t i = 1; i <= j; ++i) { // i is the position of the first cut
      if (q < p[i - 1] + r[j - i]) {
        q = p[i - 1] + r[j - i];
        s[j - 1] = i; // best cut location so far for length j;
      }
    }
  }
  return {r, s};
}

void cut_rod_solution_print(const vector<index_t> &p) {
  auto [r, s] = cut_rod_bottom_up_extended(p);
  auto n = ssize(p);
  while (n > 0) {
    cout << s[n - 1] << '\n'; // cut location for length n
    n -= s[n - 1];            // length of the remainder of the rod
  }
}

// wrong
index_t cut_rod_greedy(const vector<index_t> &p) {
  auto n = ssize(p);
  vector<float> density(n);
  for (index_t i = 0; i < n; ++i) {
    density[i] = p[i] / static_cast<float>(i + 1);
  }
  index_t q = 0;
  while (n > 0) {
    auto max_index = distance(
        density.begin(), max_element(density.begin(), density.begin() + n));
    q += p[max_index];
    n -= (max_index + 1);
  }
  return q;
}

index_t cut_rod_cost(const vector<index_t> &p, index_t c) {
  auto n = ssize(p);
  assert(c >= 0);
  // will remember solution values in r
  vector<index_t> r(n + 1);
  for (index_t j = 1; j <= n; ++j) { // for increasing rod length j
    index_t &q = r[j];               // remember the solution value for length j
    q = numeric_limits<index_t>::lowest();
    for (index_t i = 1; i <= j; ++i) { // i is the position of the first cut
      q = max(q, p[i - 1] + r[j - i] - c);
    }
  }
  return r[n];
}


index_t cut_rod_memoized_half_aux(const vector<index_t> &p, index_t n,
                             vector<index_t> &r) {
  assert(n < ssize(r));
  if (r[n] >= 0) { // already have a solution for length n?
    return r[n];
  }
  // remember the solution value for length n
  index_t &q = r[n];
  if (n == 0) {
    q = 0;
  } else {
    q = numeric_limits<index_t>::lowest();
    for (index_t i = 1; i <= n / 2; ++i) { // i is the position of the first cut
      q = max(q, p[i - 1] + cut_rod_memoized_half_aux(p, n - i, r));
      q = max(q, p[n - i] + cut_rod_memoized_half_aux(p, i - 1, r));
    }
  }
  return q;
}

index_t cut_rod_memoized_half(const vector<index_t> &p) {
  auto n = ssize(p);
  // will remember solution values in r
  vector<index_t> r(n + 1, numeric_limits<index_t>::lowest());
  return cut_rod_memoized_half_aux(p, n, r);
}


index_t cut_rod_memoized_print_aux(const vector<index_t> &p, index_t n,
                             vector<index_t> &r, vector<index_t>& s) {
  assert(n < ssize(r));
  if (r[n] >= 0) { // already have a solution for length n?
    return r[n];
  }
  // remember the solution value for length n
  index_t &q = r[n];
  if (n == 0) {
    q = 0;
  } else {
    q = numeric_limits<index_t>::lowest();
    for (index_t i = 1; i <= n; ++i) { // i is the position of the first cut
      auto val = p[i - 1] + cut_rod_memoized_print_aux(p, n - i, r, s);
      if (val > q) {
        q = val;
        s[n - 1] = i;
      }
    }
  }
  return q;
}

void cut_rod_memoized_print(const vector<index_t> &p) {
  auto n = ssize(p);
  // will remember solution values in r
  vector<index_t> r(n + 1, numeric_limits<index_t>::lowest());
  vector<index_t> s(n);
  cut_rod_memoized_print_aux(p, n, r, s);
  while (n > 0) {
    cout << s[n - 1] << '\n'; // cut location for length n
    n -= s[n - 1];            // length of the remainder of the rod
  }
}

} // namespace frozenca

#endif //__CLRS4_ROD_CUTTING_H__
