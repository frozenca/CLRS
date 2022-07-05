#ifndef __CLRS4_EDIT_DISTANCE_H__
#define __CLRS4_EDIT_DISTANCE_H__

#include <cassert>
#include <common.h>
#include <limits>
#include <string>
#include <tuple>
#include <vector>

namespace frozenca {

using namespace std;

enum class Op {
  None,
  Copy,
  Replace,
  Delete,
  Insert,
  Twiddle,
  Kill,
};

string_view op2str(const Op &curr) {
  using namespace string_view_literals;
  if (curr == Op::Kill) {
    return "kill"sv;
  } else if (curr == Op::Insert) {
    return "insert"sv;
  } else if (curr == Op::Delete) {
    return "delete"sv;
  } else if (curr == Op::Twiddle) {
    return "twiddle"sv;
  } else if (curr == Op::Replace) {
    return "replace"sv;
  } else if (curr == Op::Copy) {
    return "copy"sv;
  } else {
    assert(false);
    return "none"sv;
  }
}

void edit_distance(const string &x, const string &y) {
  auto m = ssize(x);
  auto n = ssize(y);

  constexpr float cost_copy = 1.0f;
  constexpr float cost_replace = 1.0f;
  constexpr float cost_delete = 1.0f;
  constexpr float cost_insert = 1.0f;
  constexpr float cost_twiddle = 2.0f;
  constexpr float cost_kill = 20.0f;

  vector<vector<float>> c(m + 1, vector<float>(n + 1));
  vector<vector<Op>> ops(m + 1, vector<Op>(n + 1, Op::None));

  for (index_t i = 0; i <= m; i++) {
    c[i][0] = i * cost_delete;
    ops[i][0] = Op::Delete;
  }
  for (index_t j = 0; j <= n; j++) {
    c[0][j] = j * cost_insert;
    ops[0][j] = Op::Insert;
  }
  for (index_t i = 1; i <= m; i++) {
    for (index_t j = 1; j <= n; j++) {
      c[i][j] = numeric_limits<float>::max();
      if (x[i - 1] == y[j - 1]) {
        c[i][j] = c[i - 1][j - 1] + cost_copy;
        ops[i][j] = Op::Copy;
      }
      if (x[i - 1] != y[j - 1] && c[i - 1][j - 1] + cost_replace < c[i][j]) {
        c[i][j] = c[i - 1][j - 1] + cost_replace;
        ops[i][j] = Op::Replace;
      }
      if (i >= 2 && j >= 2 && x[i - 1] == y[j - 2] && x[i - 2] == y[j - 1] &&
          c[i - 2][j - 2] + cost_twiddle < c[i][j]) {
        c[i][j] = c[i - 2][j - 2] + cost_twiddle;
        ops[i][j] = Op::Twiddle;
      }
      if (c[i - 1][j] + cost_delete < c[i][j]) {
        c[i][j] = c[i - 1][j] + cost_delete;
        ops[i][j] = Op::Delete;
      }
      if (c[i][j - 1] + cost_insert < c[i][j]) {
        c[i][j] = c[i][j - 1] + cost_insert;
        ops[i][j] = Op::Insert;
      }
    }
  }
  for (index_t i = 0; i < m - 1; i++) {
    if (c[i][n] + cost_kill < c[m][n]) {
      c[m][n] = c[i][n] + cost_kill;
      ops[m][n] = Op::Kill;
    }
  }
  cout << "Edit distance: " << c[m][n] << '\n';
  vector<tuple<Op, index_t, index_t>> op_sequence;
  {
    index_t i = m;
    index_t j = n;

    while (i > 0 || j > 0) {
      auto curr = ops[i][j];
      op_sequence.emplace_back(curr, i, j);
      if (curr == Op::Kill) {
        assert(j == n);
        for (index_t k = m - 1; k >= 0; --k) {
          if (c[k][n] == c[m][n] - cost_kill) {
            i = k;
            break;
          }
        }
      } else if (curr == Op::Insert) {
        --j;
      } else if (curr == Op::Delete) {
        --i;
      } else if (curr == Op::Twiddle) {
        i -= 2;
        j -= 2;
      } else if (curr == Op::Replace) {
        --i;
        --j;
      } else if (curr == Op::Copy) {
        --i;
        --j;
      } else {
        assert(false);
      }
    }
  }
  ranges::reverse(op_sequence);

  cout << "Operation sequence:\n";
  for (const auto &[op, i, j] : op_sequence) {
    cout << op2str(op) << ' ' << i << ' ' << j << '\n';
  }
}

void max_alignment(const string &x, const string &y) {
  auto m = ssize(x);
  auto n = ssize(y);

  constexpr float score_copy = +1.0f;
  constexpr float score_replace = -1.0f;
  constexpr float score_delete = -2.0f;
  constexpr float score_insert = -2.0f;

  vector<vector<float>> c(m + 1, vector<float>(n + 1));
  vector<vector<Op>> ops(m + 1, vector<Op>(n + 1, Op::None));

  for (index_t i = 0; i <= m; i++) {
    c[i][0] = i * score_delete;
    ops[i][0] = Op::Delete;
  }
  for (index_t j = 0; j <= n; j++) {
    c[0][j] = j * score_insert;
    ops[0][j] = Op::Insert;
  }
  for (index_t i = 1; i <= m; i++) {
    for (index_t j = 1; j <= n; j++) {
      c[i][j] = numeric_limits<float>::lowest();
      if (x[i - 1] == y[j - 1]) {
        c[i][j] = c[i - 1][j - 1] + score_copy;
        ops[i][j] = Op::Copy;
      }
      if (x[i - 1] != y[j - 1] && c[i - 1][j - 1] + score_replace > c[i][j]) {
        c[i][j] = c[i - 1][j - 1] + score_replace;
        ops[i][j] = Op::Replace;
      }
      if (c[i - 1][j] + score_delete > c[i][j]) {
        c[i][j] = c[i - 1][j] + score_delete;
        ops[i][j] = Op::Delete;
      }
      if (c[i][j - 1] + score_insert > c[i][j]) {
        c[i][j] = c[i][j - 1] + score_insert;
        ops[i][j] = Op::Insert;
      }
    }
  }
  cout << "Matching score: " << c[m][n] << '\n';
  pair<string, string> match = {"", ""};
  {
    index_t i = m;
    index_t j = n;

    while (i > 0 || j > 0) {
      auto curr = ops[i][j];
      if (curr == Op::Insert) {
        --j;
        match.first += ' ';
        match.second += y[j];
      } else if (curr == Op::Delete) {
        --i;
        match.first += x[i];
        match.second += ' ';
      } else if (curr == Op::Replace) {
        --i;
        --j;
        match.first += x[i];
        match.second += y[j];
      } else if (curr == Op::Copy) {
        --i;
        --j;
        match.first += x[i];
        match.second += y[j];
      } else {
        assert(false);
      }
    }
  }
  ranges::reverse(match.first);
  ranges::reverse(match.second);

  cout << "Optimal match:\n";
  cout << match.first << '\n';
  cout << match.second << '\n';
}

} // namespace frozenca

#endif //__CLRS4_EDIT_DISTANCE_H__
