#include <algorithm>
#include <cassert>
#include <common.h>
#include <iostream>
#include <limits>
#include <numeric>
#include <random>
#include <utility>
#include <vector>

namespace fc = frozenca;
using namespace std;

int extract_min(vector<vector<int>> &tableau) {
  int val = tableau[0][0];
  tableau[0][0] = numeric_limits<int>::max();
  pair<fc::index_t, fc::index_t> cur_pos = {0, 0};
  auto R = ssize(tableau);
  auto C = ssize(tableau[0]);
  while (true) {
    auto [r, c] = cur_pos;
    int min_val = tableau[r][c];
    auto min_r = r;
    auto min_c = c;
    if (r < R - 1 && min_val > tableau[r + 1][c]) {
      min_val = tableau[r + 1][c];
      min_r = r + 1;
    }
    if (c < C - 1 && min_val > tableau[r][c + 1]) {
      min_val = tableau[r][c + 1];
      min_r = r;
      min_c = c + 1;
    }
    if (min_r == r && min_c == c) {
      break;
    } else {
      swap(tableau[r][c], tableau[min_r][min_c]);
      cur_pos = {min_r, min_c};
    }
  }
  return val;
}

void insert_key(vector<vector<int>> &tableau, int key) {
  auto R = ssize(tableau);
  auto C = ssize(tableau[0]);
  tableau[R - 1][C - 1] = key;
  pair<fc::index_t, fc::index_t> cur_pos = {R - 1, C - 1};
  while (true) {
    auto [r, c] = cur_pos;
    int max_val = tableau[r][c];
    fc::index_t max_r = r;
    fc::index_t max_c = c;
    if (r > 0 && max_val < tableau[r - 1][c]) {
      max_val = tableau[r - 1][c];
      max_r = r - 1;
    }
    if (c > 0 && max_val < tableau[r][c - 1]) {
      max_val = tableau[r][c - 1];
      max_r = r;
      max_c = c - 1;
    }
    if (max_r == r && max_c == c) {
      break;
    } else {
      swap(tableau[r][c], tableau[max_r][max_c]);
      cur_pos = {max_r, max_c};
    }
  }
}

bool find_element(const vector<vector<int>> &tableau, int key) {
  auto R = ssize(tableau);
  auto C = ssize(tableau[0]);
  pair<fc::index_t, fc::index_t> cur_pos = {0, C - 1};
  while (cur_pos.second < C && cur_pos.first < R) {
    int cur_key = tableau[cur_pos.first][cur_pos.second];
    if (cur_key == key) {
      return true;
    } else if (cur_key < key) {
      cur_pos.first++;
    } else {
      cur_pos.second--;
    }
  }
  return false;
}

int main() {
  constexpr int INF = numeric_limits<int>::max();

  vector<vector<int>> tableau{
      {2, 3, 12, 14}, {4, 8, 16, INF}, {5, 9, INF, INF}, {INF, INF, INF, INF}};

  extract_min(tableau);

  for (const auto &row : tableau) {
    for (auto n : row) {
      cout << n << ' ';
    }
    cout << '\n';
  }

  insert_key(tableau, 2);

  for (const auto &row : tableau) {
    for (auto n : row) {
      cout << n << ' ';
    }
    cout << '\n';
  }

  vector<vector<int>> emptytableau{{INF, INF, INF, INF},
                                   {INF, INF, INF, INF},
                                   {INF, INF, INF, INF},
                                   {INF, INF, INF, INF}};

  vector<int> v(emptytableau.size() * emptytableau[0].size());
  iota(v.begin(), v.end(), 1);
  mt19937 gen(random_device{}());
  ranges::shuffle(v, gen);

  for (auto n : v) {
    insert_key(emptytableau, n);
  }

  for (const auto &row : emptytableau) {
    for (auto n : row) {
      cout << n << ' ';
    }
    cout << '\n';
  }

  v.clear();

  for (fc::index_t i = 0; i < ssize(emptytableau) * ssize(emptytableau[0]);
       i++) {
    v.push_back(extract_min(emptytableau));
  }

  for (auto n : v) {
    cout << n << ' ';
  }
  cout << '\n';

  assert(find_element(tableau, 9));
  assert(!find_element(tableau, 17));
}