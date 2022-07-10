#ifndef __CLRS4_ACTIVITY_SELECTION_H__
#define __CLRS4_ACTIVITY_SELECTION_H__

#include <algorithm>
#include <cassert>
#include <common.h>
#include <interval.h>
#include <limits>
#include <queue>
#include <ranges>
#include <set>
#include <tuple>
#include <unordered_map>
#include <vector>

namespace frozenca {

using namespace std;

vector<Interval>
recursive_activity_selector_impl(const vector<Interval> &intervals, index_t k) {
  index_t m = k + 1;
  while (m < ssize(intervals) && intervals[m].first_ < intervals[k].second_) {
    ++m;
  }
  if (m < ssize(intervals)) {
    auto A = recursive_activity_selector_impl(intervals, m);
    A.push_back(intervals[m]);
    return A;
  } else {
    return {};
  }
}

vector<Interval> recursive_activity_selector(vector<Interval> &intervals) {
  ranges::sort(intervals, [](const auto &a, const auto &b) {
    return a.second_ < b.second_;
  });
  auto A = recursive_activity_selector_impl(intervals, 0);
  A.push_back(intervals[0]);
  ranges::reverse(A);
  return A;
}

vector<Interval> greedy_activity_selector(vector<Interval> &intervals) {
  ranges::sort(intervals, [](const auto &a, const auto &b) {
    return a.second_ < b.second_;
  });
  const auto n = ssize(intervals);
  assert(n > 0);
  vector<Interval> A;
  A.push_back(intervals[0]);
  index_t k = 0;
  for (index_t m = 1; m < n; m++) {
    if (intervals[m].first_ >= intervals[k].second_) {
      A.push_back(intervals[m]);
      k = m;
    }
  }
  return A;
}

vector<Interval> dp_activity_selector(vector<Interval> &intervals) {
  ranges::sort(intervals, [](const auto &a, const auto &b) {
    return a.second_ < b.second_;
  });
  const auto n = ssize(intervals);
  vector<vector<index_t>> c(n + 1, vector<index_t>(n + 1));
  vector<vector<index_t>> a(n + 1, vector<index_t>(n + 1, -1));
  for (index_t i = 0; i < n; ++i) {
    for (index_t j = i + 1; j <= n; ++j) {
      c[i][j] = 1;
    }
  }
  for (index_t l = 2; l <= n; l++) {
    for (index_t i = 0; i <= n - l; i++) {
      index_t j = i + l;
      index_t k = j - 1;
      while (k > i && intervals[i].second_ < intervals[k].second_) {
        if (intervals[i].second_ <= intervals[k].first_ &&
            (j == n || intervals[k].second_ <= intervals[j].first_) &&
            c[i][k] + c[k][j] + 1 > c[i][j]) {
          c[i][j] = c[i][k] + c[k][j] + 1;
          a[i][j] = k;
        }
        k--;
      }
    }
  }
  vector<Interval> result;
  queue<pair<index_t, index_t>> q;
  q.emplace(0, n);
  while (!q.empty()) {
    auto [i, j] = q.front();
    q.pop();
    auto k = a[i][j];
    if (k == -1 || i == j) {
      continue;
    }
    result.push_back(intervals[k]);
    q.emplace(i, k);
    q.emplace(k, j);
  }
  result.push_back(intervals[0]);
  ranges::reverse(result);
  return result;
}

vector<Interval> greedy_activity_selector_rev(vector<Interval> &intervals) {
  ranges::sort(intervals, [](const auto &a, const auto &b) {
    return a.second_ < b.second_;
  });
  const auto n = ssize(intervals);
  assert(n > 0);
  vector<Interval> A;
  A.push_back(intervals[n - 1]);
  index_t k = n - 1;
  for (index_t m = n - 2; m >= 0; m--) {
    if (intervals[m].second_ <= intervals[k].first_) {
      A.push_back(intervals[m]);
      k = m;
    }
  }
  ranges::reverse(A);
  return A;
}

bool is_overlapping(const Interval &iv1, const Interval &iv2) {
  return (iv2.first_ <= iv1.second_) || (iv1.first_ <= iv2.second_);
}

bool is_overlapping(const vector<Interval> &intervals, const Interval &iv) {
  if (intervals.empty()) {
    return false;
  }
  const auto n = ssize(intervals);
  for (index_t i = 0; i < n; ++i) {
    if (is_overlapping(intervals[i], iv)) {
      return true;
    }
  }
  return false;
}

vector<Interval> greedy_activity_selector_compatible_least_duration(
    vector<Interval> &intervals) {
  ranges::sort(intervals, [](const auto &a, const auto &b) {
    return a.second_ < b.second_;
  });
  const auto n = ssize(intervals);
  assert(n > 0);
  vector<Interval> A;
  while (true) {
    index_t idx = -1;
    float least_duration = numeric_limits<float>::max();
    for (index_t i = 0; i < n; ++i) {
      if (!is_overlapping(A, intervals[i]) &&
          intervals[i].length() < least_duration) {
        least_duration = intervals[i].length();
        idx = i;
      }
    }
    if (idx == -1) {
      break;
    } else {
      A.push_back(intervals[idx]);
    }
  }
  return A;
}

vector<Interval>
greedy_activity_selector_overlaps_fewest(vector<Interval> &intervals) {
  ranges::sort(intervals, [](const auto &a, const auto &b) {
    return a.second_ < b.second_;
  });
  const auto n = ssize(intervals);
  assert(n > 0);
  vector<Interval> A;
  vector<int> selected(n);
  while (true) {
    index_t idx = -1;
    int overlap_count = numeric_limits<int>::max();
    for (index_t i = 0; i < n; ++i) {
      if (!selected[i] && !is_overlapping(A, intervals[i])) {
        int curr_overlap_count = 0;
        for (index_t j = 0; j < n; ++j) {
          if (i != j && !selected[j] &&
              is_overlapping(intervals[i], intervals[j])) {
            curr_overlap_count++;
          }
        }
        if (curr_overlap_count < overlap_count) {
          idx = i;
          overlap_count = curr_overlap_count;
        }
      }
    }
    if (idx == -1) {
      break;
    } else {
      A.push_back(intervals[idx]);
      selected[idx] = true;
    }
  }
  return A;
}

vector<Interval>
greedy_activity_selector_compatible_earliest(vector<Interval> &intervals) {
  ranges::sort(intervals, [](const auto &a, const auto &b) {
    return a.second_ < b.second_;
  });
  const auto n = ssize(intervals);
  assert(n > 0);
  vector<Interval> A;
  vector<int> selected(n);
  while (true) {
    index_t idx = -1;
    float start_time = numeric_limits<float>::max();
    for (index_t i = 0; i < n; ++i) {
      if (!selected[i] && !is_overlapping(A, intervals[i])) {
        if (start_time > intervals[i].first_) {
          idx = i;
          start_time = intervals[i].first_;
        }
      }
    }
    if (idx == -1) {
      break;
    } else {
      A.push_back(intervals[idx]);
      selected[idx] = true;
    }
  }
  return A;
}

unordered_map<index_t, index_t> schedule_lectures(vector<Interval> &intervals) {
  ranges::sort(intervals, [](const auto &a, const auto &b) {
    return a.second_ < b.second_;
  });
  const auto n = ssize(intervals);
  assert(n > 0);
  vector<tuple<float, float, index_t, bool>> endpoints;
  for (index_t i = 0; i < n; ++i) {
    const auto &[start, finish] = intervals[i];
    endpoints.emplace_back(start, start, i, true);
    endpoints.emplace_back(finish, start, i, false);
  }
  ranges::sort(endpoints);
  set<index_t> available;
  unordered_map<index_t, index_t> used_map;
  for (index_t i = 0; i < n; ++i) {
    available.insert(i);
  }
  for (const auto &[point, start_of, idx, is_start] : endpoints) {
    if (is_start) {
      auto least_available = *available.begin();
      available.erase(least_available);
      used_map[idx] = least_available;
    } else {
      auto being_freed = used_map[idx];
      available.insert(being_freed);
    }
  }
  return used_map;
}

vector<Interval>
dp_activity_selector_valued(vector<pair<Interval, float>> &intervals) {
  ranges::sort(intervals, [](const auto &a, const auto &b) {
    return a.first.second_ < b.first.second_;
  });
  const auto n = ssize(intervals);
  vector<vector<float>> c(n + 1, vector<float>(n + 1));
  vector<vector<index_t>> a(n + 1, vector<index_t>(n + 1, -1));
  for (index_t i = 0; i < n; ++i) {
    for (index_t j = i + 1; j <= n; ++j) {
      c[i][j] = intervals[i].second;
    }
  }
  for (index_t l = 2; l <= n; l++) {
    for (index_t i = 0; i <= n - l; i++) {
      index_t j = i + l;
      index_t k = j - 1;
      while (k > i && intervals[i].first.second_ < intervals[k].first.second_) {
        if (intervals[i].first.second_ <= intervals[k].first.first_ &&
            (j == n ||
             intervals[k].first.second_ <= intervals[j].first.first_) &&
            c[i][k] + c[k][j] + intervals[k].second > c[i][j]) {
          c[i][j] = c[i][k] + c[k][j] + intervals[k].second;
          a[i][j] = k;
        }
        k--;
      }
    }
  }
  vector<Interval> result;
  queue<pair<index_t, index_t>> q;
  q.emplace(0, n);
  while (!q.empty()) {
    auto [i, j] = q.front();
    q.pop();
    auto k = a[i][j];
    if (k == -1 || i == j) {
      continue;
    }
    result.push_back(intervals[k].first);
    q.emplace(i, k);
    q.emplace(k, j);
  }
  result.push_back(intervals[0].first);
  ranges::reverse(result);
  return result;
}

} // namespace frozenca

#endif //__CLRS4_ACTIVITY_SELECTION_H__
