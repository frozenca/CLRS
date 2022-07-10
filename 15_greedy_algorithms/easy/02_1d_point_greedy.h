#ifndef __CLRS4_1D_POINT_GREEDY_H__
#define __CLRS4_1D_POINT_GREEDY_H__

#include <algorithm>
#include <cassert>
#include <common.h>
#include <interval.h>
#include <limits>
#include <vector>

namespace frozenca {

using namespace std;

vector<index_t> skating(vector<float> &highway, float m) {
  ranges::sort(highway);
  const auto n = ssize(highway);
  assert(n > 1);
  vector<index_t> stops;
  index_t curr = 0;
  stops.push_back(0);
  while (curr < n - 1) {
    index_t probe = curr + 1;
    while (probe < n && highway[probe] <= highway[curr] + m) {
      ++probe;
    }
    if (probe == curr + 1) {
      break;
    }
    curr = probe - 1;
    stops.push_back(curr);
  }
  return stops;
}

vector<Interval> unit_length_intervals(vector<float> &points) {
  ranges::sort(points);
  vector<Interval> s;
  float start = numeric_limits<float>::lowest();
  for (auto point : points) {
    if (start == numeric_limits<float>::lowest() || point > start + 1.0f) {
      start = point;
      s.emplace_back(start, start + 1.0f);
    }
  }
  return s;
}

} // namespace frozenca

#endif //__CLRS4_1D_POINT_GREEDY_H__
