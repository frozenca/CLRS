#include <14_dynamic_programming/inc/easy.h>
#include <cassert>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

int main() {
  namespace fc = frozenca;
  using namespace std;

  mt19937 gen(random_device{}());
  uniform_real_distribution<float> WAR_dist(0.01f, 0.20f);
  uniform_int_distribution<fc::index_t> cost_dist(20, 100);

  constexpr int num_positions = 9;
  constexpr int num_players_per_pos = 10;

  vector<vector<fc::FAPlayer>> players(num_positions);

  for (int i = 0; i < num_positions; ++i) {
    for (int j = 0; j < num_players_per_pos; ++j) {
      players[i].emplace_back(cost_dist(gen), WAR_dist(gen));
    }
  }

  constexpr fc::index_t budget = 300ll;

  auto [max_WAR, used_money, signed_players] = fc::signing_FA(budget, players);

  cout << "Max WAR: " << max_WAR << '\n';
  cout << "Used money: " << used_money << '\n';
  cout << "Signed players: " << '\n';
  for (auto [cost, WAR] : signed_players) {
    cout << "Cost " << cost << ", WAR " << WAR << '\n';
  }
  cout << '\n';
}
