#include <15_greedy_algorithms/inc/easy.h>
#include <cassert>
#include <iostream>
#include <vector>

int main() {
  namespace fc = frozenca;
  using namespace std;

  auto [p, k, d, q] = fc::coin_change_us(87);
  cout << p << ' ' << k << ' ' << d << ' ' << q << '\n';
  auto coin_powers = fc::coin_change_powers(87, 4, 4);
  for (auto coin : coin_powers) {
    cout << coin << ' ';
  }
  cout << '\n';
  auto [total_coins, coins] = fc::coin_change_general(87, {1, 5, 10, 25});
  cout << total_coins << '\n';
  for (auto coin : coins) {
    cout << coin << ' ';
  }
  cout << '\n';
}
