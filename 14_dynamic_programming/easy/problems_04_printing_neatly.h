#ifndef __CLRS4_PRINTING_NEATLY_H__
#define __CLRS4_PRINTING_NEATLY_H__

#include <algorithm>
#include <cassert>
#include <cctype>
#include <cmath>
#include <common.h>
#include <iostream>
#include <limits>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>

namespace frozenca {

using namespace std;

index_t give_lines(const vector<index_t> &p, const vector<string_view> &tokens,
                   index_t j) {
  index_t i = p[j];
  index_t k = 0;
  if (i == 1) {
    k = 1;
  } else if (i == 0) {
    k = 0;
  } else {
    k = give_lines(p, tokens, i - 1) + 1;
  }
  for (index_t idx = i; idx <= j; idx++) {
    cout << tokens[idx] << ' ';
  }
  cout << '\n';
  return k;
}

void print_neatly(const string &paragraph, index_t M) {
  vector<string_view> tokens;
  const char *ptr = paragraph.data();
  const char *end_ptr = paragraph.data() + ssize(paragraph);
  while (ptr < end_ptr) {
    auto next =
        find_if(ptr, end_ptr, [](auto c) { return isspace(c) || c == '\0'; });
    string_view token(ptr, next);
    if (!token.empty()) {
      tokens.push_back(token);
    }
    ptr = next + 1;
  }

  const auto n = ssize(tokens);
  vector<vector<index_t>> extras(n, vector<index_t>(n));
  for (index_t i = 0; i < n; ++i) {
    assert(M >= ssize(tokens[i]));
    extras[i][i] = M - ssize(tokens[i]);
    for (index_t j = i + 1; j < n; ++j) {
      extras[i][j] = extras[i][j - 1] - ssize(tokens[j]) - 1;
    }
  }
  vector<vector<index_t>> cost(n, vector<index_t>(n));
  for (index_t i = 0; i < n; i++) {
    for (index_t j = i; j < n; j++) {
      if (extras[i][j] < 0) {
        cost[i][j] = numeric_limits<index_t>::max();
      } else if (j == n - 1 && extras[i][j] >= 0) {
        cost[i][j] = 0;
      } else {
        cost[i][j] = static_cast<index_t>(pow(extras[i][j], 3));
      }
    }
  }
  vector<index_t> c(n + 1);
  vector<index_t> p(n);
  for (index_t j = 0; j < n; j++) {
    c[j + 1] = numeric_limits<index_t>::max();
    for (index_t i = 0; i <= j; i++) {
      if (cost[i][j] != numeric_limits<index_t>::max() &&
          c[i] + cost[i][j] < c[j + 1]) {
        c[j + 1] = c[i] + cost[i][j];
        p[j] = i;
      }
    }
  }
  give_lines(p, tokens, n - 1);
}

} // namespace frozenca

#endif //__CLRS4_PRINTING_NEATLY_H__
