#include <algorithm>
#include <cassert>
#include <common.h>
#include <iostream>
#include <iterator>
#include <vector>

namespace fc = frozenca;
using namespace std;

vector<fc::index_t> get_argmax(const vector<vector<int>> &monge) {
  vector<fc::index_t> res(monge.size());
  if (monge.size() == 1) {
    res[0] = distance(monge[0].begin(),
                      min_element(monge[0].begin(), monge[0].end()));
    return res;
  } else if (monge.size() < 4) {
    res[1] = distance(monge[1].begin(),
                      min_element(monge[1].begin(), monge[1].end()));
    res[0] =
        distance(monge[0].begin(),
                 min_element(monge[0].begin(), monge[0].begin() + res[1] + 1));
    if (monge.size() == 3) {
      res[2] = distance(monge[2].begin(),
                        min_element(monge[2].begin() + res[1], monge[2].end()));
    }
    return res;
  }
  vector<vector<int>> monge_evens;
  for (fc::index_t r = 1; r < ssize(monge); r += 2) {
    monge_evens.push_back(monge[r]);
  }
  vector<fc::index_t> argmax_evens = get_argmax(monge_evens);
  for (fc::index_t r = 1, re = 0; r < ssize(monge); r += 2, re++) {
    res[r] = argmax_evens[re];
  }
  if (monge.size() % 2 == 1) {
    res.push_back(monge[0].size() - 1);
  }
  res[0] =
      distance(monge[0].begin(),
               min_element(monge[0].begin(), monge[0].begin() + res[1] + 1));
  for (fc::index_t r = 2; r < ssize(monge); r += 2) {
    res[r] = distance(monge[r].begin(),
                      min_element(monge[r].begin() + res[r - 1],
                                  monge[r].begin() + res[r + 1] + 1));
  }
  if (monge.size() % 2 == 1) {
    res.pop_back();
  }
  return res;
}

int main() {

  vector<vector<int>> monge{{10, 17, 13, 28, 23}, {17, 22, 16, 29, 23},
                            {24, 28, 22, 34, 24}, {11, 13, 6, 17, 7},
                            {45, 44, 32, 37, 23}, {36, 33, 19, 21, 6},
                            {75, 66, 51, 53, 34}};

  auto argmaxes = get_argmax(monge);
  for (auto n : argmaxes) {
    cout << n << '\n';
  }
}