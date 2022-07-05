#ifndef __CLRS4_FA_PLAYERS_H__
#define __CLRS4_FA_PLAYERS_H__

#include <algorithm>
#include <cassert>
#include <common.h>
#include <iostream>
#include <iterator>
#include <ranges>
#include <tuple>
#include <vector>

namespace frozenca {

using namespace std;

struct FAPlayer {
  index_t cost_ = 0;
  float WAR_ = 0.0f;
};

tuple<float, index_t, vector<FAPlayer>>
signing_FA(index_t budget, const vector<vector<FAPlayer>> &players) {
  const auto positions = ssize(players);
  assert(positions > 0 && budget >= 0);
  vector<vector<float>> f(positions + 1, vector<float>(budget + 1));
  vector<vector<index_t>> players_index(positions, vector<index_t>(budget + 1));
  for (index_t pos = 1; pos <= positions; pos++) {
    for (index_t j = 0; j <= budget; j++) {
      f[pos][j] = f[pos - 1][j]; // don't sign
      for (index_t p = 0; p < ssize(players[pos - 1]); ++p) {
        const auto &[cost, WAR] = players[pos - 1][p];
        if (j >= cost && f[pos - 1][j - cost] + WAR > f[pos][j]) {
          f[pos][j] = f[pos - 1][j - cost] + WAR; // sign
          players_index[pos - 1][j] = p;
        }
      }
    }
  }

  auto max_WAR_iter = ranges::max_element(f[positions]);
  float max_WAR = *max_WAR_iter;
  index_t used_money = distance(f[positions].begin(), max_WAR_iter);

  vector<FAPlayer> to_sign;
  index_t curr_money = used_money;
  for (index_t i = positions - 1; i >= 0; --i) {
    auto player = players[i][players_index[i][curr_money]];
    to_sign.push_back(player);
    curr_money -= player.cost_;
  }
  ranges::reverse(to_sign);
  return {max_WAR, used_money, move(to_sign)};
}

} // namespace frozenca

#endif //__CLRS4_FA_PLAYERS_H__
