#ifndef __CLRS4_RECTANGLE_OVERLAP_H__
#define __CLRS4_RECTANGLE_OVERLAP_H__

#include <17_augmenting_data_structures/easy/03_interval_tree.h>
#include <algorithm>
#include <cassert>
#include <common.h>
#include <ranges>
#include <tuple>
#include <vector>

namespace frozenca {

using namespace std;

bool overlap_exist(vector<Rectangle2d> &rectangles) {
  vector<tuple<float, float, float, bool>> y_intervals;
  for (const auto &[x, y, w, h] : rectangles) {
    y_intervals.emplace_back(x, y, y + h, true);
    y_intervals.emplace_back(x + w, y, y + h, false);
  }
  ranges::sort(y_intervals);

  IntervalTree tree;
  for (const auto &[x, y_b, y_e, left] : y_intervals) {
    if (left) {
      if (tree.overlaps({y_b, y_e})) {
        return true;
      }
      tree.insert({y_b, y_e});
    } else {
      tree.erase_exact({y_b, y_e});
    }
  }
  return false;
}

} // namespace frozenca

#endif //__CLRS4_RECTANGLE_OVERLAP_H__
