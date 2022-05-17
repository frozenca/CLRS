#ifndef __CLRS4_SORTING_VARIABLE_LENGTH_ITEMS_H__
#define __CLRS4_SORTING_VARIABLE_LENGTH_ITEMS_H__

#include <08_sorting_in_linear_time/easy/03_radix_sort.h>
#include <cassert>
#include <cmath>
#include <common.h>
#include <string>


namespace frozenca {

using namespace std;

void bucket_radix_sort(vector<uint16_t> &A) {
  const auto n = ssize(A);
  vector<vector<uint16_t>> B(n + 1);
  for (auto num : A) {
    B[static_cast<int>(floor(log2(num)))].push_back(num);
  }
  for (index_t i = 0; i <= n; ++i) {
    radix_sort(B[i], static_cast<int>(i));
  }
  A.clear();
  for (auto &b : B) {
    ranges::move(b, back_inserter(A));
  }
}

namespace {

constexpr int num_chars = 26;

void bucket_string_sort(vector<string> &A, index_t pos, index_t max_pos) {
  if (pos == max_pos) {
    return;
  }
  vector<vector<string>> B(num_chars + 1);
  for (const auto &str : A) {
    if (ssize(str) <= pos) {
      B[0].push_back(str);
    } else {
      B[1 + str[pos] - 'a'].push_back(str);
    }
  }
  A.clear();
  for (auto &b : B) {
    ranges::move(b, back_inserter(A));
  }
  bucket_string_sort(A, pos + 1, max_pos);
}

} // anonymous namespace

void bucket_string_sort(vector<string> &A) {
  index_t max_length = 0;
  for (const auto &str : A) {
    max_length = max(max_length, ssize(str));
  }
  bucket_string_sort(A, 0, max_length);
}

} // namespace frozenca

#endif //__CLRS4_SORTING_VARIABLE_LENGTH_ITEMS_H__
