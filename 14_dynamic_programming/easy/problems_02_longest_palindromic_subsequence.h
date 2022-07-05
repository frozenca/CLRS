#ifndef __CLRS4_LONGEST_PALINDROMIC_SUBSEQUENCE_H__
#define __CLRS4_LONGEST_PALINDROMIC_SUBSEQUENCE_H__

#include <algorithm>
#include <cassert>
#include <common.h>
#include <string>
#include <vector>


namespace frozenca {

using namespace std;

namespace {

enum class DirectionLPS {
  None,
  Up,
  Right,
  Upright,
};

} // anonymous namespace

string longest_palindromic_subsequence(const string &str) {
  auto n = ssize(str);
  vector<vector<index_t>> len(n, vector<index_t>(n));
  for (index_t i = 0; i < n; ++i) {
    len[i][i] = 1;
  }

  vector<vector<DirectionLPS>> B(n,
                                 vector<DirectionLPS>(n, DirectionLPS::None));

  for (index_t l = 2; l <= n; ++l) {
    for (index_t i = 0; i < n - l + 1; ++i) {
      index_t j = i + l - 1;
      if (str[i] == str[j]) {
        len[i][j] = len[i + 1][j - 1] + 2;
        B[i][j] = DirectionLPS::Upright;
      } else if (len[i][j - 1] > len[i + 1][j]) {
        len[i][j] = len[i][j - 1];
        B[i][j] = DirectionLPS::Up;
      } else {
        len[i][j] = len[i + 1][j];
        B[i][j] = DirectionLPS::Right;
      }
    }
  }
  index_t i = 0, j = n - 1;
  string result;
  bool centered = false;
  while (i < n && j < n && len[i][j]) {
    if (len[i][j] == 1) {
      assert(i == j);
      centered = true;
      break;
    }
    if (B[i][j] == DirectionLPS::Upright) {
      result += str[i];
      ++i;
      --j;
    } else if (B[i][j] == DirectionLPS::Up) {
      --j;
    } else {
      ++i;
    }
  }
  auto rev = result;
  ranges::reverse(rev);
  if (centered) {
    result += str[i];
  }
  result += rev;
  return result;
}

} // namespace frozenca

#endif //__CLRS4_LONGEST_PALINDROMIC_SUBSEQUENCE_H__
