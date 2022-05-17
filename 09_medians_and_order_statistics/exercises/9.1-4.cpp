#include <algorithm>
#include <cassert>
#include <common.h>
#include <iostream>
#include <numeric>
#include <random>
#include <ranges>
#include <vector>

namespace fc = frozenca;
using namespace std;

pair<int, int> minmax(const vector<int> &A) {
  vector<int> maxmin_set, max_set, min_set, none_set;
  maxmin_set = A;
  while (true) {
    if (maxmin_set.size() >= 2) {
      int a = maxmin_set[maxmin_set.size() - 1];
      int b = maxmin_set[maxmin_set.size() - 2];
      if (a > b) {
        max_set.push_back(a);
        min_set.push_back(b);
      } else {
        max_set.push_back(b);
        min_set.push_back(a);
      }
      maxmin_set.pop_back();
      maxmin_set.pop_back();
    } else if (!maxmin_set.empty() && !none_set.empty()) {
      int a = maxmin_set.back();
      max_set.push_back(a);
      maxmin_set.pop_back();
    } else if (min_set.size() >= 2) {
      int a = min_set[min_set.size() - 1];
      int b = min_set[min_set.size() - 2];
      min_set.pop_back();
      min_set.pop_back();
      if (a > b) {
        none_set.push_back(a);
        min_set.push_back(b);
      } else {
        none_set.push_back(b);
        min_set.push_back(a);
      }
    } else if (max_set.size() >= 2) {
      int a = max_set[max_set.size() - 1];
      int b = max_set[max_set.size() - 2];
      max_set.pop_back();
      max_set.pop_back();
      if (a < b) {
        none_set.push_back(a);
        max_set.push_back(b);
      } else {
        none_set.push_back(b);
        max_set.push_back(a);
      }
    } else {
      break;
    }
  }
  assert(min_set.size() == 1 && max_set.size() == 1 && maxmin_set.empty());
  return {min_set[0], max_set[0]};
}

int main() {
  vector<int> v{1, 5, 3, 2, 6};
  auto [min_elem, max_elem] = minmax(v);
  cout << min_elem << ' ' << max_elem;
}