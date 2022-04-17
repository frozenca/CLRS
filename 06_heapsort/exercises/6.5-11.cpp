#include <cassert>
#include <chrono>
#include <forward_list>
#include <iostream>
#include <numeric>
#include <queue>
#include <random>
#include <utility>
#include <vector>

using namespace std;

forward_list<int> merge_k_sorted_lists(const vector<forward_list<int>> &lists) {
  using it_p_t = pair<forward_list<int>::const_iterator, forward_list<int>::const_iterator>;
  
  auto node_comp = [](it_p_t &na, it_p_t &nb) {
    return *(na.first) > *(nb.first);
  };
  priority_queue<it_p_t, vector<it_p_t>, decltype(node_comp)> iters(node_comp);
  forward_list<int> merged_list;
  if (lists.empty())
    return merged_list;
  for (const auto &list : lists) {
    if (!list.empty()) {
      iters.emplace(list.cbegin(), list.cend());
    }
  }
  if (iters.empty())
    return merged_list;
  auto curr_iter = iters.top();
  merged_list.push_front(*(curr_iter.first));
  auto merged_it = merged_list.begin();
  while (curr_iter.first != curr_iter.second) {
    iters.pop();
    if (++curr_iter.first != curr_iter.second) {
      iters.push(curr_iter);
    }
    if (iters.empty()) {
      break;
    }
    curr_iter = iters.top();
    merged_it = merged_list.insert_after(merged_it, *(iters.top().first));
  }
  return merged_list;
}

int main() {
  forward_list<int> A{1, 4, 5};
  forward_list<int> B{1, 3, 4};
  forward_list<int> C{2, 6};
  vector<forward_list<int>> v{A, B, C};
  forward_list<int> ABC_correct{1, 1, 2, 3, 4, 4, 5, 6};
  auto ABC = merge_k_sorted_lists(v);
  if (ABC == ABC_correct) {
    cout << "Merge k sorted lists succeeded\n";
  } else {
    cout << "Merge k sorted lists failed\n";
  }

  constexpr int max_size = 1'000'000;
  constexpr int num_lists = 10;
  vector<forward_list<int>> v2(num_lists);
  mt19937 gen(random_device{}());
  uniform_int_distribution<> dist(0, num_lists - 1);
  for (int i = max_size - 1; i >= 0; i--) {
    v2[dist(gen)].push_front(i);
  }
  forward_list<int> v2_correct(max_size);
  iota(v2_correct.begin(), v2_correct.end(), 0);
  auto start = chrono::steady_clock::now();
  assert(merge_k_sorted_lists(v2) == v2_correct);
  auto end = chrono::steady_clock::now();
  auto diff = chrono::duration_cast<chrono::milliseconds>(end - start);
  assert(diff.count() < 1'000);
  cout << "OK\n";
}