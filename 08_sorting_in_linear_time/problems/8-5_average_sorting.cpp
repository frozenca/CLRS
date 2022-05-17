#include <algorithm>
#include <cassert>
#include <common.h>
#include <iostream>
#include <iterator>
#include <numeric>
#include <queue>
#include <random>
#include <ranges>
#include <vector>


namespace fc = frozenca;
using namespace std;

void average_sort(vector<int> &v, fc::index_t k) {
  mt19937 gen(random_device{}());
  vector<vector<int>> V(k);
  for (fc::index_t i = 0; i < ssize(v); i++) {
    V[i % k].push_back(v[i]);
  }
  for (auto &V_ : V) {
    ranges::sort(V_);
  }
  for (fc::index_t m = 0; m < k; m++) {
    for (fc::index_t i = 0; i < ssize(V[m]); i++) {
      v[m + i * k] = V[m][i];
    }
  }
  for (fc::index_t i = 0; i < ssize(v) - k; i++) {
    assert(v[i] <= v[i + k]);
  }
}

using it_p_t = pair<vector<int>::const_iterator, vector<int>::const_iterator>;

vector<int> merge_k_lists(const vector<vector<int>> &lists) {
  auto node_comp = [](it_p_t &na, it_p_t &nb) {
    return *(na.first) > *(nb.first);
  };
  priority_queue<it_p_t, vector<it_p_t>, decltype(node_comp)> iters(node_comp);
  vector<int> merged_vec;
  if (lists.empty())
    return merged_vec;
  for (const auto &list : lists) {
    if (!list.empty()) {
      iters.emplace(list.cbegin(), list.cend());
    }
  }
  if (iters.empty())
    return merged_vec;
  auto curr_iter = iters.top();
  merged_vec.push_back(*(curr_iter.first));
  while (curr_iter.first != curr_iter.second) {
    iters.pop();
    if (++curr_iter.first != curr_iter.second) {
      iters.push(curr_iter);
    }
    if (iters.empty()) {
      break;
    }
    curr_iter = iters.top();
    merged_vec.push_back(*(iters.top().first));
  }
  return merged_vec;
}

void average_sort2(vector<int> &v, fc::index_t k) {
  vector<vector<int>> V(k);
  for (fc::index_t i = 0; i < ssize(v); i++) {
    V[i % k].push_back(v[i]);
  }
  for (auto &V_ : V) {
    ranges::sort(V_);
  }
  v = merge_k_lists(V);
}

int main() {
  constexpr int N = 500;

  mt19937 gen(random_device{}());
  vector<int> v(N);
  iota(v.begin(), v.end(), 0);
  ranges::shuffle(v, gen);
  constexpr fc::index_t k = 10;
  average_sort(v, k);

  for (int i = 0; i < N; i++) {
    cout << v[i] << ' ';
    if (i % k == (k - 1)) {
      cout << '\n';
    }
  }

  ranges::shuffle(v, gen);
  average_sort2(v, k);
  if (ranges::is_sorted(v)) {
    cout << "Sorted\n";
  } else {
    cout << "Sorting failed\n";
  }
}